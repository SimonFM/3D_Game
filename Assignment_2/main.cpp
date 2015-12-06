
//Some Windows Headers (For Time, IO, etc.)
#include "Includes.h"
#pragma warning(disable:4996)

#pragma region GAME MECHANICS
void initialiseModels() {
	//for (int i = 0; i < numOfModels; i++)	models.push_back(identity_mat4());
	//ground
	ground = scale(ground, vec3(2.0,2.0,2.0));
	
	//ball = translate(ball, vec3(0.0f, 0.0f, 0.0f));// middle
	ball1 = translate(ball1, vec3(-5.0f, 0.0f, -5.0f));	// left
	ball2 = translate(ball2, vec3(5.0f, 0.0f, -5.0f));	// left
	playerModel = translate(playerModel, vec3(-5.0f, 0.0f, 5.0f));	// player

}

void handleMovement() {
	vec3 pos2 = getPosition(ball1);
	vec3 pos3 = getPosition(ball2);
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
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, ground.m);
		//glUniformMatrix4fv(camera_Pos, 1, GL_FALSE, view.m);
		
		// ground
		glBindVertexArray(GROUND);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawArrays(GL_TRIANGLES, 0, plane);

		if (!ball1IsCaught) {
			// pyramid (Left)
			glBindVertexArray(PLAYER);
			glBindTexture(GL_TEXTURE_2D, textures[1]);

			glUniformMatrix4fv(matrix_location, 1, GL_FALSE, ball1.m);
			glDrawArrays(GL_TRIANGLES, 0, palm);
		}
		
		if (!ball2IsCaught) {
			// ball2( Right)
			glBindVertexArray(PLAYER);
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			glUniformMatrix4fv(matrix_location, 1, GL_FALSE, ball2.m);
			glDrawArrays(GL_TRIANGLES, 0, palm);
		}
		

		// Other
		glBindVertexArray(PLAYER);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, playerModel.m);
		glDrawArrays(GL_TRIANGLES, 0, palm);

		// walls
		glBindVertexArray(WALLS);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, wallsMat.m);
		glDrawArrays(GL_TRIANGLES, 0, walls);
		
	}
	// Draw the main Screen
	else {
		add_text("Press Any Key!", -1, 1, 20.0f,1, 1, 1, 1);
		draw_texts();
	}
	glutSwapBuffers();
}


void updateScene() {

	handleMovement();
	if (numOfModels == 1) exit(1);
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
	textures[2] = textureLoading(POKEBALL_PATH);
	textures[4] = textureLoading(WALLS_MESH_OBJ);

	GLuint * vertexArrays = (GLuint *)malloc(sizeof(GLuint) * VAO_SIZE);
	glGenVertexArrays(VAO_SIZE, vertexArrays);
	// Draw the sand plane with that texture
	generateObjectBufferMesh(PLANE_MESH_OBJ,GROUND);
	plane = g_point_count;
	// draw palm trees
	generateObjectBufferMesh(BALL_MESH_OBJ, PLAYER);
	palm = g_point_count;
	generateObjectBufferMesh(WALLS_MESH_OBJ, WALLS);
	walls = g_point_count;
}

#pragma region HANDLE_INPUT

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {
	if (anyKeyPressed) {
		switch (key) {
			case 'w': view = translate(view, vec3(0.0f, 0.0f, 1.0f)); break;
			case 's': view = translate(view, vec3(0.0f, 0.0f, -1.0f)); break;
			case 'a': view = translate(view, vec3(1.0f, 0.0f, 0.0f)); break;
			case 'd': view = translate(view, vec3(-1.0f, 0.0f, 0.0f)); break;
			case 'q': view = rotate_z_deg(view, -1); break;
			case 'e': view = rotate_z_deg(view, 1); break;
			case ESC_BUTTON: exit(0);
		}
		print(view);
	}
	else {
		switch (key) {
			case ESC_BUTTON: exit(0);
			default: anyKeyPressed = true;
		}
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


// keeps a record of each model's position in the system.
void updatePositions(mat4 newPlayerPos, mat4 newBall1, mat4 newBall2) {
	modelsPos[0] = getPosition(newPlayerPos);
	for (int i = 1; i < numOfModels; i++) {
		switch (i) {
		case 1:
			modelsPos[i] = getPosition(newBall1);
			break;
		case 2:
			modelsPos[i] = getPosition(newBall2);
			break;
		}
	}
	
}

// Returns the distance between two points in the 3D co ordinate system.
double distanceBetweenPoints(vec3 p1, vec3 p2) {
	double t1 = p1.v[0] - p2.v[0];
	double t2 = p1.v[1] - p2.v[1];
	double t3 = p1.v[2] - p2.v[2];
	return sqrt((t1*t1) + (t2*t2) + (t3*t3));
}

void moveLeft(int result, mat4 newPlayerModel, mat4 newball1, mat4 newball2) {
	if (result < 0) {
		std::cout << "You caught: " << result * -1 << std::endl;
		if (result * -1 == 1) {
			ball1IsCaught = true;
			modelsPos.erase(modelsPos.begin() + 1);
		}
		else {
			ball2IsCaught = true;
			modelsPos.erase(modelsPos.begin() + 2);
		}
		numOfModels--;
	}
	else if (result == 0) {
		playerModel = newPlayerModel;
		if (!ball1IsCaught)
			ball1 = translate(ball1, vec3(1.0f, 0.0f, 0.0f));
		if (!ball2IsCaught)
			ball2 = translate(ball2, vec3(1.0f, 0.0f, 0.0f));
		updatePositions(playerModel, ball1, ball2);
	}
}

void moveRight(int result, mat4 newPlayerModel, mat4 newball1, mat4 newball2) {
	if (result < 0) {
		std::cout << "You caught: " << result * -1 << std::endl;
		if (result * -1 == 1) {
			ball1IsCaught = true;
			modelsPos.erase(modelsPos.begin() + 1);
		}
		else {
			ball2IsCaught = true;
			modelsPos.erase(modelsPos.begin() + 2);
		}
		numOfModels--;
	}
	else if (result == 0) {
		playerModel = newPlayerModel;
		if (!ball1IsCaught)
			ball1 = translate(ball1, vec3(-1.0f, 0.0f, 0.0f));
		if (!ball2IsCaught)
			ball2 = translate(ball2, vec3(-1.0f, 0.0f, 0.0f));
		updatePositions(playerModel, ball1, ball2);
	}
}

void moveUp(int result, mat4 newPlayerModel, mat4 newball1, mat4 newball2) {
	if (result < 0) {
		std::cout << "You caught: " << result * -1 << std::endl;
		if (result * -1 == 1) {
			ball1IsCaught = true;
			modelsPos.erase(modelsPos.begin() + 1);
		}
		else {
			ball2IsCaught = true;
			modelsPos.erase(modelsPos.begin() + 2);
		}
		numOfModels--;
	}
	else if (result == 0) {
		playerModel = newPlayerModel;
		if (!ball1IsCaught)
			ball1 = translate(ball1, vec3(0.0f, 0.0f, 1.0f));
		if (!ball2IsCaught)
			ball2 = translate(ball2, vec3(0.0f, 0.0f, 1.0f));
		updatePositions(playerModel, ball1, ball2);
	}
}

void moveDown(int result, mat4 newPlayerModel, mat4 newball1, mat4 newball2) {
	if (result < 0) {
		std::cout << "You caught: " << result * -1 << std::endl;
		if (result * -1 == 1) {
			ball1IsCaught = true;
			modelsPos.erase(modelsPos.begin() + 1);
		}
		else {
			ball2IsCaught = true;
			modelsPos.erase(modelsPos.begin() + 2);
		}
		numOfModels--;
	}
	else if (result == 0) {
		playerModel = newPlayerModel;
		if (!ball1IsCaught)
			ball1 = translate(ball1, vec3(0.0f, 0.0f, -1.0f));
		if (!ball2IsCaught)
			ball2 = translate(ball2, vec3(0.0f, 0.0f, -1.0f));
		updatePositions(playerModel, ball1, ball2);
	}
}

// Compares the distance between each object.
int collisionDetection(mat4 newPlayerPos, mat4 newball1, mat4 newball2) {
	updatePositions(newPlayerPos, newball1, newball2);
	double dist;

	for (int i = 1; i < numOfModels; i++) {
		dist = distanceBetweenPoints(modelsPos[0], modelsPos[i]);
		std::cout <<"Distance: "<< dist << std::endl;
		if (dist < 2) return i * -1;;
	}

	// Now detect if the ball is within the box
	for (int i = 1; i < numOfModels; i++) {
		print(modelsPos[i]);
		// X Bounds
		if (modelsPos[i].v[0] > 1) return 1;
		else if (modelsPos[i].v[0] < -10) return 1;
		// Y Bounds
		else if (modelsPos[i].v[1] > BOUNDARY) return 1;
		else if (modelsPos[i].v[1] < BOUNDARY * -1) return 1;
		// Z Bounds
		else if (modelsPos[i].v[2] > BOUNDARY) return 1;
		else if (modelsPos[i].v[2] < BOUNDARY * -1) return 1;
		else return 0;
	}
} 

// handles the movement of the player
void mySpecialKeyboardFunction(int key, int x, int y) {
	mat4 newPlayerModel, newball1, newball2;
	int result;
	switch (key) {
		// Move right
		case GLUT_KEY_RIGHT:
			newPlayerModel = translate(playerModel, vec3(1.0f, 0.0f, 0.0f));
			newball1 = translate(ball1, vec3(-1.0f, 0.0f, 0.0f));
			newball2 = translate(ball2, vec3(-1.0f, 0.0f, 0.0f));
			result = collisionDetection(newPlayerModel, newball1, newball2);
			
			moveRight(result, newPlayerModel, newball1, newball2);
			break;
		// Move back
		case GLUT_KEY_DOWN:
			newPlayerModel = translate(playerModel, vec3(0.0f, 0.0f, 1.0f));
			newball1 = translate(ball1, vec3(0.0f, 0.0f, -1.0f));
			newball2 = translate(ball2, vec3(0.0f, 0.0f, -1.0f));
			result = collisionDetection(newPlayerModel, newball1, newball2);

			moveDown(result, newPlayerModel, newball1, newball2);
			break;
		//Move left
		case GLUT_KEY_LEFT:
			newPlayerModel = translate(playerModel, vec3(-1.0f, 0.0f, 0.0f));
			newball1 = translate(ball1, vec3(1.0f, 0.0f, 0.0f));
			newball2 = translate(ball2, vec3(1.0f, 0.0f, 0.0f));
			result = collisionDetection(newPlayerModel, newball1, newball2);
			
			moveLeft(result, newPlayerModel, newball1, newball2);
			break;
		// Move Forward
		case GLUT_KEY_UP:
			newPlayerModel = translate(playerModel, vec3(0.0f, 0.0f, -1.0f));
			newball1 = translate(ball1, vec3(0.0f, 0.0f, 1.0f));
			newball2 = translate(ball2, vec3(0.0f, 0.0f, 1.0f));
			result = collisionDetection(newPlayerModel, newball1, newball2);
			
			moveUp(result, newPlayerModel, newball1, newball2);
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
	modelsPos.push_back(vec3(0.0f, 0.0f, 0.0f));
	modelsPos.push_back(vec3(0.0f, 0.0f, 0.0f));
	modelsPos.push_back(vec3(0.0f, 0.0f, 0.0f));
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
