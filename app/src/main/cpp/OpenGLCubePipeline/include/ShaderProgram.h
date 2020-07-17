//This file was modified from a FOSS MIT-licenced game engine, NovelRT. Repo can be found at: https://github.com/novelrt/NovelRT

#ifndef NOVELRT_GRAPHICS_SHADERPROGRAM_H
#define NOVELRT_GRAPHICS_SHADERPROGRAM_H

#include<GLES3/gl3.h>
#include<GLES3/gl3ext.h>

struct ShaderProgram {
public:
    GLuint shaderProgramId = 0;
    GLuint finalViewMatrixBufferUboId = 0;
    GLuint lightPosDataUboId = 0;


    ShaderProgram() {}
};

#endif