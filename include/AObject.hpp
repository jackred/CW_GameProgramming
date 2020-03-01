//
// Created by Timothée Couble on 02/11/2019.
//

#ifndef AOBJECT_HPP
# define AOBJECT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "Misc.hpp"
#include "Model.hpp"
#include "Shader.hpp"

namespace scene {

    class AObject {
    public:
        explicit AObject(ModelType type);
        AObject(ModelType type, unsigned int id);
        AObject(const AObject& other);
		virtual ~AObject() = default;

		void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders);
        void checkDepth(const scene::Models_t &models, const gl_wrapper::Shader_ptr_t &depth);

        virtual void setPosition(glm::vec3 position);
        void setOrientation(glm::vec3 orientation);
        virtual void setShape(glm::vec3 shape);

        unsigned int getId() const;
        ModelType getType() const;
        glm::vec3 getPosition() const;

    protected:
        const unsigned int _id;
        glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 _orientation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 _offset = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 _shape = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 _size = glm::vec3(1.0f, 1.0f, 1.0f);

    private:
        static unsigned int _idGen;
        const ModelType _type = scene::ModelType::UNKNOWN;
    };

    typedef std::vector<AObject*> Objects_t;

}

#endif /* !AOBJECT_HPP */
