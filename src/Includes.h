#pragma comment(lib, "winmm.lib")
#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "maths_funcs.h"
#include "text.h"
#include <conio.h>
#include "Camera.cpp"
#include <string>
#include <stdlib.h>
#include <math.h>

// Assimp includes
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector> // STL dynamic memory.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/

//OBJS
#define PLANE_MESH_OBJ		"../Textures/Models/plane_2.obj"
#define PYRAMID_MESH_OBJ		"../Textures/Models/pyramid.obj"
#define CIRCLE_MESH_OBJ		"../Textures/Models/circle.obj"
#define PALM_MESH_OBJ		"../Textures/Models/palm.obj"
#define WALLS_MESH_OBJ		"../Textures/Models/wallls.obj"
#define BALL_MESH_OBJ		"../Textures/Models/ball.obj"

// TEXTURES
#define SAND_PATH			 "../Textures/sand.jpg"
#define PALM_PATH			 "../Textures/palm.jpg"
#define BRICK_PATH			 "../Textures/pokeball.png"
#define POKEBALL_PATH		 "../Textures/snow.jpg"
#define BOULDER_PATH		 "../Textures/boulder.jpg"
#define BOULDER_PATH		 "../Textures/boulder.jpg"
#define SKYBOX_PATH_FRONT	 "../Textures/Yokohama2/posx.jpg"
#define SKYBOX_PATH_BACK		 "../Textures/Yokohama2/posy.jpg"
#define SKYBOX_PATH_TOP		 "../Textures/Yokohama2/posz.jpg"
#define SKYBOX_PATH_BOTTOM	 "../Textures/Yokohama2/negx.jpg"
#define SKYBOX_PATH_LEFT		 "../Textures/Yokohama2/negy.jpg"
#define SKYBOX_PATH_RIGHT	 "../Textures/Yokohama2/negz.jpg"

//Font
#define FONT				 "../Font/font.bmp"

//Levels
#define LEVEL_1			"../Levels/Level_2.png"
#define LEVEL_2			"../Levels/Level_1.png"
#define LEVEL_3			"../Levels/Level_3.png"
#define LEVEL_4			"../Levels/Level_4.png"
#define LEVEL_5			"../Levels/Level_5.png"
#define TOMS_LEVEL			"../Levels/TomsLevel.png"

#define BLUE				28
#define BLACK				0
#define RED				76
#define WHITE				255

// MUSIC
// music was found on new grounds:
// http://www.newgrounds.com/audio/listen/657490
#define MUSIC				"..\Music\music.mp3"
// MISC
#define COLOR_R			 1
#define COLOR_G			 2
#define COLOR_B			 3

#define ESC_BUTTON			 27
#define RERDAW				 'r'
#define NUM_OF_MODELS		 3
#define VAO_SIZE			 10
#define GROUND				 1
#define MODELS				 2
#define WALLS				 5
#define PLAYER				 4
#define SKYBOX				 4
#define BOUNDARY			 23
#define VELOCITY			 0.05

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/
std::vector<float> g_vp, g_vn, g_vt;
int g_point_count = 0;
float x_val = -0.35, y_val = 22.12, z_val = 28.80;
float pos_x = 0.0f, pos_y = 5.0f, pos_z = 0.0f;

vec3 PLAYERSTARTPOINT = vec3(20.0f, 20.0f, 35.0f);
vec3 OVERHEADVIEW = vec3(0.0f, 0.0f, 0.0f);
vec3 POS = vec3(pos_x, pos_y, pos_z);
mat4 view = look_at(PLAYERSTARTPOINT, OVERHEADVIEW, vec3(0.0, 1.0, 0.0));

vec3  move = vec3(0.005f, 0.0f, 0.0f);
vec3  move2 = vec3(0.005f, 0.0f, 0.0f);

vec3  MOVE_LEFT = vec3(1.0f, 0.0f, 0.0f);
vec3  MOVE_RIGHT = vec3(-1.0f, 0.0f, 0.0f);
vec3  MOVE_UP = vec3(0.0f, 0.0f, 1.0f);
vec3  MOVE_DOWN = vec3(0.0f, 0.0f, -1.0f);

bool gameStart = false;

int currentLevel = 0;

GLuint skyID, skyboxTexture;
GLuint skyboxVAO, skyboxVBO;


float fov = 45.0f;
unsigned int vao = 0, vn_vbo = 0;
unsigned int vp_vbo = 0, vt_vbo = 0;

// Texture stuff
// Handles to our textures
GLuint textures[10];
GLuint texturesCube[10];
GLuint skyBox;
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
std::vector<mat4> ballModels;
float rotatey = 0.05;

mat4 playerModel = identity_mat4();

int pyramid = 0, plane = 0;
int palm = 0, walls = 0;

std::vector<mat4> boulders;
std::vector<mat4> grounds;
std::vector<vec3> modelsPos, bouldersPos;


bool ball1IsCaught = false;
bool ball2IsCaught = false;

int numOfModels = NUM_OF_MODELS;
int hasWon;

bool firstMouse = true;
bool gameEnd = false;
GLfloat lastX = 400, lastY = 300;

void moveEverybody(mat4 newPlayerModel, std::vector<mat4> balls);

// handles mouse scrolling this stackoverflow helped:
// http://stackoverflow.com/questions/14378/using-the-mouse-scrollwheel-in-glut
void mouseWheel(int button, int direction, int x, int y);

// Handles moving the mouse, where x and y are the co ordinates of the mouse
void mouseMove(int x, int y);

// keeps a record of each model's position in the system.
void updatePositions(mat4 newPlayerPos, mat4 newBall1, mat4 newBall2);

// Returns the distance between two points in the 3D co ordinate system.
double distanceBetweenPoints(vec3 p1, vec3 p2);

void moveLeft(mat4 newPlayerModel, std::vector<mat4> balls);

void moveRight(int result, mat4 newPlayerModel, mat4 newball1, mat4 newball2);

void moveUp(int result, mat4 newPlayerModel, mat4 newball1, mat4 newball2);

void moveDown(int result, mat4 newPlayerModel, mat4 newball1, mat4 newball2);

// Compares the distance between each object.
int collisionDetection(mat4 newPlayerPos, std::vector<mat4> newball2);
void getPixels(unsigned char * img, int imgWidth, int imgHeight, int n);

void generateMap();

void printMapValues(unsigned char * img, int imgWidth, int imgHeight, int n);

void loadMapFromImage(char * mapPath);

void loadNextLevel(int currentLevel);

void reloadLevel();