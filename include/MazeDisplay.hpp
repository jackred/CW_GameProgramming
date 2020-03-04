//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef MAZE_DISPLAY_HPP
# define MAZE_DISPLAY_HPP

#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "Maze.hpp"
#include "Intersect.hpp"
#include "Wall.hpp"
#include "Instance.hpp"

namespace scene {

    class MazeDisplay {
    public:
        MazeDisplay();

        void init();
        void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders);
        void reset();
        bool update(const glm::vec3 playerPosition);
        void clear();

        bool intersectSquare(glm::vec3 center, float size) const;
        normal_collision_t intersectSphereWalls(glm::vec3 center, float radius) const;
        normal_collision_t intersectSphereFloor(glm::vec3 center, float radius) const;

        const glm::vec2 getStart() const;
        const glm::vec2 getEnd() const;

    private:
        void initFloor(const glm::vec2 &size);
        void changeFloorColor();

    private:
        backstage::Maze _maze;
        std::vector<Wall*> _walls;
        gl_wrapper::Instance _floor;

        const glm::vec3 _defaultColor = glm::vec3(0.39f, 0.17f, 0.03f);
        const glm::vec3 _pathColor = glm::vec3(0.70f, 0.59f, 0.29f);
        const glm::vec3 _endColor = glm::vec3(0.70f, 0.70f, 0.50f);
    };
}

#endif /* !MAZE_DISPLAY_HPP */
