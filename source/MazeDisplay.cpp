//
// Created by Timothée Couble on 16/02/2020.
//

#include <MazeDisplay.hpp>

scene::MazeDisplay::MazeDisplay() : _floor(WIDTH * HEIGHT), _maze(WIDTH, HEIGHT) {}

void scene::MazeDisplay::init() {
    backstage::walls_t _test = _maze.getWalls();
    const glm::vec2 size(_maze.getWidth(), _maze.getLength());

    for (const auto &it : _test) {
        auto *cube = new scene::Wall();
        const glm::vec2 &start = it[0];
        const glm::vec2 &end = it[1];
        cube->setPosition(glm::vec3(start.x - size.x / 2, 1.0f, start.y - size.y / 2));
        cube->setShape(glm::vec3(1.0f + end.x - start.x, 1.0f, 1.0f + end.y - start.y));

        _walls.push_back(cube);
    }

    initFloor(size);
}

void scene::MazeDisplay::draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders) {
    for (const auto &it : _walls)
        it->draw(models, shaders);
    for (auto &shader : shaders)
        if (shader->getType() == gl_wrapper::ShaderType::INSTANCE)
            _floor.draw(shader);
}

void scene::MazeDisplay::reset() {
    _maze.resetPathFromEnd();
    changeFloorColor();
}

bool scene::MazeDisplay::update(const glm::vec3 playerPosition) {
    const glm::vec2 size(_maze.getWidth(), _maze.getLength());
    glm::vec2 start = _maze.getStart();
    glm::vec2 pos((playerPosition.x) / 2.0f + size.x / 2.0f, (playerPosition.z) / 2.0f + size.y / 2.0f);
    if ((int) start.x != (int) pos.x || (int) start.y != (int) pos.y) {
        glm::vec2 newPos((int) pos.x, (int) pos.y);
        _maze.setStart(newPos);
        changeFloorColor();
    }

    glm::vec2 end = _maze.getEnd();
    return ((int) end.x == (int) pos.x && (int) end.y == (int) pos.y);
}

void scene::MazeDisplay::clear() {
    for (const auto &it : _walls)
        delete it;
}

void scene::MazeDisplay::initFloor(const glm::vec2 &size) {
    const size_t chunk_nb = size.x * size.y;

    unsigned int i = 0;
    for (unsigned int x = 0; x < (int) size.x; x++) {
        for (unsigned int y = 0; y < (int) size.y; y++) {
            const glm::vec2 pos((float) x - size.x / 2, (float) y - size.y / 2);

            glm::mat4 translate = glm::translate(glm::mat4(1.0f),
                    glm::vec3(pos.x * 2.0f + 1.0f, -0.1f, pos.y * 2.0f + 1.0f));
            glm::mat4 rotate = glm::mat4(1.0f);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 0.2f, 2.0f));

            _floor[i].matrix = translate * rotate * scale;
            i++;
        }
    }
    changeFloorColor();
    _floor.updateModel(chunk_nb);
}

void scene::MazeDisplay::changeFloorColor() {
    const glm::vec2 size(_maze.getWidth(), _maze.getLength());
    const size_t chunk_nb = size.x * size.y;
    std::stack<glm::vec2> path = _maze.getAStar();
    glm::vec2 end = _maze.getEnd();

    for (int i = 0; i < chunk_nb; i++)
        _floor[i].color = _defaultColor;
    while (path.size() > 1) {
        glm::vec2 &pos = path.top();
        _floor[pos.x * size.y + pos.y].color = _pathColor;
        path.pop();
    }
    _floor[end.x * size.y + end.y].color = _endColor;

    _floor.updateModel(chunk_nb);
}

bool scene::MazeDisplay::intersectSquare(glm::vec3 center, float size) const {
    const glm::vec3 min(center - glm::vec3(size / 2.0f));
    const glm::vec3 max(center + glm::vec3(size / 2.0f));

    if (min.y < 0.0f)
        return true;
    for (auto &wall : _walls)
        if (Intersect::squares(wall->getMin(), wall->getMax(), min, max))
            return true;
    return false;
}

scene::normal_collision_t scene::MazeDisplay::intersectSphereWalls(glm::vec3 center, float radius) const {
    for (auto &wall : _walls)
        if (Intersect::sphereSquare(wall->getMin(), wall->getMax(), center, radius))
            return Intersect::vectorNormal(wall->getPosition(), wall->getSize(), center);
    return std::make_tuple(false, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f));
}

scene::normal_collision_t scene::MazeDisplay::intersectSphereFloor(glm::vec3 center, float radius) const {
    const glm::vec2 size(_maze.getWidth(), _maze.getLength());
    const glm::vec3 floorMin((-size.x / 2) * 2.0f + 1.0f, -0.2f, (-size.y / 2) * 2.0f + 1.0f);
    const glm::vec3 floorMax((size.x / 2) * 2.0f + 1.0f, 0.0f, (size.y / 2) * 2.0f + 1.0f);

    if (center.y - radius < 0.01f)
        return std::make_tuple(true, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, center.y, 0.0f));
    return std::make_tuple(false, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f));
}

const glm::vec2 scene::MazeDisplay::getStart() const {
    auto start = _maze.getStart();
    const glm::vec2 size(_maze.getWidth(), _maze.getLength());
    return glm::vec2((start.x - size.x / 2.0f) * 2.0f + 1.0f, (start.y - size.y / 2.0f) * 2.0f + 1.0f);
}

const glm::vec2 scene::MazeDisplay::getEnd() const {
    auto end = _maze.getEnd();
    const glm::vec2 size(_maze.getWidth(), _maze.getLength());
    return glm::vec2((end.x - size.x / 2.0f) * 2.0f + 1.0f, (end.y - size.y / 2.0f) * 2.0f + 1.0f);
}
