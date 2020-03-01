//
// Created by Timothée Couble on 28/02/2020.
//

#include "Particles.hpp"

gl_wrapper::Particles::Particles() {
    std::string objPath = "../resource/cube.obj";
    auto objModel = loader::OBJLoader(objPath).load();

    for (unsigned int i = 0; i < objModel.size(); i++)
        _meshList.emplace_back(gl_wrapper::Mesh(objModel.getVertices(i), objModel.getIndices(i)));

    _modelMatrices.reserve(MAX_PARTICLES);
    //_particlesContainer.reserve(_maxParticles);

    glGenBuffers(1, &_vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayID);
    glBufferData(GL_ARRAY_BUFFER, _actualAmount * sizeof(ModelList), _modelMatrices.data(), GL_STATIC_DRAW);

    for (auto &mesh : _meshList) {

        glBindVertexArray(mesh.getVaoID());
        // set attribute pointers for matrix (4 times vec4)

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

gl_wrapper::Particles::~Particles() {
    for (auto &it : _meshList)
        it.clearBuffers();
}

void gl_wrapper::Particles::draw(const Shader_ptr_t &shader) {
    this->refreshParticles();

    shader->bind();

    for (auto &mesh : _meshList) {
        glBindVertexArray(mesh.getVaoID());
        glDrawElementsInstanced(GL_TRIANGLES, mesh.getIndicesSize(), GL_UNSIGNED_INT, nullptr, _actualAmount);
        glBindVertexArray(0);
    }

    gl_wrapper::Shader::unBind();
}

void gl_wrapper::Particles::refreshParticles() {
    double currentTime = glfwGetTime();
    double delta = currentTime - _lastTime;
    _lastTime = currentTime;

    int nbParticles = (int) (delta * 2000.0);
    if (nbParticles > (int) (0.016f * 2000.0))
        nbParticles = (int) (0.016f * 2000.0);

    for(int i = 0; i < nbParticles; i++){
        int index = this->findUnusedParticle();
        _particlesContainer[index].life = 4.0f;
        _particlesContainer[index].pos = glm::vec3(0.0f, 1.0f, 0.0f);

        float spread = 1.5f;
        glm::vec3 maindir = glm::vec3(0.0f, 8.0f, 0.0f);
        double R = 1.0f;

        double phi = (rand() % (int) (2 * M_PI * 1000)) / 1000.0f;
        double costheta = (rand() % 2000 - 1000) / 1000.0f;
        double u = (rand() % 1000) / 1000.0f;

        double theta = std::acos(costheta);
        double r = R * std::cbrt(u);

        glm::vec3 randDir;
        randDir.x = r * std::sin(theta) * std::cos(phi);
        randDir.y = r * std::sin(theta) * std::sin(phi);
        randDir.z = r * std::cos(theta);

        _particlesContainer[index].speed = maindir + randDir * spread;

        _particlesContainer[index].rgb.r = (rand() % 160 + 50) / 255.0f;
        _particlesContainer[index].rgb.g = (rand() % 160 + 50) / 255.0f;
        _particlesContainer[index].rgb.b = (rand() % 160 + 50) / 255.0f;

        _particlesContainer[index].size = (rand() % 1000) / 2000.0f + 0.1f;
    }

    _actualAmount = 0;
    for(int i = 0; i < MAX_PARTICLES; i++){

        Particle &p = _particlesContainer[i];

        if (p.life > 0.0f) {

            p.life -= (float) delta;
            if (p.life > 0.0f){

                p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float) delta * 0.5f;
                p.pos += p.speed * (float) delta;

                if (p.pos.y < 0.0f)
                    p.pos.y = 0.0f;

                glm::mat4 translate = glm::translate(glm::mat4(1.0f), p.pos);
                glm::mat4 rotate = glm::mat4(1.0f);
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
                scale = glm::scale(scale, glm::vec3(p.size));

                _modelMatrices[_actualAmount].color = p.rgb;
                _modelMatrices[_actualAmount].matrix = translate * rotate * scale;

            }
            _actualAmount++;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayID);
    glBufferData(GL_ARRAY_BUFFER, _actualAmount * sizeof(ModelList), _modelMatrices.data(), GL_STATIC_DRAW);
}

int gl_wrapper::Particles::findUnusedParticle() {
    for (int i = _lastUsedParticle; i < MAX_PARTICLES; i++){
        if (_particlesContainer[i].life < 0){
            _lastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < _lastUsedParticle; i++){
        if (_particlesContainer[i].life < 0){
            _lastUsedParticle = i;
            return i;
        }
    }
    return 0;
}
