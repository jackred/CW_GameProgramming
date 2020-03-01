//
// Created by Timothée Couble on 16/02/2020.
//

#include "Ball.hpp"

scene::Ball::Ball() : AObject(ModelType::BALL) {
    _size = _size * glm::vec3(0.4f);
}

void scene::Ball::setPosition(glm::vec3 position) {
    AObject::setPosition(position * _size);
}
