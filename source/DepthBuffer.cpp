//
// Created by Timothée Couble on 17/02/2020.
//

#include "DepthBuffer.hpp"

void gl_wrapper::DepthBuffer::init() {
    glGenFramebuffers(1, &_fboID);
    glGenTextures(1, &depthMapTexture);

    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    DepthBuffer::unBind();
}

void gl_wrapper::DepthBuffer::clear() {
    glDeleteFramebuffers(1, &_fboID);
}

void gl_wrapper::DepthBuffer::setViewPort() {
    glViewport(0, 0, _width, _height);
}

void gl_wrapper::DepthBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fboID);
}

void gl_wrapper::DepthBuffer::unBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl_wrapper::DepthBuffer::bindTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
}
