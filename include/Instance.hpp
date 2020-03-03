//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef INSTANCE_HPP
# define INSTANCE_HPP

#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "Lib.hpp"
#include "Mesh.hpp"
#include "Misc.hpp"
#include "OBJLoader.hpp"

namespace gl_wrapper {
    struct ModelList {
        glm::vec3 color;
        glm::mat4 matrix;
    };

    class Instance {
    public:
        explicit Instance(size_t max_size);
        ~Instance();

        void draw(const Shader_ptr_t &shader);
        void updateModel(size_t size);

        ModelList &operator[](size_t i);

    private:
        std::vector<Mesh> _meshList;

        unsigned int _vertexArrayID;
        std::vector<ModelList> _modelMatrices;

        size_t _size = 0;
    };
}

#endif /* !INSTANCE_HPP */
