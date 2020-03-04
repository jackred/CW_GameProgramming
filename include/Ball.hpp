//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef BALL_HPP
# define BALL_HPP

#include <iostream>

#include "AObject.hpp"

namespace scene {

    class Ball : public AObject {
    public:
        explicit Ball();

        void setPosition(glm::vec3 position) override;
    };
}

#endif /* !BALL_HPP */
