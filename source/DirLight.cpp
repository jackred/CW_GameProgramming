//
// Created by Timothée Couble on 19/10/2019.
//

#include "DirLight.hpp"

scene::DirLight::DirLight(const glm::vec3 &position, const glm::vec3 &lookAt)
    : _position(position), _lookAt(lookAt), ALight("dirLight") {}

glm::mat4 scene::DirLight::getLightSpaceMatrix() const {
    glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, _nearPlane, _farPlane);
    glm::mat4 lightView = glm::lookAt(_position, _position + _lookAt, glm::vec3(0.0, 1.0, 0.0));
    return lightProjection * lightView;
}

void scene::DirLight::setShader(const gl_wrapper::Shaders_t &shaders) const {
    for (auto &shader : shaders) {
        if (shader->getType() == gl_wrapper::ShaderType::MODEL) {
            shader->bind();
            ALight::setShader(shader);
            shader->setUniformVector3(getUniformName("position").c_str(), _position);
            shader->setUniformMatrix4("light_space", this->getLightSpaceMatrix());
            gl_wrapper::Shader::unBind();
        }
    }
}

void scene::DirLight::setDepthShader(const gl_wrapper::Shader_ptr_t &depth) const {
    depth->bind();
    depth->setUniformMatrix4("light_space", this->getLightSpaceMatrix());
    gl_wrapper::Shader::unBind();
}
