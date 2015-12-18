#pragma once
#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "maths_funcs.h"
#include <conio.h>
#include "Camera.hpp"
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
// I found the stbi image header from the link Anton Referenced in the
// discussion board.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/

//OBJS
#define PLANE_MESH_OBJ		"../textures/Models/plane_2.obj"
#define BALL_MESH_OBJ		  "../textures/Models/ball.obj"

// TEXTURES
#define SAND_PATH			   "../textures/images/sand.jpg"
#define BRICK_PATH			 "../textures/images/pokeball.png"
#define POKEBALL_PATH		 "../textures/images/snow.jpg"
#define BOULDER_PATH		 "../textures/images/boulder.jpg"


//Font
#define FONT				 "../Font/font.bmp"

//Levels
#define LEVEL_1			"../levels/Level_2.png"
#define LEVEL_2			"../levels/Level_1.png"
#define LEVEL_3			"../levels/Level_3.png"
#define LEVEL_4			"../levels/Level_4.png"
#define LEVEL_5			"../levels/Level_5.png"
#define TOMS_LEVEL	"../levels/TomsLevel.png"

// Greyscale colour values through green channel.
#define BLUE				28
#define BLACK				0
#define RED				  76
#define WHITE				255

// MUSIC
// music was found on new grounds:
// http://www.newgrounds.com/audio/listen/657490
#define MUSIC				"..\Music\music.mp3"
// MISC
#define COLOR_R			 1
#define COLOR_G			 2
#define COLOR_B			 3

// Button interaction
#define ESC_BUTTON			 27
#define RERDAW				   'r'
#define NUM_OF_MODELS		 3
#define VAO_SIZE			   10
#define GROUND				   1
#define MODELS				   2
#define WALLS				     5
#define PLAYER				   4
#define SKYBOX				   6
#define VELOCITY			   0.5

/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/
std::vector<float> g_vp, g_vn, g_vt;
int g_point_count = 0;
float x_val = -0.35, y_val = 22.12, z_val = 28.80;
float pos_x = 0.0f, pos_y = 5.0f, pos_z = 0.0f;

glm::vec3 PLAYERSTARTPOINT = glm::vec3(-15.0f, 20.0f, 3.0f);
glm::vec3 OVERHEADVIEW = glm::vec3(10.0f, 0.0f, 10.0f);
vec3 POS = vec3(pos_x, pos_y, pos_z);
glm::mat4 view = glm::lookAt(PLAYERSTARTPOINT, OVERHEADVIEW, glm::vec3(0.0, 1.0, 0.0));

vec3  move = vec3(0.005f, 0.0f, 0.0f);
vec3  move2 = vec3(0.005f, 0.0f, 0.0f);

vec3  MOVE_LEFT = vec3(1.0f, 0.0f, 0.0f);
vec3  MOVE_RIGHT = vec3(-1.0f, 0.0f, 0.0f);
vec3  MOVE_UP = vec3(0.0f, 0.0f, 1.0f);
vec3  MOVE_DOWN = vec3(0.0f, 0.0f, -1.0f);


int currentLevel = 0;
// Skybox things
GLuint skyID, skyboxTexture;
GLuint skyboxVAO = 9, skyboxVBO;
std::vector<std::string> faces;

float fov = 45.0f;
unsigned int vao = 0, vn_vbo = 0;
unsigned int vp_vbo = 0, vt_vbo = 0;

// Texture stuff
// Handles to our textures
GLuint textures[10];
GLuint texturesCube[10];
GLuint textureID;

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *) NULL + (i))

GLuint shaderProgramID, skyBoxProgramID;

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
int ballVerticesSize = 0, walls = 0;

std::vector<mat4> boulders;
std::vector<mat4> grounds;
std::vector<vec3> modelsPos, bouldersPos;

int numOfModels = NUM_OF_MODELS;
int hasWon;
bool gameEnd = false;
bool gameStart = false;

Camera camera(PLAYERSTARTPOINT); // sets the camera's position
GLfloat lastX = 400, lastY = 400; // bounds for mouse movement

GLfloat skyboxVertices[] = {
	// Positions          
	-45.0f,  45.0f, -45.0f,
	-45.0f, -45.0f, -45.0f,
	45.0f, -45.0f, -45.0f,
	45.0f, -45.0f, -45.0f,
	45.0f,  45.0f, -45.0f,
	-45.0f,  45.0f, -45.0f,

	-45.0f, -45.0f,  45.0f,
	-45.0f, -45.0f, -45.0f,
	-45.0f,  45.0f, -45.0f,
	-45.0f,  45.0f, -45.0f,
	-45.0f,  45.0f,  45.0f,
	-45.0f, -45.0f,  45.0f,

	45.0f, -45.0f, -45.0f,
	45.0f, -45.0f,  45.0f,
	45.0f,  45.0f,  45.0f,
	45.0f,  45.0f,  45.0f,
	45.0f,  45.0f, -45.0f,
	45.0f, -45.0f, -45.0f,

	-45.0f, -45.0f,  45.0f,
	-45.0f,  45.0f,  45.0f,
	45.0f,  45.0f,  45.0f,
	45.0f,  45.0f,  45.0f,
	45.0f, -45.0f,  45.0f,
	-45.0f, -45.0f,  45.0f,

	-45.0f,  45.0f, -45.0f,
	45.0f,  45.0f, -45.0f,
	45.0f,  45.0f,  45.0f,
	45.0f,  45.0f,  45.0f,
	-45.0f,  45.0f,  45.0f,
	-45.0f,  45.0f, -45.0f,

	-45.0f, -45.0f, -45.0f,
	-45.0f, -45.0f,  45.0f,
	45.0f, -45.0f, -45.0f,
	45.0f, -45.0f, -45.0f,
	-45.0f, -45.0f,  45.0f,
	45.0f, -45.0f,  45.0f
};

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

GLuint loadCubemap(std::vector<std::string> faces);