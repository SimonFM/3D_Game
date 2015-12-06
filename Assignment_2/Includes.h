#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "maths_funcs.h"
#include "text.h"
#include <conio.h>


// Assimp includes
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define PLANE_MESH_OBJ "../Textures/plane.obj"
#define PYRAMID_MESH_OBJ "../Textures/pyramid.obj"
#define CIRCLE_MESH_OBJ "../Textures/circle.obj"
#define PALM_MESH_OBJ "../Textures/palm.obj"
#define WALLS_MESH_OBJ "../Textures/wallls.obj"
#define BALL_MESH_OBJ "../Textures/ball.obj"

#define SAND_PATH "../Textures/sand.jpg"
#define PALM_PATH "../Textures/palm.jpg"
#define BRICK_PATH "../Textures/sand_brick.jpg"
#define POKEBALL_PATH "../Textures/pokeball.png"
#define ESC_BUTTON 27
#define NUM_OF_MODELS 3
#define VAO_SIZE	10
#define GROUND	1
#define MODELS 2
#define WALLS 5
#define PLAYER 4
#define BOUNDARY 10

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/
std::vector<float> g_vp, g_vn, g_vt;
int g_point_count = 0;
float x_val = -0.35, y_val = 22.12, z_val = 28.80;
float pos_x = 0.0f, pos_y = 5.0f, pos_z = 0.0f;

vec3 PLAYERSTARTPOINT = vec3(x_val, y_val, z_val);
vec3 POS = vec3(pos_x, pos_y, pos_z);
mat4 view = look_at(PLAYERSTARTPOINT, POS, vec3(0.0, 1.0, 0.0));
//vec3 PLAYERSTARTPOINT = vec3(-5.0f, 0.0f, 5.0f);
//
//vec3 CAM = PLAYERSTARTPOINT;
//vec3 LOOKAT = vec3(pos_x, pos_y, pos_z);
//mat4 view = look_at(CAM, LOOKAT, vec3(0.0, 1.0, 0.0));

vec3  move = vec3(0.005f, 0.0f, 0.0f);
vec3  move2 = vec3(0.005f, 0.0f, 0.0f);
float pyramidX = 0.05f;
vec3 pyramind3Movement = vec3(0.0, pyramidX, 0.0);

bool anyKeyPressed = true;


float fov = 45.0f;
unsigned int vao = 0, vn_vbo = 0;
unsigned int vp_vbo = 0, vt_vbo = 0;

// Texture stuff
// Handles to our textures
GLuint textures[5];

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *) NULL + (i))

GLuint shaderProgramID;

unsigned int mesh_vao = 0;
int width = 1000, height = 720;

GLuint loc1, loc2, loc3;
GLuint uv;
GLfloat rotate_y = 0.1f;

// multiple VAO 
int vaoArray[VAO_SIZE];
std::vector<mat4> models;
mat4 playerModel = identity_mat4(), wallsMat = identity_mat4();
mat4 ground = identity_mat4();
mat4 ball = identity_mat4();
mat4 ball1 = identity_mat4();
mat4 ball2 = identity_mat4();

int pyramid = 0, plane = 0;
int palm = 0, walls = 0;

std::vector<vec3> modelsPos;

bool ball1IsCaught = false;
bool ball2IsCaught = false;

int numOfModels = NUM_OF_MODELS;
int hasWon = 2;
