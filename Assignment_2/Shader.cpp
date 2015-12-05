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
#pragma warning(disable:4996)

class Shader {
public:
	GLuint shaderProgramID;
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


	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
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
};