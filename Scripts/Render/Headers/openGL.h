#pragma once
#include "../../General/Headers/structs.h"

void PrintGLError(int line);

void InitOpenGL(App *app);

void PrintOpenGLInfo();

void InitRenderPipeline(App *app);

Renderer *CreateRenderer();

GLuint CreateShaderProgram(char *vertexShaderName, char *fragmentShaderName);

void SetProjectionMatrix(Renderer *renderer, int windowWidth, int windowHeight);

void CalculateProjectionMatrix(float projection[16], int windowWidth, int windowHeight);

void CreateWindowFBO(App *app, Window *window);
