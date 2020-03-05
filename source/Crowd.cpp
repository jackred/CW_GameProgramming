//
// Created by Timothée Couble on 03/03/2020.
//

#include "Crowd.hpp"

unsigned int scene::Crowd::_idGen = 0;

scene::Crowd::Crowd() : _speed(0.0f), _id(_idGen++), _ball(ModelType::CROWD) {
    _ball.setSize(glm::vec3(_size));
}

// Drawing method for a crowd
void scene::Crowd::draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders) {
    _collidePlayer = false;
    for (auto collide : _collideCrowd)
        collide = false;
    _ball.draw(models, shaders);
}

// Reset crowd position based on the number of crowd in total and the position of the payer
void scene::Crowd::reset(const scene::MazeDisplay &maze) {
    auto &start = maze.getStart();
    glm::vec2 offset(std::sin(_id * (360.0f / _idGen) * M_PI / 180.0f),
                     std::cos(_id * (360.0f / _idGen) * M_PI / 180.0f));
    _speed = glm::vec3(0.0f);
    _ball.setPosition(glm::vec3(start.x + offset.x / 1.3f, 1.0f, start.y + offset.y / 1.3f));
    _collideCrowd.resize(_idGen);
}

// Calculate speed vector for the boids model
// Base on three vector gravity, repel and follow
glm::vec3 scene::Crowd::boids(const glm::vec3 &player, const std::vector<Crowd *> &crowds) {
    glm::vec3 gravityVelocity(0.0f);
    glm::vec3 repelVelocity(0.0f);
    glm::vec3 pos = getPosition();
    glm::vec3 followVelocity = player - pos;
    for (auto &crowd : crowds) {
        if (crowd->getId() != _id) {
            gravityVelocity += crowd->getPosition();
            if (glm::distance(pos, crowd->getPosition()) < _maxSpace)
                repelVelocity += pos - crowd->getPosition();
        }
    }
    gravityVelocity /= crowds.size() - 1;
    glm::vec3 res = 0.01f * gravityVelocity + 0.5f * repelVelocity + 0.2f * followVelocity;
    res = scaleMax(res);
    return res;
}

// Update the crowd position and check all the collisions
void scene::Crowd::update(const scene::MazeDisplay &maze, const glm::vec3 &player, const std::vector<Crowd *> &crowds) {
    double currentTime = glfwGetTime();
    double delta = currentTime - _lastTime;
    _lastTime = currentTime;

    if (_doBoids)
        _speed += boids(player, crowds);
    // Apply gravity
    _speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float) delta * 0.5f;

    collideWithWalls(maze, delta);
    collideWithFloor(maze, delta);
    collideWithPlayer(player, delta);
    collideWithCrowd(crowds, delta);

    _speed = scaleMax(_speed);
    auto pos = _ball.getPosition();
    if (pos.y < 0.0f)
        _ball.setPosition(glm::vec3(pos.x, 0.11f, pos.z));

}

// Prevent the crowd to collide with the player
void scene::Crowd::collideWithPlayer(const glm::vec3 &player, double &delta) {
    if (_collidePlayer)
        return;
    glm::vec3 newPos = _ball.getPosition() + _speed * (float) delta;
    if (Intersect::spheres(newPos, _size, player, 0.2f))
        _speed = -_speed;
    _collidePlayer = true;
}

// Prevent the crowd to collide with other crowd
void scene::Crowd::collideWithCrowd(const std::vector<Crowd *> &crowds, double &delta) {
    for (auto &ball : crowds) {
        auto &id = ball->getId();
        if (id != _id && !_collideCrowd[id]) {
            glm::vec3 newPos = _ball.getPosition() + _speed * (float) delta;
            if (Intersect::spheres(newPos, _size, ball->getPosition(), _size)) {
                _speed = -_speed;
                _collideCrowd[id] = true;
                //ball->inverseSpeed(_id);
            }
        }
    }

}

// Prevent the crowd to collide with walls
void scene::Crowd::collideWithWalls(const scene::MazeDisplay &maze, double &delta) {
    normal_collision_t collision;
    auto pos = _ball.getPosition();
    glm::vec3 newPos = pos + _speed * (float) delta;

    try {
        collision = maze.intersectSphereWalls(newPos, _size);
    } catch (const std::runtime_error &e) {
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
void scene::Crowd::collideWithFloor(const scene::MazeDisplay &maze, double &delta) {
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
glm::vec3 scene::Crowd::doCollision(scene::normal_collision_t &collision, glm::vec3 &pos, glm::vec3 &newPos) {
    const glm::vec3 &normal = std::get<1>(collision);
    const glm::vec3 speedThreshold = normal * _speed;

    if (glm::length(glm::abs(speedThreshold)) < 0.15f) {
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

void scene::Crowd::toggleBoids() {
    if (_lastTime - _lastTimeToggle > 1.0f) {
        _lastTimeToggle = glfwGetTime();
        _doBoids = !_doBoids;
    }
}

const glm::vec3 scene::Crowd::getPosition() const {
    return _ball.getPosition();
}

const float &scene::Crowd::getSize() const {
    return _size;
}

const unsigned int &scene::Crowd::getId() const {
    return _id;
}
