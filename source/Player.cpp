//
// Created by Timothée Couble on 03/03/2020.
//

#include "Player.hpp"

scene::Player::Player() : _ball(ModelType::BALL), _speed(0.0f) {
    _ball.setSize(glm::vec3(_size));
}

// Drawing method for a player
void scene::Player::draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders) {
    _ball.draw(models, shaders);
}

// Reset crowd position based on the starting point on the maze
void scene::Player::reset(const MazeDisplay &maze) {
    auto &start = maze.getStart();
    _speed = glm::vec3(0.0f);
    _ball.setPosition(glm::vec3(start.x, 1.0f, start.y));
}

// Update the player position and check all the collisions
void scene::Player::update(scene::Camera_ptr_t &camera, const scene::MazeDisplay &maze, std::vector<Crowd *> &crowd) {
    double currentTime = glfwGetTime();
    double delta = currentTime - _lastTime;
    _lastTime = currentTime;

    // Apply gravity
    _speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float) delta * 0.5f;

    collideWithFloor(maze, delta);
    collideWithWalls(maze, delta);

    _speed = scaleMax(_speed);
    collideWithCrowd(crowd, delta);

    auto pos = _ball.getPosition();
    if (_lock) {
        const glm::vec3 dir(0.0f, -12.0f, -6.0f);
        camera->setCameraPosition(pos - dir);
        camera->setCameraFront(dir);
    }
    if (pos.y < 0.0f)
        _ball.setPosition(glm::vec3(pos.x, 0.21f, pos.z));
}

// Prevent the player to collide with other crowd
void scene::Player::collideWithCrowd(std::vector<Crowd *> &crowd, double &delta) {
    for (auto &ball : crowd) {
        glm::vec3 newPos = _ball.getPosition() + _speed * (float) delta;
        if (Intersect::spheres(newPos, _size, ball->getPosition(), ball->getSize()))
            ball->setPlayerSpeed(_speed);
    }
}

// Prevent the crowd to collide with walls
void scene::Player::collideWithWalls(const MazeDisplay &maze, double &delta) {
    normal_collision_t collision;
    auto pos = _ball.getPosition();
    glm::vec3 newPos = pos + _speed * (float) delta;

    try {
        collision = maze.intersectSphereWalls(newPos, _size);
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

// Prevent the crowd to collide with floor
void scene::Player::collideWithFloor(const MazeDisplay &maze, double &delta) {
    auto pos = _ball.getPosition();
    glm::vec3 newPos = pos + _speed * (float) delta;

    normal_collision_t collision = maze.intersectSphereFloor(newPos, _size);
    if (std::get<0>(collision)) {
        pos += doCollision(collision, pos, newPos) * (float) delta;
    } else
        pos = newPos;
    _ball.setPosition(pos);
}

// Collision private method where we are calculating the new speed and new position when a collision from happening
glm::vec3 scene::Player::doCollision(normal_collision_t &collision, glm::vec3 &pos, glm::vec3 &newPos) {
    const glm::vec3 &normal = std::get<1>(collision);
    const glm::vec3 speedThreshold = normal * _speed;

    if (glm::length(glm::abs(speedThreshold)) < 0.5f) {
        _speed -= speedThreshold;
        return _speed;
    } else {
        const glm::vec3 offset = normal * _size;
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

// Scale on the max speed
glm::vec3 &scene::Player::scaleMax(glm::vec3 &velocity) {
    auto max = std::max(velocity.x, std::max(velocity.y, velocity.z));
    if (max > _maxSpeed) {
        float ratio = _maxSpeed / max;
        velocity *= ratio;
    }
    return velocity;
}

void scene::Player::goUp() {
    _speed += glm::vec3(0.0f, 0.0f, -_velocity);
    if (_speed.z < -_maxSpeed)
        _speed.z = -_maxSpeed;
}

void scene::Player::goRight() {
    _speed += glm::vec3(_velocity, 0.0f, 0.0f);
    if (_speed.x > _maxSpeed)
        _speed.x = _maxSpeed;

}

void scene::Player::goDown() {
    _speed += glm::vec3(0.0f, 0.0f, _velocity);
    if (_speed.z > _maxSpeed)
        _speed.z = _maxSpeed;
}

void scene::Player::goLeft() {
    _speed += glm::vec3(-_velocity, 0.0f, 0.0f);
    if (_speed.x < -_maxSpeed)
        _speed.x = -_maxSpeed;

}

void scene::Player::doJump() {
    auto pos = _ball.getPosition();
    if (pos.y < 0.3f && _speed.y < 1.0f)
        _speed += glm::vec3(0.0f, 1.8f, 0.0f);
}

void scene::Player::unLockCamera() {
    static double oldTime = 0;

    if (_lastTime - oldTime > 0.5f) {
        oldTime = glfwGetTime();
        _lock = !_lock;
    }
}

const glm::vec3 scene::Player::getPosition() const {
    return _ball.getPosition();
}
