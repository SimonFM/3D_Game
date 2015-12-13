
//Some Windows Headers (For Time, IO, etc.)
#include "Includes.h"

#pragma warning(disable:4996)

Camera camera(PLAYERSTARTPOINT);


// This was from a tutorial found here:
// https://www.youtube.com/watch?v=elE__Nouv54
void drawText(const char * textToDraw, int length, int x, int y) {
	glColor3f(0, 1, 0);
	glMatrixMode(GL_PROJECTION);
	double * matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX,matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);

	for (int i = 0; i < length; i++) 
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int) textToDraw[i]);

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
void keypress(unsigned char key, int x, int y) {
	mat4 newPlayerModel, newball1, newball2;
	std::vector<mat4>  balls ;
	int result;
		switch (key) {
			// Move right
		case 'd':
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
				case -2:  // check Player Position with Boulders

					break;
				case -3:	// Check Enemies with the Boulders
					playerModel = newPlayerModel;

					break;
			}
			
			break;
			// Move back
		case 's':
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
			case -2:  // check Player Position with Boulders

				break;
			case -3:	// Check Enemies with the Boulders
				playerModel = newPlayerModel;

				break;
			}
			break;
			//Move left
		case 'a':
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
			case -2:  // check Player Position with Boulders

				break;
			case -3:	// Check Enemies with the Boulders
				playerModel = newPlayerModel;

				break;
			}
			break;
			// Move Forward
		case 'w':
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
			case -2:  // check Player Position with Boulders

				break;
			case -3:	// Check Enemies with the Boulders
				playerModel = newPlayerModel;
				break;
			}
			break;
		case 'D':
			camera.ProcessKeyboard(RIGHT, 0.5);
			print(camera.Position);

			break;
			// Move back
		case 'S':
			camera.ProcessKeyboard(BACKWARD, 0.5);
			print(camera.Position);

			break;
			//Move left
		case 'A':
			camera.ProcessKeyboard(LEFT, 0.5);
			print(camera.Position);

			break;
			// Move Forward
		case 'W':
			camera.ProcessKeyboard(FORWARD, 0.5);

			print(camera.Position);
			break;
		case 'c':
			camera.Position = OVERHEADVIEW;
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
void mouseMove(int x, int y) {
	if (firstMouse){
		lastX = x;
		lastY = y;
		firstMouse = false;
	}
	GLfloat xoffset = x - lastX;
	GLfloat yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

	lastX = x;lastY = y;
	if (lastX > width - 200 || lastX < 200 || lastY < 200 || lastY > height - 200) {
		glutWarpPointer(width/2,height/2);
		lastX = width / 2;
		lastY = height / 2;
	}
	else camera.ProcessMouseMovement(xoffset, yoffset);
}



void moveEverybody(mat4 newPlayerModel, std::vector<mat4> balls) {
	for (int i = 0; i < balls.size(); i++) {
		ballModels[i] = balls[i];
	}
	playerModel = newPlayerModel;
}

#pragma endregion GAME MECHANICS



#pragma region MAPS

void getPixels(unsigned char * img, int imgWidth, int imgHeight, int n) {
	std::vector<unsigned char> pixels;
	int enemies = 0, world = 0, bouldersCount = 0, player = 0 ;
	int count = 0;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			vec3 currentLocation = vec3(i, 0.0f, j);
			vec3 currentGroundLocation = vec3(i-1, 0.0f, j-1);

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

	std::cout << "size: " << bouldersPos.size() << std::endl;

	std::cout << "enemies: " << enemies <<std::endl;
	std::cout << "World: " << world << std::endl;
	std::cout << "boulders: " << bouldersCount << std::endl;
	std::cout << "player: " << player << std::endl;
	hasWon = enemies;
}

void printMapValues(unsigned char * img, int imgWidth, int imgHeight, int n) {
	int count = 0;
	int black = 0, white = 0, red = 0, yellow = 0;
	getPixels(img, imgWidth, imgHeight, n);

}

void loadMapFromImage(char * mapPath) {
	int imgWidth, imgHeight, n;
	unsigned char * img = stbi_load(mapPath, &imgWidth, &imgHeight, &n, 1);
	if (img == nullptr) std::cout << "unable to find: " << mapPath << std::endl;
	printMapValues(img, imgWidth, imgHeight, n);
	
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
		getch();
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
	AddShader(shaderProgramID, "../Shaders/simpleVertexShader.shader", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "../Shaders/simpleFragmentShader.shader", GL_FRAGMENT_SHADER);
	/*AddShader(skyID, "../Shaders/skyBoxVertex.shader", GL_VERTEX_SHADER);
	AddShader(skyID, "../Shaders/skyBoxFrag.shader", GL_FRAGMENT_SHADER);*/

	/*AddShader(shaderProgramID, "../Shaders/LightingVertex.shader", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "../Shaders/LightingFragment.shader", GL_FRAGMENT_SHADER);*/

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		getch();
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
	mat4 skyModel = identity_mat4();
	mat4 skyProj = identity_mat4();
	if (gameStart) {
		// tell GL to only draw onto a pixel if the shape is closer to the viewer
		glEnable(GL_DEPTH_TEST); // enable depth-testing
		glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgramID);

		//Declare your uniform variables that will be used in your shader
		int matrix_location = glGetUniformLocation(shaderProgramID, "model");
		int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
		int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");

		// Root of the Hierarchy
		mat4 persp_proj = perspective(fov, (float)width / (float)height, 0.1, 100.0);

		view = camera.GetViewMatrix();

		/*std::cout << " View" << std::endl;
		print(getPosition(view));*/
		// update uniforms & draw
		glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
		glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);

		// draw Ground
		for (int i = 0; i < grounds.size(); i++) {
			//std::cout << "size of ground: " << grounds.size() << std::endl;
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
			glDrawArrays(GL_TRIANGLES, 0, palm);
		}


		// Player
		glBindVertexArray(PLAYER);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, playerModel.m);
		glDrawArrays(GL_TRIANGLES, 0, palm);

		glBindVertexArray(PLAYER);
		glBindTexture(GL_TEXTURE_2D, textures[2]);

		mat4 body = identity_mat4();
		// translation is 15 units in the y direction from the parents coordinate system
		body = translate(playerModel, vec3(0.0f, 1.0f, 0.0f));
		// global of the child is got by pre-multiplying the local of the child by the global of the parent
		//mat4 toDraw = playerModel * rotatingHead;
		// update uniform & draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, body.m);
		glDrawArrays(GL_TRIANGLES, 0, palm);

		mat4 rotatingHead = identity_mat4();
		// translation is 15 units in the y direction from the parents coordinate system
		rotatingHead = translate(body, vec3(0.0f, 1.0f, 0.0f));
		// global of the child is got by pre-multiplying the local of the child by the global of the parent
		//mat4 toDraw = playerModel * rotatingHead;
		// update uniform & draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, rotatingHead.m);
		glDrawArrays(GL_TRIANGLES, 0, palm);
		
		// Boulders
		for (int i = 0; i < boulders.size(); i++) {
			glBindVertexArray(PLAYER);
			glBindTexture(GL_TEXTURE_2D, textures[5]);
			glUniformMatrix4fv(matrix_location, 1, GL_FALSE, boulders[i].m);
			glDrawArrays(GL_TRIANGLES, 0, palm);
		}


		if (gameEnd) {
			std::string won = "Congrats! You win!";
			drawText(won.data(), won.size(), 325, 400);
		}
		else {
			std::string text = " Enemies Left: " + std::to_string(hasWon);
			drawText(text.data(), text.size(), 5, 100);
		}
	}
	else {
		std::string gameStartText = "Press Space to begin";
		drawText(gameStartText.data(), gameStartText.size(), 325, 400);
	}
	
	glutSwapBuffers();
}

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

void loadNextLevel() {
	//clear all the models
	boulders.clear();
	grounds.clear();
	modelsPos.clear();
	bouldersPos.clear();
	 
	// draw a different level
	switch(currentLevel) {
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


void updateScene() {
	rotatey += 0.5;
	if (hasWon == 0) loadNextLevel();
	// Draw the next frame
	glutPostRedisplay();
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(std::vector<const GLchar*> faces){

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = stbi_load(faces[i], &width, &height, 0, STBI_rgb_alpha);
		if (!image) std::cout << "Unable to find image" << std::endl;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
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

void init(){
	// Set up the shaders
	//Shader myShader = new Shader();
	GLuint shaderProgramID = CompileShaders();
	// load in the textures
	textures[0] = textureLoading(SAND_PATH);
	textures[1] = textureLoading(BRICK_PATH);
	textures[2] = textureLoading(POKEBALL_PATH);
	textures[4] = textureLoading(WALLS_MESH_OBJ);
	textures[5] = textureLoading(BOULDER_PATH);

#pragma region "object_initialization"
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	// Setup skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Cubemap (Skybox)
	std::vector<const GLchar*> faces;
	faces.push_back("../skybox/right.tga");
	faces.push_back("../skybox/left.tga");
	faces.push_back("../skybox/top.tga");
	faces.push_back("../skybox/bottom.tga");
	faces.push_back("../skybox/back.tga");
	faces.push_back("../skybox/front.tga");
	skyboxTexture = loadCubemap(faces);
	std::cout << "Loaded SkyBox" << std::endl;
#pragma endregion

	GLuint * vertexArrays = (GLuint *) malloc(sizeof (GLuint) * VAO_SIZE);
	glGenVertexArrays(VAO_SIZE, vertexArrays);
	// Draw the sand plane with that texture
	generateObjectBufferMesh(PLANE_MESH_OBJ, GROUND);
	plane = g_point_count;
	// draw palm trees
	generateObjectBufferMesh(BALL_MESH_OBJ, PLAYER);
	palm = g_point_count;
	generateObjectBufferMesh(WALLS_MESH_OBJ, WALLS);
	walls = g_point_count;
	std::cout << "Loading Level 1" << std::endl;
	loadMapFromImage(LEVEL_1);
	

	// music was found on new grounds:
	// http://www.newgrounds.com/audio/listen/657490
	LPCSTR a = "open ../Music/music.mp3 type mpegvideo";
	mciSendString(a, NULL, 0, 0);
	LPCSTR b = "play ../Music/music.mp3 repeat";
	int error2 = mciSendString(b, NULL, 0, 0);
}

int main(int argc, char** argv) {
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
