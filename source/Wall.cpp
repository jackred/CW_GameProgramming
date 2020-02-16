//
// Created by Timothée Couble on 16/02/2020.
//

#include "Wall.hpp"

scene::Wall::Wall() : AObject(ModelType::CUBE) {
    _size = _size * glm::vec3(1.0f, 1.2f, 1.0f);
}

void scene::Wall::draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders) {
    AObject::draw(models, shaders);
}

void scene::Wall::setShape(glm::vec3 shape) {
    shape.x = -shape.x;
    shape.z = -shape.z;
    _shape = shape;
    _offset = (_size * _shape) / glm::vec3(2);
}
