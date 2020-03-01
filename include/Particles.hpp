//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef PARTICLES_HPP
# define PARTICLES_HPP

#include <iostream>
#include <vector>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Lib.hpp"
#include "Mesh.hpp"
#include "Misc.hpp"
#include "OBJLoader.hpp"

namespace gl_wrapper {

    struct Particle {
        glm::vec3 pos;
        glm::vec3 speed;
        glm::vec3 rgb;
        float size;
        float life = -1.0f;
    };

    struct ModelList {
        glm::vec3 color;
        glm::mat4 matrix;
    };

    const size_t MAX_PARTICLES = 10000;

    class Particles {
    public:
        explicit Particles();
        ~Particles();

        void draw(const Shader_ptr_t &shader);

    private:
        void refreshParticles();
        int findUnusedParticle();

    private:
        std::vector<Mesh> _meshList;

        unsigned int _vertexArrayID;
        std::vector<ModelList> _modelMatrices;

        size_t _actualAmount = 0;

        int _lastUsedParticle = 0;
        Particle _particlesContainer[MAX_PARTICLES];

        double _lastTime = 0;
    };
}

#endif /* !PARTICLES_HPP */
