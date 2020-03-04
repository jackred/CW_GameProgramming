//
// Created by Timothée Couble on 16/02/2020.
//

#include "Wall.hpp"

scene::Wall::Wall() : AObject(ModelType::CUBE) {
    _size = _size * glm::vec3(2.0f, 1.2f, 2.0f);
}

void scene::Wall::setPosition(glm::vec3 position) {
    AObject::setPosition(position * _size);
}

void scene::Wall::setShape(glm::vec3 shape) {
    shape.x = -shape.x;
    shape.z = -shape.z;
    _shape = shape;
    _offset = (_size * _shape) / glm::vec3(2);
}

const glm::vec3 scene::Wall::getMin() const {
    glm::vec3 size(_size * _shape);
    size.y = -size.y;

    return _position - _offset + size / 2.0f;
}

const glm::vec3 scene::Wall::getMax() const {
    glm::vec3 size(_size * _shape);
    size.y = -size.y;

    return _position - _offset - size / 2.0f;
}
