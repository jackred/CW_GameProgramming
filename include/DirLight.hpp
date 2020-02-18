//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef DIR_LIGHT_HPP
# define DIR_LIGHT_HPP

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ALight.hpp"

namespace scene {

    class DirLight : public ALight {
    public:
        explicit DirLight(const glm::vec3 &position, const glm::vec3 &lookAt);

        void setShader(const gl_wrapper::Shaders_t &shaders) const;
        void setDepthShader(const gl_wrapper::Shader_ptr_t &depth) const;

    private:
        glm::mat4 getLightSpaceMatrix() const;

    private:
        const float _nearPlane = 1.0f;
        const float _farPlane = 30.0f;
        glm::vec3 _position;
        glm::vec3 _lookAt;
    };
}

#endif /* !DIR_LIGHT_HPP */
