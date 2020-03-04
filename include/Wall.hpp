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

        void setPosition(glm::vec3 position) override;
        void setShape(glm::vec3 shape) override;

        const glm::vec3 getMin() const;
        const glm::vec3 getMax() const;
    };
}

#endif /* !WALL_HPP */
