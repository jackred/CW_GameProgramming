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

        void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders,
                  Camera_ptr_t &camera, const MazeDisplay &maze);

        void goUp();
        void goRight();
        void goDown();
        void goLeft();
        void unLockCamera();

    private:
        Ball _ball;
        glm::vec3 _speed;
        double _velocity = 0.1f;
        const float _maxSpeed = 2.5f;
        bool _lock = true;

        double _lastTime = 0;
    };

}

#endif /* !PLAYER_HPP */
