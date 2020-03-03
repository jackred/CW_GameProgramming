//
// Created by Timothée Couble on 02/03/2020.
//

#include "Instance.hpp"

gl_wrapper::Instance::Instance(size_t max_size) {
    std::string objPath = "../resource/cube.obj";
    auto objModel = loader::OBJLoader(objPath).load();

    for (unsigned int i = 0; i < objModel.size(); i++)
        _meshList.emplace_back(gl_wrapper::Mesh(objModel.getVertices(i), objModel.getIndices(i)));

    _modelMatrices.reserve(max_size);

    glGenBuffers(1, &_vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayID);

    for (auto &mesh : _meshList) {

        glBindVertexArray(mesh.getVaoID());

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ModelList),
                              (void *) offsetof(ModelList, color));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ModelList),
                              (void *) offsetof(ModelList, matrix));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(ModelList),
                              (void *) (offsetof(ModelList, matrix) + (sizeof(glm::vec4))));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(ModelList),
                              (void *) (offsetof(ModelList, matrix) + (2 * sizeof(glm::vec4))));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(ModelList),
                              (void *) (offsetof(ModelList, matrix) + (3 * sizeof(glm::vec4))));

        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

}

gl_wrapper::Instance::~Instance() {
    for (auto &it : _meshList)
        it.clearBuffers();
}

void gl_wrapper::Instance::draw(const Shader_ptr_t &shader) {
    shader->bind();

    for (auto &mesh : _meshList) {
        glBindVertexArray(mesh.getVaoID());
        glDrawElementsInstanced(GL_TRIANGLES, mesh.getIndicesSize(), GL_UNSIGNED_INT, nullptr, _size);
        glBindVertexArray(0);
    }

    gl_wrapper::Shader::unBind();
}

void gl_wrapper::Instance::updateModel(size_t size) {
    _size = size;
    glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayID);
    glBufferData(GL_ARRAY_BUFFER, _size * sizeof(ModelList), _modelMatrices.data(), GL_STATIC_DRAW);
}

gl_wrapper::ModelList &gl_wrapper::Instance::operator[](size_t i) {
    return _modelMatrices[i];
}

