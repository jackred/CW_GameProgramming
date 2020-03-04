//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef PLAYER_HPP
# define PLAYER_HPP

#include <iostream>

#include "glm/glm.hpp"

#include "Ball.hpp"
#include "Camera.hpp"
#include "Lib.hpp"
#include "Intersect.hpp"
#include "MazeDisplay.hpp"

namespace scene {

    class Player {
    public:
        Player(const MazeDisplay &maze);

        void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders);
        void reset(const MazeDisplay &maze);
        void update(Camera_ptr_t &camera, const MazeDisplay &maze);

        void goUp();
        void goRight();
        void goDown();
        void goLeft();
        void doJump();
        void unLockCamera();

        const glm::vec3 getPosition() const;

    private:
        glm::vec3 doCollision(normal_collision_t &collision, glm::vec3 &pos, glm::vec3 &newPos);

    private:
        Ball _ball;
        glm::vec3 _speed;
        double _velocity = 0.1f;
        const float _maxSpeed = 2.5f;
        bool _lock = true;

        double _lastTime = glfwGetTime();
    };

}

#endif /* !PLAYER_HPP */
