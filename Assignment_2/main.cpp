
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "maths_funcs.h"
#include "text.h"
#include <conio.h>
#include <GLFW/glfw3.h>


// Assimp includes
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma warning(disable:4996)


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
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

std::vector<float> g_vp, g_vn, g_vt;
int g_point_count = 0;
float x_val = -10.0, y_val = 5.0, z_val = 5.0;
float pos_x = 0.0f, pos_y = 5.0f, pos_z = 0.0f;
vec3 CAM = vec3(x_val, y_val, z_val);
vec3 POS = vec3(pos_x, pos_y, pos_z);
mat4 view = look_at(CAM,POS,vec3(0.0,1.0,0.0));
vec3  move = vec3(0.005f, 0.0f, 0.0f);
vec3  move2 = vec3(0.005f, 0.0f, 0.0f);


float pyramidX = 0.05f;
vec3 pyramind3Movement  = vec3(0.0, pyramidX, 0.0);

bool anyKeyPressed = true;
#define ESC_BUTTON 27
#define NUM_OF_MODELS 10

float fov = 45.0f;
unsigned int vao = 0;
unsigned int vn_vbo = 0;
unsigned int vp_vbo = 0;
unsigned int vt_vbo = 0;

// Texture stuff
// Handles to our textures
GLuint textures[5];

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLuint shaderProgramID;

unsigned int mesh_vao = 0;
int width = 500;
int height = 500;

GLuint loc1, loc2, loc3;
GLuint uv;
GLfloat rotate_y = 0.1f;

// multiple VAO 
int vaoArray[6];
std::vector<mat4> models;
mat4 playerModel = identity_mat4();
mat4 wallsMat = identity_mat4();

int pyramid = 0, plane = 0;
int palm = 0, walls = 0;

#pragma region GAME MECHANICS
void initialiseModels() {
	for (int i = 0; i < NUM_OF_MODELS; i++)	models.push_back(identity_mat4());
	//ground
	models[0] = scale(models[0], vec3(2.0,2.0,2.0));
	// middle
	models[1] = translate(models[1], vec3(0.0f, 0.0f, 0.0f));
	// left
	models[2] = translate(models[2], vec3(-5.0f, 0.0f, -5.0f));
	// right
	models[3] = translate(models[3], vec3(-5.0f, 0.0f, 5.0f));
	// other
	playerModel = translate(playerModel, vec3(-5.0f, 0.0f, 5.0f));
}

void handleMovement() {
	vec3 pos2 = getPosition(models[2]);
	vec3 pos3 = getPosition(models[3]);
	if (pos3.v[0] > 10.0) move = vec3(-.005f, 0.0f, 0.0f);
	else if (pos3.v[0] < -10.0) move = vec3(0.005f, 0.0f, 0.0f);

	if (pos2.v[0] > 10.0) move2 = vec3(-.001f, 0.0f, 0.0f);
	else if (pos2.v[0] < -10.0) move2 = vec3(0.001f, 0.0f, 0.0f);
}
#pragma endregion GAME MECHANICS

#pragma region TEXTURE LOADING
// This tutorial helped me get this texture loading
// http://docs.gl/gl4/glTexImage2D
GLuint textureLoading(char * texturePath) {
	//loading image
	// from Anton's advice in the discussion board
	int textureWidth, textureHeight, n;
	unsigned char * img = stbi_load(texturePath, &textureWidth, &textureHeight, &n, STBI_rgb);
	if (img == nullptr) std::cout << "unable to find: "<<texturePath << std::endl;
	GLuint texture;
	// Allocate space for texture and give it the image
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(img);
	std::cout << "Loaded: " << texturePath << std::endl;
	return texture;
}
#pragma endregion TEXTURE LOADING

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
						MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

bool load_mesh(const char* file_name) {
	g_vp.clear();
	g_vn.clear();
	g_vt.clear();
	const aiScene* scene = aiImportFile(file_name, aiProcess_Triangulate); // TRIANGLES!
	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return false;
	}
	printf("  %i animations\n", scene->mNumAnimations);
	printf("  %i cameras\n", scene->mNumCameras);
	printf("  %i lights\n", scene->mNumLights);
	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		g_point_count = mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				//printf ("      vp %i (%f,%f,%f)\n", v_i, vp->x, vp->y, vp->z);
				g_vp.push_back(vp->x);
				g_vp.push_back(vp->y);
				g_vp.push_back(vp->z);
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				//printf ("      vn %i (%f,%f,%f)\n", v_i, vn->x, vn->y, vn->z);
				g_vn.push_back(vn->x);
				g_vn.push_back(vn->y);
				g_vn.push_back(vn->z);
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				//printf ("      vt %i (%f,%f)\n", v_i, vt->x, vt->y);
				g_vt.push_back(vt->x);
				g_vt.push_back(vt->y);
			}
			if (mesh->HasTangentsAndBitangents()) {
				// NB: could store/print tangents here
			}
		}
	}
	aiReleaseImport(scene);
	return true;
}

#pragma endregion MESH LOADING

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS

// Create a NULL-terminated string by reading the provided file
char* readShaderSource(const char* shaderFile) {
	FILE* fp = fopen(shaderFile, "rb"); //!->Why does binary flag "RB" work and not "R"... wierd msvc thing?

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "../Shaders/simpleVertexShader.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "../Shaders/simpleFragmentShader.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program
	//can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another 
	// or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS

void generateObjectBufferMesh(char * meshPath, int myvao) {

	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/
	load_mesh(meshPath);
	//Note: you may get an error "vector subscript out of range" if you are using this code for a 
	//mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.
	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	// positions
	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, g_point_count * 3 * sizeof(float), &g_vp[0], GL_STATIC_DRAW);

	// normals
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, g_point_count * 3 * sizeof(float), &g_vn[0], GL_STATIC_DRAW);

	// text coords
	glGenBuffers (1, &vt_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glBufferData (GL_ARRAY_BUFFER, g_point_count * 2 * sizeof (float), &g_vt[0], GL_STATIC_DRAW);

	// bind them
	glBindVertexArray(myvao);

	// positions
	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// normals
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// text coords
	glEnableVertexAttribArray (loc3);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

#pragma endregion VBO_FUNCTIONS

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	if (anyKeyPressed) {
		//Declare your uniform variables that will be used in your shader
		int matrix_location = glGetUniformLocation(shaderProgramID, "model");
		int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
		int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
		//int camera_Pos = glGetUniformLocation(shaderProgramID, "light_position_world");


		// Root of the Hierarchy
		mat4 persp_proj = perspective(fov, (float)width / (float)height, 0.1, 100.0);

		// update uniforms & draw
		glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
		glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, models[0].m);
		//glUniformMatrix4fv(camera_Pos, 1, GL_FALSE, view.m);
		//add_text("Text", -1, 1, 20.0f, 1, 1, 1, 1);

		// ground
		glBindVertexArray(1);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawArrays(GL_TRIANGLES, 0, plane);

		
		// pyramid Mid
		glBindVertexArray(2);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		models[1] = rotate_y_deg(models[1], rotate_y);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, models[1].m);
		glDrawArrays(GL_TRIANGLES, 0, pyramid);

		// pyramid Left
		glBindVertexArray(2);
		models[2] = translate(models[2], move2);

		glBindTexture(GL_TEXTURE_2D, textures[1]);

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, models[2].m);
		glDrawArrays(GL_TRIANGLES, 0, pyramid);

		// pyramid Right
		glBindVertexArray(2);

		glBindTexture(GL_TEXTURE_2D, textures[1]);
		models[3] = translate(models[3], move);
		//models[3] = rotate_y_deg(models[3], pyramidX);
					
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, models[3].m);
		glDrawArrays(GL_TRIANGLES, 0, pyramid);

		// Other
		glBindVertexArray(3);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, models[4].m);
		glDrawArrays(GL_TRIANGLES, 0, palm);

		// walls
		glBindVertexArray(4);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glDrawArrays(GL_TRIANGLES, 0, walls);

		
	}
	// Draw the main Screen
	else {
		int hello_id = add_text("Press Any Key!", -1, 1, 20.0f,1, 1, 1, 1);
		draw_texts();
	}
	glutSwapBuffers();
}


void updateScene() {

	// Placeholder code, if you want to work with framerate
	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f) delta = 0.03f;
	last_time = curr_time;
	handleMovement();
	// Draw the next frame
	glutPostRedisplay();
}


void init(){
	// Set up the shaders
	//Shader myShader = new Shader();
	GLuint shaderProgramID = CompileShaders();
	// load in the textures
	textures[0] = textureLoading(SAND_PATH);
	textures[1] = textureLoading(BRICK_PATH);
	textures[2] = textureLoading(PALM_PATH); 
	textures[4] = textureLoading(WALLS_MESH_OBJ);

	GLuint * vertexArrays = (GLuint *)malloc(sizeof(GLuint) * 10);
	glGenVertexArrays(5, vertexArrays);
	// Draw the sand plane with that texture
	generateObjectBufferMesh(PLANE_MESH_OBJ,1);
	plane = g_point_count;
	// Generate the pyramids in the object
	generateObjectBufferMesh(PYRAMID_MESH_OBJ,2);
	pyramid = g_point_count;
	// draw palm trees
	generateObjectBufferMesh(BALL_MESH_OBJ, 3);
	palm = g_point_count;
	generateObjectBufferMesh(WALLS_MESH_OBJ,4);
	walls = g_point_count;

}

#pragma region HANDLE_INPUT

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	if (anyKeyPressed) {
		switch (key) {
		//	case 'w': view = translate(view, vec3(0.0f, 0.0f, 1.0f)); break;
	//		case 's': view = translate(view, vec3(0.0f, 0.0f, -1.0f)); break;
	//		case 'a': view = translate(view, vec3(1.0f, 0.0f, 0.0f)); break;
	//		case 'd': view = translate(view, vec3(-1.0f, 0.0f, 0.0f)); break;
	//		case 'q': view = rotate_z_deg(view, -1); break;
	//		case 'e': view = rotate_z_deg(view, 1); break;
			case ESC_BUTTON: exit(0);
		}
	}
	else 
		switch (key) {
			case ESC_BUTTON: exit(0);
			default: anyKeyPressed = true;
		}
	
}
// handles mouse scrolling this stackoverflow helped:
// http://stackoverflow.com/questions/14378/using-the-mouse-scrollwheel-in-glut
void mouseWheel(int button, int direction, int x, int y) {
	float newFov = 0;

	if (direction > 0) newFov = fov + 5;
	else newFov = fov - 5;

	if (newFov < 100.0 && newFov > 0) fov = newFov;
}

// Handles moving the mouse, where x and y are the co ordinates of the mouse
void mouseMove(int x, int y) {
	if (x > width - 50 && x < width) view = rotate_y_deg(view, 1);
	if (y > height - 50 && y < height)	view = rotate_x_deg(view, 1);
	if ((x < 50 && x > 0))  view = rotate_y_deg(view, -1);
	if (y < 50 && y > 0)  view = rotate_x_deg(view, -1);
}

// handles the movement of the player
void mySpecialKeyboardFunction(int key, int x, int y) {
	switch (key) {
		// Right and rotate Y positively
		case GLUT_KEY_RIGHT:
			playerModel = translate(playerModel, vec3(1.0f, 0.0f, 0.0f));
			break;
		// Down and rotate X positively
		case GLUT_KEY_DOWN:
			playerModel = translate(playerModel, vec3(0.0f, 0.0f, 1.0f));
			break;
		// Left and rotate Y negatively
		case GLUT_KEY_LEFT:
			playerModel = translate(playerModel, vec3(-1.0f, 0.0f, 0.0f));
			break;
		// Up and rotate Z negatively
		case GLUT_KEY_UP:
			playerModel = translate(playerModel, vec3(0.0f, 0.0f, -1.0f));
			break;
	}
	glutPostRedisplay(); //request display()
}
#pragma endregion HANDLE_INPUT

int main(int argc, char** argv) {
	initialiseModels();
	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Simon's Lab 5");
	
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutSpecialFunc(mySpecialKeyboardFunction);


	// Mouse Movement
	glutPassiveMotionFunc(mouseMove);

	// mouse wheel
	glutMouseWheelFunc(mouseWheel);
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	init_text_rendering("freemono.png", "freemono.meta", width, height);

	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
