//
// Created by Timothée Couble on 19/10/2019.
//

#ifndef DEPTH_BUFFER_HPP
# define DEPTH_BUFFER_HPP

#include <iostream>

#include "Lib.hpp"

namespace gl_wrapper {

    class DepthBuffer {
    public:
        DepthBuffer() = default;

        void init();
        void clear();

        void setViewPort();
        void bind();
        static void unBind();

        void bindTexture();

    private:
        const unsigned int _width = 1024;
        const unsigned int _height = 1024;
        unsigned int _fboID = 0;
        unsigned int depthMapTexture = 0;
    };
}

#endif /* !DEPTH_BUFFER_HPP */
