//
// Created by Timothée Couble on 17/10/2019.
//

#include "Scene.hpp"

scene::Scene::Scene() : App(1920, 1080, "Scene") {
    init();
}

scene::Scene::~Scene() {
    for (auto &it : _models)
        delete it.second;
    for (auto &it : _objects)
        delete it;
}

void scene::Scene::init() {
    std::string vsModelPath = "../shader/model_vs.glsl";
    std::string fsModelPath = "../shader/model_fs.glsl";
    std::string vsLightPath = "../shader/light_vs.glsl";
    std::string fsLightPath = "../shader/light_fs.glsl";
    std::string vsTexturePath = "../shader/texture_vs.glsl";
    std::string fsTexturePath = "../shader/texture_fs.glsl";

    _shaders.push_back(std::make_unique<gl_wrapper::Shader>(
            gl_wrapper::Shader(vsModelPath, fsModelPath, gl_wrapper::ShaderType::MODEL)
    ));
    _shaders.push_back(std::make_unique<gl_wrapper::Shader>(
            gl_wrapper::Shader(vsLightPath, fsLightPath, gl_wrapper::ShaderType::LIGHT)
    ));
    _shaders.push_back(std::make_unique<gl_wrapper::Shader>(
            gl_wrapper::Shader(vsTexturePath, fsTexturePath, gl_wrapper::ShaderType::TEXTURE_DIFFUSE)
    ));

    std::string path = "../resource/mushroom.obj";
    _models.emplace(ModelType::MUSHROOM, new Model(path));

    _objects.push_back(new AObject(ModelType::MUSHROOM));

    for (auto &it : _objects)
        it->init();

    _dirLight.setAmbient(glm::vec3(0.3f, 0.3f, 0.1f));
    _dirLight.setShader(_shaders);

    _pointLights.emplace_back(scene::PointLight(glm::vec3(9.3f, 4.3f, 0.0f), 10));
    _pointLights.emplace_back(scene::PointLight(glm::vec3(1.2f, 3.5f, 0.0f), 25));
    _pointLights[0].setAmbient(glm::vec3(0.96f, 0.85f, 0.05f));
    _pointLights[1].setAmbient(glm::vec3(0.82f, 0.12f, 0.05f));
    _pointLights[1].setDiffuse(glm::vec3(0.82f, 0.12f, 0.05f));
    for (auto &it : _pointLights)
        it.setShader(_shaders);
}

void scene::Scene::onDraw() {
    glPolygonMode(GL_FRONT_AND_BACK, _mode);

    this->checkKey();

    auto aTime = glfwGetTime();

    for (auto &shader : _shaders) {
        shader->bind();
        shader->setUniformMatrix4("view_matrix", _camera->getViewMatrix());
        shader->setUniformMatrix4("proj_matrix", _camera->getProjectionMatrix(getWindow()));
        if (shader->getType() != gl_wrapper::ShaderType::LIGHT)
            shader->setUniformVector3("viewPos", _camera->getCameraPosition());
        gl_wrapper::Shader::unBind();
    }

    for (auto &object : _objects)
        object->draw(_models, _shaders, _objects);
}

void scene::Scene::checkKey() {
    for (const auto &it : _keyMap) {
        if (_keyCode[it.first] && _pressed)
            (_camera.get()->*it.second)();
    }
    if (_keyCode[GLFW_KEY_ESCAPE])
        getWindow().setClose(true);
    if (_keyCode[GLFW_KEY_SPACE] && _pressed) {
        if (_mode == GL_FILL)
            _mode = GL_LINE;
        else
            _mode = GL_FILL;
        _pressed = false;
    }
}

void scene::Scene::onMouseScroll(double x, double y) {
    _camera->zoom(y);
}
