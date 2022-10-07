/*
 *  CSCI 441, Computer Graphics, Fall 2022
 *
 *  Project: lab04
 *  File: main.cpp
 *
 *  Description:
 *      This file contains the basic setup to work with GLSL shaders.
 *
 *  Author: Dr. Paone, Colorado School of Mines, 2022
 *
 */

#include "A3Engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

///*****************************************************************************
//
// Our main function
int main() {

    auto a3Engine = new A3Engine();
    a3Engine->initialize();
    if (a3Engine->getError() == CSCI441::OpenGLEngine::OPENGL_ENGINE_ERROR_NO_ERROR) {
        a3Engine->run();
    }
    a3Engine->shutdown();
    delete a3Engine;

	return EXIT_SUCCESS;
}
