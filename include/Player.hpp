//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef PLAYER_HPP
# define PLAYER_HPP

#include <iostream>

#include "glm/glm.hpp"

#include "AObject.hpp"
#include "Camera.hpp"
#include "Lib.hpp"
#include "Intersect.hpp"
#include "MazeDisplay.hpp"
#include "Crowd.hpp"

namespace scene {

    class Player {
    public:
        Player();

        void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders);
        void reset(const MazeDisplay &maze);
        void update(Camera_ptr_t &camera, const MazeDisplay &maze, std::vector<Crowd *> &crowd);

        void goUp();
        void goRight();
        void goDown();
        void goLeft();
        void doJump();
        void unLockCamera();

        const glm::vec3 getPosition() const;

    private:
        void collideWithCrowd(std::vector<Crowd *> &crowd, double &delta);
        void collideWithWalls(const MazeDisplay &maze, double &delta);
        void collideWithFloor(const MazeDisplay &maze, double &delta);
        glm::vec3 doCollision(normal_collision_t &collision, glm::vec3 &pos, glm::vec3 &newPos);
        glm::vec3 &scaleMax(glm::vec3 &velocity);

    private:
        AObject _ball;
        glm::vec3 _speed;
        const float _size = 0.2f;
        const double _velocity = 0.1f;
        const float _maxSpeed = 1.5f;
        bool _lock = true;

        double _lastTime = glfwGetTime();
    };

}

#endif /* !PLAYER_HPP */
