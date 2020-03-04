//
// Created by Timothée Couble on 02/11/2019.
//

#include "AObject.hpp"

unsigned int scene::AObject::_idGen = 0;

scene::AObject::AObject(scene::ModelType type) : _type(type), _id(_idGen++) {}

scene::AObject::AObject(ModelType type, unsigned int id) : _type(type), _id(id) {}

scene::AObject::AObject(const scene::AObject &other) : _type(other._type), _id(_idGen++),
    _orientation(other._orientation), _shape(other._shape), _size(other._size) {}

void scene::AObject::draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders) {
    if (_type == ModelType::UNKNOWN)
        return;
    models.at(_type)->setPosition(_position);
    models.at(_type)->setOrientation(_orientation);
    models.at(_type)->setSize(_size * _shape);
    models.at(_type)->setOffset(_offset);
    models.at(_type)->draw(shaders);
}

void scene::AObject::setPosition(glm::vec3 position) {
    _position = position;
}

void scene::AObject::setOrientation(glm::vec3 orientation) {
    _orientation = orientation;
}

void scene::AObject::setShape(glm::vec3 shape) {
    _shape = shape;
    _offset = (_size * _shape) / glm::vec3(2);
}

void scene::AObject::setSize(glm::vec3 size) {
    _size = size;
}

unsigned int scene::AObject::getId() const {
    return _id;
}

scene::ModelType scene::AObject::getType() const {
    return _type;
}

const glm::vec3 scene::AObject::getPosition() const {
    return _position - _offset;
}

const glm::vec3 scene::AObject::getSize() const {
    return _size * _shape;
}
