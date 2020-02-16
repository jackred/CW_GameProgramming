//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef WALL_HPP
# define WALL_HPP

#include <iostream>

#include "AObject.hpp"

namespace scene {

    class Wall : public AObject {
    public:
        explicit Wall();

        void draw(const scene::Models_t &models, const gl_wrapper::Shaders_t &shaders) override;

        void setShape(glm::vec3 shape) override;
    };
}

#endif /* !WALL_HPP */
