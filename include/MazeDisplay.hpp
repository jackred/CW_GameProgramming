//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef MAZE_DISPLAY_HPP
# define MAZE_DISPLAY_HPP

#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "Maze.hpp"
#include "Wall.hpp"
#include "Instance.hpp"

namespace scene {

    class MazeDisplay {
    public:
        MazeDisplay();

        void init();
        void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders);
        void clear();

    private:
        void initFloor(const glm::vec2 &size);

    private:
        backstage::Maze _maze;
        std::vector<Wall*> _walls;
        gl_wrapper::Instance _floor;
    };
}

#endif /* !MAZE_DISPLAY_HPP */
