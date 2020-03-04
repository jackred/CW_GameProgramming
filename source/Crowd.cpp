//
// Created by Timothée Couble on 03/03/2020.
//

#include "Crowd.hpp"

unsigned int scene::Crowd::_idGen = 0;

scene::Crowd::Crowd() : _speed(0.0f), _id(_idGen++) {
    _ball.setSize(glm::vec3(0.1f));
}

std::ostream &operator<<(std::ostream &stream, const glm::vec3 &vec) {
    stream << vec.x << " " << vec.y << " " << vec.z;
    return stream;
}

void scene::Crowd::draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders) {
    _collidePlayer = false;
    for (auto collide : _collideCrowd)
        collide = false;

    _ball.draw(models, shaders);
}

void scene::Crowd::reset(const scene::MazeDisplay &maze) {
    auto &start = maze.getStart();
    glm::vec2 offset(std::sin(_id * (360.0f / _idGen) * M_PI / 180.0f),
            std::cos(_id * (360.0f / _idGen) * M_PI / 180.0f));
    _speed = glm::vec3(0.0f);
    _ball.setPosition(glm::vec3(start.x + offset.x / 1.5f, 1.0f, start.y + offset.y / 1.5f));
    _collideCrowd.resize(_idGen);
}

void scene::Crowd::update(const scene::MazeDisplay &maze, const glm::vec3 &player, const std::vector<Crowd *> &crowds) {
    double currentTime = glfwGetTime();
    double delta = currentTime - _lastTime;
    _lastTime = currentTime;

    // Apply gravity
    _speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float) delta * 0.5f;

    collideWithFloor(maze, delta);
    collideWithWalls(maze, delta);
    collideWithPlayer(player, delta);
    collideWithCrowd(crowds, delta);

    _speed = scaleMax(_speed);
    auto pos = _ball.getPosition();
    if (pos.y < 0.0f)
        _ball.setPosition(glm::vec3(pos.x, 0.11f, pos.z));
}

void scene::Crowd::collideWithPlayer(const glm::vec3 &player, double &delta) {
    if (_collidePlayer)
        return;
    glm::vec3 newPos = _ball.getPosition() + _speed * (float) delta;
    if (Intersect::spheres(newPos, 0.1f, player, 0.2f))
        _speed = -_speed;
    _collidePlayer = true;
}

void scene::Crowd::collideWithCrowd(const std::vector<Crowd *> &crowds, double &delta) {
    for (auto &ball : crowds) {
        auto &id = ball->getId();
        if (id != _id && !_collideCrowd[id]) {
            glm::vec3 newPos = _ball.getPosition() + _speed * (float) delta;
            if (Intersect::spheres(newPos, 0.1f, ball->getPosition(), 0.1f)) {
                _speed = -_speed;
                _collideCrowd[id] = true;
                ball->inverseSpeed(_id);
            }
        }
    }
}

void scene::Crowd::collideWithWalls(const scene::MazeDisplay &maze, double &delta) {
    normal_collision_t collision;
    auto pos = _ball.getPosition();
    glm::vec3 newPos = pos + _speed * (float) delta;

    try {
        collision = maze.intersectSphereWalls(newPos, 0.1f);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        reset(maze);
        return;
    }
    if (std::get<0>(collision)) {
        pos += doCollision(collision, pos, newPos) * (float) delta;
    } else
        pos = newPos;
    _ball.setPosition(pos);

}

void scene::Crowd::collideWithFloor(const scene::MazeDisplay &maze, double &delta) {
    auto pos = _ball.getPosition();
    glm::vec3 newPos = pos + _speed * (float) delta;

    normal_collision_t collision = maze.intersectSphereFloor(newPos, 0.1f);
    if (std::get<0>(collision)) {
        pos += doCollision(collision, pos, newPos) * (float) delta;
    } else
        pos = newPos;
    _ball.setPosition(pos);
}

glm::vec3 scene::Crowd::doCollision(scene::normal_collision_t &collision, glm::vec3 &pos, glm::vec3 &newPos) {
    const glm::vec3 &normal = std::get<1>(collision);
    const glm::vec3 speedThreshold = normal * _speed;

    if (glm::length(glm::abs(speedThreshold)) < 0.5f) {
        _speed -= speedThreshold;
        return _speed;
    } else {
        const glm::vec3 offset = normal * 0.1f;
        const glm::vec3 adj_distance = -std::get<2>(collision) - glm::abs(normal) * (pos - newPos) + offset;
        // Hypotenuse = adj / cos(a)
        // Note: cos(acos(a)) == a
        const glm::vec3 hypotenuse = adj_distance / glm::dot(glm::normalize(_speed), glm::normalize(adj_distance));
        const float ratio = glm::length(hypotenuse) / glm::distance(pos, newPos + offset);

        const glm::vec3 oldSpeed = _speed;
        _speed = _speed - 2 * glm::dot(_speed, normal) * normal;
        _speed *= 0.85f; // collision speed cost

        return _speed * (1 - ratio) + oldSpeed * ratio;
    }
}

glm::vec3 &scene::Crowd::scaleMax(glm::vec3 &velocity) {
    auto max = std::max(velocity.x, std::max(velocity.y, velocity.z));
    if (max > _maxSpeed) {
        float ratio = _maxSpeed / max;
        velocity *= ratio;
    }
    return velocity;
}

void scene::Crowd::setPlayerSpeed(const glm::vec3 &speed) {
    _speed = _speed * 0.5f + speed;
    _speed = scaleMax(_speed);
    _collidePlayer = true;
}

void scene::Crowd::inverseSpeed(const unsigned int &otherId) {
    if (!_collideCrowd[otherId]) {
        _speed = -_speed;
        _collideCrowd[otherId] = true;
    }
}

void scene::Crowd::doJump() {
    auto pos = _ball.getPosition();
    if (pos.y < 0.3f && _speed.y < 1.0f)
        _speed += glm::vec3(0.0f, 1.8f, 0.0f);
}

const glm::vec3 scene::Crowd::getPosition() const {
    return _ball.getPosition();
}

const unsigned int &scene::Crowd::getId() const {
    return _id;
}
