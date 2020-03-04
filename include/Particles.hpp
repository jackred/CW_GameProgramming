//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef PARTICLES_HPP
# define PARTICLES_HPP

#include <iostream>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Lib.hpp"
#include "Shader.hpp"
#include "Instance.hpp"
#include "MazeDisplay.hpp"

namespace scene {

    struct Particle {
        glm::vec3 pos;
        glm::vec3 speed;
        glm::vec3 rgb;
        float size;
        float life = -1.0f;
    };

    const size_t MAX_PARTICLES = 1500;

    class Particles {
    public:
        explicit Particles();

        void draw(const gl_wrapper::Shader_ptr_t &shader, const MazeDisplay &maze);
        void setPosition(glm::vec3 pos);

    private:
        void refreshParticles(const MazeDisplay &maze);
        int findUnusedParticle();

    private:
        gl_wrapper::Instance _instances;
        glm::vec3 _position = glm::vec3(0.0f);

        int _lastUsedParticle = 0;
        Particle _particlesContainer[MAX_PARTICLES];

        double _lastTime = 0;
    };
}

#endif /* !PARTICLES_HPP */
