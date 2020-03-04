//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef CROWD_HPP
# define CROWD_HPP

#include <iostream>

#include "glm/glm.hpp"

#include "Ball.hpp"
#include "Camera.hpp"
#include "Lib.hpp"
#include "Intersect.hpp"
#include "MazeDisplay.hpp"

namespace scene {
  class Crowd {
  public:
    Crowd();

    void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders);
    void reset(const MazeDisplay &maze);
    void update(const MazeDisplay &maze, const glm::vec3 &player, const std::vector<Crowd *> &crowds);

    void setPlayerSpeed(const glm::vec3 &speed);
    void inverseSpeed(const unsigned int &otherId);

    void doJump();

    const glm::vec3 getPosition() const;
    const unsigned int &getId() const;

  private:
    void collideWithPlayer(const glm::vec3 &player, double &delta);
    void collideWithCrowd(const std::vector<Crowd *> &crowds, double &delta);
    void collideWithWalls(const MazeDisplay &maze, double &delta);
    void collideWithFloor(const MazeDisplay &maze, double &delta);
    glm::vec3 doCollision(normal_collision_t &collision, glm::vec3 &pos, glm::vec3 &newPos);
    glm::vec3 &scaleMax(glm::vec3 &velocity);
    glm::vec3 boids(const glm::vec3 &player, const std::vector<Crowd *> &crowds);

    
  private:
    const unsigned int _id;
    static unsigned int _idGen;

    Ball _ball;
    glm::vec3 _speed;
    const float _maxSpeed = 1.5f;
    const float _maxSpace = 0.55f;
    bool _collidePlayer = false;
    std::vector<bool> _collideCrowd;

    double _lastTime = glfwGetTime();
  };


}

#endif /* !CROWD_HPP */
