//
// Created by Timothée Couble on 17/10/2019.
//

#ifndef SCENE_HPP
# define SCENE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <set>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "App.hpp"
#include "AObject.hpp"
#include "Camera.hpp"
#include "Crowd.hpp"
#include "MazeDisplay.hpp"
#include "DirLight.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Misc.hpp"
#include "Particles.hpp"
#include "Player.hpp"
#include "PointLight.hpp"
#include "Shader.hpp"

namespace scene {

    class Scene : public gl_wrapper::App {

        typedef void (scene::Camera::*change_camera_t)();
        typedef void (scene::Player::*move_player_t)();

    public:
        Scene();
        ~Scene();

        void init();

        void onDraw() override;

        void onMouseScroll(double x, double y) override;

    private:
        void checkKey() override;
        void reset();

    private:
        GLenum _mode = GL_FILL;
        scene::Models_t _models;
        std::vector<Crowd *> _crowd;
        gl_wrapper::Shaders_t _shaders;
        Particles _particles;
        MazeDisplay _maze;
        std::vector<scene::PointLight> _pointLights;
        scene::DirLight _dirLight = scene::DirLight(glm::vec3(-10.0f, 10.0f, -10.0f), glm::vec3(2.0f, -4.0f, 2.0f));
        Camera_ptr_t _camera = std::make_unique<Camera>(Camera());
        Player _player;

        const std::unordered_map<int, change_camera_t> _keyMapCamera = {
                {GLFW_KEY_W, &scene::Camera::moveUp},
                {GLFW_KEY_S, &scene::Camera::moveDown},
                {GLFW_KEY_R, &scene::Camera::moveForward},
                {GLFW_KEY_F, &scene::Camera::moveBackward},
                {GLFW_KEY_A, &scene::Camera::moveLeft},
                {GLFW_KEY_D, &scene::Camera::moveRight},
                {GLFW_KEY_Q, &scene::Camera::rotateLeft},
                {GLFW_KEY_E, &scene::Camera::rotateRight}
        };
        const std::unordered_map<int, move_player_t> _keyMapPlayer = {
                {GLFW_KEY_UP, &scene::Player::goUp},
                {GLFW_KEY_LEFT, &scene::Player::goLeft},
                {GLFW_KEY_RIGHT, &scene::Player::goRight},
                {GLFW_KEY_DOWN, &scene::Player::goDown},
                {GLFW_KEY_SPACE, &scene::Player::doJump},
                {GLFW_KEY_LEFT_SHIFT, &scene::Player::unLockCamera}
        };
    };
}
#endif /* !SCENE_HPP */
