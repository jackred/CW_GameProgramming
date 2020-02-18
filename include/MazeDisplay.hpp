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

namespace scene {

    class MazeDisplay {
    public:
        MazeDisplay() = default;

        void init();
        void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders);
        void checkDepth(const scene::Models_t &models, const gl_wrapper::Shader_ptr_t &depth);
        void clear();

    private:
        backstage::walls_t _test;
        std::vector<Wall*> _walls;
        Wall *_floor;
    };
}

#endif /* !MAZE_DISPLAY_HPP */
