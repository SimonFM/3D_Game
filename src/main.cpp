#include "Includes.h"
#pragma warning(disable:4996)

// This was from a tutorial found here:
// https://www.youtube.com/watch?v=elE__Nouv54
void drawText(const char * textToDraw, int length, int x, int y) {
	glColor3f(0, 1, 0);
	glMatrixMode(GL_PROJECTION);
	double * matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);

	for (int i = 0; i < length; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)textToDraw[i]);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}
#pragma region GAME MECHANICS

// Compares the distance between each object.
int collisionDetection(mat4 newPlayerPos, std::vector<mat4>  newBalls) {
	vec3 playerPosition = getPosition(newPlayerPos);
	int i, temp = 0;

	// check Player Position with Boulders
	for (i = 0; i < bouldersPos.size(); i++)
		if (getPosition(newPlayerPos).v[0] == bouldersPos[i].v[0]
			&& getPosition(newPlayerPos).v[2] == bouldersPos[i].v[2]) return -2;

	// Check Enemies with the Boulders
	for (i = 0; i < bouldersPos.size(); i++)
		for (int j = 0; j < newBalls.size(); j++)
			if (getPosition(newBalls[j]).v[0] == bouldersPos[i].v[0]
				&& getPosition(newBalls[j]).v[2] == bouldersPos[i].v[2]) return -3;

	// check Enemy Collision with the Player
	for (i = 0; i < newBalls.size(); i++)
		if (getPosition(playerModel).v[0] == getPosition(newBalls[i]).v[0]
			&& getPosition(playerModel).v[2] == getPosition(newBalls[i]).v[2]) {
			ballModels.erase(ballModels.begin() + i);
			return -1;
		}

	// check against current position
		else if (getPosition(newPlayerPos).v[0] == getPosition(newBalls[i]).v[0]
			&& getPosition(newPlayerPos).v[2] == getPosition(newBalls[i]).v[2]) {
			ballModels.erase(ballModels.begin() + i);
			return -1;
		}
		return 0;
}


// Placeholder code for the keypress
// To move a player and the balls, I simply calculated the next position
// of each of the models and then checked if any of them hit with each other
// both in their current position and in their next position.
// The result of this collision detection determined the result of the movement.
void keypress(unsigned char key, int x, int y) {
	mat4 newPlayerModel, newball1, newball2;
	std::vector<mat4>  balls;
	int result;
	switch (key) {
		// Move right
	case 'd':
		if (gameStart) {
			newPlayerModel = translate(playerModel, vec3(1.0f, 0.0f, 0.0f));
			for (int i = 0; i < ballModels.size(); i++)
				balls.push_back(translate(ballModels[i], vec3(-1.0f, 0.0f, 0.0f)));
			result = collisionDetection(newPlayerModel, balls);
			std::cout << "results " << result << std::endl;

			switch (result) {
			case 0:
				moveEverybody(newPlayerModel, balls);
				break;
			case -1: // check Enemy Collision with the Player
				hasWon--;
				break;
			case -3:	// Check Enemies with the Boulders
				playerModel = newPlayerModel;
				break;
			}
		}

		break;
		// Move back
	case 's':
		if (gameStart) {
			newPlayerModel = translate(playerModel, vec3(0.0f, 0.0f, 1.0f));
			for (int i = 0; i < ballModels.size(); i++)
				balls.push_back(translate(ballModels[i], vec3(0.0f, 0.0f, -1.0f)));
			result = collisionDetection(newPlayerModel, balls);
			std::cout << "results " << result << std::endl;
			switch (result) {
			case 0:
				moveEverybody(newPlayerModel, balls);
				break;
			case -1: // check Enemy Collision with the Player
				hasWon--;
				break;
			case -3:	// Check Enemies with the Boulders
				playerModel = newPlayerModel;
				break;
			}
		}
		break;
		//Move left
	case 'a':
		if (gameStart) {
			newPlayerModel = translate(playerModel, vec3(-1.0f, 0.0f, 0.0f));
			for (int i = 0; i < ballModels.size(); i++)
				balls.push_back(translate(ballModels[i], vec3(1.0f, 0.0f, 0.0f)));

			result = collisionDetection(newPlayerModel, balls);
			std::cout << "results " << result << std::endl;
			switch (result) {
			case 0:
				moveEverybody(newPlayerModel, balls);
				break;
			case -1: // check Enemy Collision with the Player
				hasWon--;
				break;
			case -3:	// Check Enemies with the Boulders
				playerModel = newPlayerModel;
				break;
			}
		}

		break;
		// Move Forward
	case 'w':
		if (gameStart) {
			newPlayerModel = translate(playerModel, vec3(0.0f, 0.0f, -1.0f));
			for (int i = 0; i < ballModels.size(); i++)
				balls.push_back(translate(ballModels[i], vec3(0.0f, 0.0f, 1.0f)));

			result = collisionDetection(newPlayerModel, balls);
			std::cout << "results " << result << std::endl;

			switch (result) {
			case 0:
				moveEverybody(newPlayerModel, balls);
				break;
			case -1: // check Enemy Collision with the Player
				hasWon--;
				break;
			case -3:	// Check Enemies with the Boulders
				playerModel = newPlayerModel;
				break;
			}
		}
		break;
	case 'D':
		if (gameStart)	camera.ProcessKeyboard(RIGHT, VELOCITY);
		break;
		// Move back
	case 'S':
		if (gameStart)	camera.ProcessKeyboard(BACKWARD, VELOCITY);
		break;
		//Move left
	case 'A':
		if (gameStart) camera.ProcessKeyboard(LEFT, VELOCITY);
		break;
		// Move Forward
	case 'W':
		if (gameStart) camera.ProcessKeyboard(FORWARD, VELOCITY);
		break;
	case 'c':
		if (gameStart)	camera.Position = PLAYERSTARTPOINT;
		break;
	case ' ':
		gameStart = true;
		break;
	case RERDAW:
		reloadLevel();
		break;
	case ESC_BUTTON: exit(0);
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
// My improved camera movement was found through a tutorial on learnopengl.com
// http://learnopengl.com/#!Getting-started/Camera
void mouseMove(int x, int y) {
	
	GLfloat xoffset = x - lastX;
	GLfloat yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

	lastX = x; lastY = y;
	if (lastX > width - 200 || lastX < 200 || lastY < 200 || lastY > height - 200) {
		glutWarpPointer(width / 2, height / 2);
		lastX = width / 2;
		lastY = height / 2;
	}
	else camera.ProcessMouseMovement(xoffset, yoffset);
}


// Moves everyone to their new locations.
void moveEverybody(mat4 newPlayerModel, std::vector<mat4> balls) {
	for (int i = 0; i < balls.size(); i++) {
		ballModels[i] = balls[i];
	}
	playerModel = newPlayerModel;
}

#pragma endregion GAME MECHANICS

#pragma region MAPS

// A function that when given some image data, calculates the positions and the number
// of models in a scene.
void getPixels(unsigned char * img, int imgWidth, int imgHeight, int n) {
	std::vector<unsigned char> pixels;
	int enemies = 0, world = 0, bouldersCount = 0, player = 0;
	int count = 0;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			vec3 currentLocation = vec3(i - 30, 0.0f, j - 30);
			vec3 currentGroundLocation = vec3(i - 31, 0.0f, j - 31);

			if (count == imgHeight) {
				std::cout << std::endl;
				count = 0;
			}
			switch (img[i + (j * imgWidth)]) { // treats the single array as if it was a 2D array
			case WHITE:
				world++;
				grounds.push_back(translate(identity_mat4(), currentGroundLocation));
				std::cout << " _ ";
				break;
			case BLACK:
				bouldersCount++;
				std::cout << " B ";
				boulders.push_back(translate(identity_mat4(), currentLocation));
				bouldersPos.push_back(getPosition(translate(identity_mat4(), currentLocation)));
				break;
			case RED:
				std::cout << " E ";
				enemies++;
				ballModels.push_back(translate(identity_mat4(), currentLocation));
				break;
			case BLUE:
				player++;
				playerModel = translate(identity_mat4(), currentLocation);	// player
				std::cout << " P ";
				break;
			}
			count++;
		}
	}
	hasWon = enemies;
}


// Loads a scene from an image.
void loadMapFromImage(char * mapPath) {
	int imgWidth, imgHeight, n;
	unsigned char * img = stbi_load(mapPath, &imgWidth, &imgHeight, &n, 1);
	if (img == nullptr) std::cout << "unable to find: " << mapPath << std::endl;
	getPixels(img, imgWidth, imgHeight, n);

}
#pragma endregion MAPS

#pragma region TEXTURE LOADING
// This tutorial helped me get this texture loading
// http://docs.gl/gl4/glTexImage2D
GLuint textureLoading(char * texturePath) {
	//loading image
	// from Anton's advice in the discussion board
	int textureWidth, textureHeight, n;
	unsigned char * img = stbi_load(texturePath, &textureWidth, &textureHeight, &n, STBI_rgb);
	if (img == nullptr) std::cout << "unable to find: " << texturePath << std::endl;
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


/**
* Loads a cubemap texture from 6 individual texture faces
* Order should be:
* +X (right)
* -X (left)
* +Y (top)
* -Y (bottom)
* +Z (front)
* -Z (back)
* This was implemented using help found on learnopengl.com
**/
GLuint loadCubemap(std::vector<std::string> faces) {
	GLuint textureID;
	glGenTextures(1, &textureID);

	int widthimg, heightImg, n;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++) {
		image = stbi_load(faces[i].c_str(), &widthimg, &heightImg, &n, STBI_rgb);
		if (!image) std::cout << "cant find the skybox texture: " << faces[i] << std::endl;
		std::cout << "Width: " << widthimg << std::endl;
		std::cout << "Height: " << heightImg << std::endl;
		std::cout << "Loaded: " << faces[i] << std::endl;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, widthimg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
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

#pragma region SKYBOXREGION

// The textures for the skybox were found online
// http://www.custommapmakers.org/skyboxes.php
// I found out how to do skyboxes by following a tutorial onlearnopengl.com
// http://learnopengl.com/#!Advanced-OpenGL/Cubemaps
void loadSkyBox() {
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	faces.push_back("../skybox/front.tga"); //right
	faces.push_back("../skybox/back.tga"); //left
	faces.push_back("../skybox/top.tga");//top
	faces.push_back("../skybox/bottom.tga"); //bottom
	faces.push_back("../skybox/right.tga"); //back
	faces.push_back("../skybox/left.tga"); //front
	skyboxTexture = loadCubemap(faces);
}

// The textures for the skybox were found online
// http://www.custommapmakers.org/skyboxes.php
// I found out how to do skyboxes by following a tutorial onlearnopengl.com
// http://learnopengl.com/#!Advanced-OpenGL/Cubemaps
void drawSkybox(mat4 persp_proj) {
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(skyBoxProgramID);
	glUniformMatrix4fv(glGetUniformLocation(skyBoxProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyBoxProgramID, "projection"), 1, GL_FALSE, persp_proj.m);
	// Remove any translation component of the view matrix
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(skyBoxProgramID, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS); // Set depth function back to default
}
#pragma endregion SKYBOXREGION

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


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType){
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		getch();
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
		getch();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders(bool isSkyBox){
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	GLuint shader = glCreateProgram();
	if (shader == 0) {
		fprintf(stderr, "Error creating shader program\n");
		getch();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	if (!isSkyBox) {
		AddShader(shader, "../Shaders/simpleVertexShader.shader", GL_VERTEX_SHADER);
		AddShader(shader, "../Shaders/simpleFragmentShader.shader", GL_FRAGMENT_SHADER);
		std::cout << "Added Normal Shaders" << std::endl;
	}
	else {
		AddShader(shader, "../Shaders/skyBoxVertex.shader", GL_VERTEX_SHADER);
		AddShader(shader, "../Shaders/skyBoxFrag.shader", GL_FRAGMENT_SHADER);
		std::cout << "Added Skybox Shaders" << std::endl;
	}

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shader);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shader, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		glGetProgramInfoLog(shader, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		getch();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program
	// can execute given the current pipeline state
	glValidateProgram(shader);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shader, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		getch();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another 
	// or explicitly disable its use
	glUseProgram(shader);
	return shader;
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
	glGenBuffers(1, &vt_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
	glBufferData(GL_ARRAY_BUFFER, g_point_count * 2 * sizeof(float), &g_vt[0], GL_STATIC_DRAW);

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
	glEnableVertexAttribArray(loc3);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo);
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}


#pragma endregion VBO_FUNCTIONS

void display() {
	view = camera.GetViewMatrix();
	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");

	// Root of the Hierarchy
	mat4 persp_proj = perspective(fov, (float)width / (float)height, 0.1, 100.0);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, glm::value_ptr(view));

	// draw Ground
	for (int i = 0; i < grounds.size(); i++) {
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, grounds[i].m);
		// ground
		glBindVertexArray(GROUND);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawArrays(GL_TRIANGLES, 0, plane);
	}

	// draw Enenmies
	for (int i = 0; i < ballModels.size(); i++) {
		glBindVertexArray(PLAYER);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, ballModels[i].m);
		glDrawArrays(GL_TRIANGLES, 0, ballVerticesSize);
	}

	// Hierarchal Model
	// Player root
	glBindVertexArray(PLAYER);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, playerModel.m);
	glDrawArrays(GL_TRIANGLES, 0, ballVerticesSize);

	glBindVertexArray(PLAYER);
	glBindTexture(GL_TEXTURE_2D, textures[2]);

	// Body of player
	mat4 body = identity_mat4();
	body = translate(playerModel, vec3(0.0f, 1.0f, 0.0f));
	
	// update uniform & draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, body.m);
	glDrawArrays(GL_TRIANGLES, 0, ballVerticesSize);

	mat4 rotatingArm1 = identity_mat4();
	rotatingArm1 = translate(rotatingArm1, vec3(1.0f, 0.0f, 0.0f));
	rotatingArm1 = rotate_x_deg(rotatingArm1, rotatey);
	mat4 globalArm1 = body * rotatingArm1;
	// update uniform & draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, globalArm1.m);
	glDrawArrays(GL_TRIANGLES, 0, ballVerticesSize);

	mat4 rotatingArm2 = identity_mat4();
	rotatingArm2 = translate(rotatingArm2, vec3(-1.0f, 0.0f, 0.0f));
	rotatingArm2 = rotate_x_deg(rotatingArm2, rotatey);
	mat4 globalArm2 = body * rotatingArm2;
	// update uniform & draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, globalArm2.m);
	glDrawArrays(GL_TRIANGLES, 0, ballVerticesSize);

	// Boulders
	for (int i = 0; i < boulders.size(); i++) {
		glBindVertexArray(PLAYER);
		glBindTexture(GL_TEXTURE_2D, textures[5]);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, boulders[i].m);
		glDrawArrays(GL_TRIANGLES, 0, ballVerticesSize);
	}

	// when the player wins, draw some text
	if (gameEnd) {
		std::string won = "Congrats! You win!";
		drawText(won.data(), won.size(), 325, 400);
	}
	else { // otherwise just display how many enemies are left
		std::string text = " Enemies Left: " + std::to_string(hasWon);
		drawText(text.data(), text.size(), 5, 100);
	}

	if (!gameStart) { // while the player hasnt started the game
		std::string gameStartText = "Press Space to begin";
		drawText(gameStartText.data(), gameStartText.size(), 325, 400);
	}

	// Skybox drawing.
	drawSkybox(persp_proj);
	glutSwapBuffers();

}

#pragma region LEVELS
// A method that redraws the models should the player get stuck.
void reloadLevel() {
	//clear all the models
	boulders.clear();
	grounds.clear();
	modelsPos.clear();
	bouldersPos.clear();
	ballModels.clear();

	// draw a different level
	switch (currentLevel) {
	case 0:
		std::cout << "Reloading Level 1" << std::endl;
		loadMapFromImage(LEVEL_1);
		break;
	case 1:
		std::cout << "Reloading Level 2" << std::endl;
		loadMapFromImage(LEVEL_2);
		break;
	case 2:
		std::cout << "Reloading Level 3" << std::endl;
		loadMapFromImage(LEVEL_3);
		break;
	case 3:
		std::cout << "Reloading Level 4" << std::endl;
		loadMapFromImage(LEVEL_4);
		currentLevel++;
		break;
	case 4:
		std::cout << "Reloading Level 5" << std::endl;
		loadMapFromImage(LEVEL_5);
		break;
	}

}

// Loads the next level when the player has killed all the enemies
void loadNextLevel() {
	//clear all the models
	boulders.clear();
	grounds.clear();
	modelsPos.clear();
	bouldersPos.clear();

	// draw a different level
	switch (currentLevel) {
	case 0:
		std::cout << "Loading Level 2" << std::endl;
		loadMapFromImage(LEVEL_2);
		currentLevel++;
		break;
	case 1:
		std::cout << "Loading Level 3" << std::endl;
		loadMapFromImage(LEVEL_3);
		currentLevel++;
		break;
	case 2:
		std::cout << "Loading Level 4" << std::endl;
		loadMapFromImage(LEVEL_4);
		currentLevel++;
		break;
	case 3:
		std::cout << "Loading Level 5" << std::endl;
		loadMapFromImage(LEVEL_5);
		currentLevel++;
		break;
	default:
		std::cout << "Congrats! You win!" << std::endl;
		gameEnd = true;

		break;
	}

}
#pragma endregion LEVELS

void updateScene() {
	rotatey += 0.5;
	if (hasWon == 0) loadNextLevel();
	// Draw the next frame
	glutPostRedisplay();
}

void init() {
	// Set up the shaders
	shaderProgramID = CompileShaders(false);
	skyBoxProgramID = CompileShaders(true);

	// load in the textures
	textures[0] = textureLoading(SAND_PATH);
	textures[1] = textureLoading(BRICK_PATH);
	textures[2] = textureLoading(POKEBALL_PATH);
	textures[5] = textureLoading(BOULDER_PATH);

	GLuint * vertexArrays = (GLuint *)malloc(sizeof(GLuint) * VAO_SIZE);
	glGenVertexArrays(VAO_SIZE, vertexArrays);
	// Draw the sand plane with that texture
	generateObjectBufferMesh(PLANE_MESH_OBJ, GROUND);
	plane = g_point_count;
	// draw ballVerticesSize trees
	generateObjectBufferMesh(BALL_MESH_OBJ, PLAYER);
	ballVerticesSize = g_point_count;
	std::cout << "Loading Level 1" << std::endl;
	loadMapFromImage(LEVEL_1);


	// music was found on new grounds:
	// http://www.newgrounds.com/audio/listen/657490
	LPCSTR a = "open ../Music/music.mp3 type mpegvideo";
	mciSendString(a, NULL, 0, 0);
	LPCSTR b = "play ../Music/music.mp3 repeat";
	int error2 = mciSendString(b, NULL, 0, 0);
	loadSkyBox();
}

int main(int argc, char ** argv) {
	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Catch Them!!!");
	modelsPos.push_back(vec3(0.0f, 0.0f, 0.0f));
	modelsPos.push_back(vec3(0.0f, 0.0f, 0.0f));
	modelsPos.push_back(vec3(0.0f, 0.0f, 0.0f));
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	//glutSpecialFunc(mySpecialKeyboardFunction);
	glutSetCursor(GLUT_CURSOR_NONE);

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

	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
