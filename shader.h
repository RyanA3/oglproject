#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::string;
using std::ifstream;
using std::cout;
using std::endl;

class Shader {

public:
	//Shader program id
	unsigned int ID;  

	//Constructor reads and builds shaders from respective files
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {

		string vertexShaderCode;
		string fragmentShaderCode;
		ifstream vShaderFile;
		ifstream fShaderFile;

		//Ensure the file streams can throw exceptions
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

		try {
			//Open the files
			vShaderFile.open(vertexShaderPath);
			fShaderFile.open(fragmentShaderPath);
			std::stringstream vShaderStream, fShaderStream;

			//Read each file's buffer contents into respective string streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//Close each file
			vShaderFile.close();
			fShaderFile.close();

			//Convert string streams into strings
			vertexShaderCode = vShaderStream.str();
			fragmentShaderCode = fShaderStream.str();
		} 
		catch (ifstream::failure e) {
			cout << "ERROR::SHADER::FILE_UNSUCCESSFUL_READ" << endl;
		}

		const char* vShaderCode = vertexShaderCode.c_str();
		const char* fShaderCode = fragmentShaderCode.c_str();


		//Compile the shaders (vertex & fragment)
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER); //Generate the shader
		glShaderSource(vertex, 1, &vShaderCode, NULL); //Attach source code
		glCompileShader(vertex); //Compile

		//Check if compilation succeeded
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER); //Generate the shader
		glShaderSource(fragment, 1, &fShaderCode, NULL); //Attach source code
		glCompileShader(fragment); //Compile

		//Check if compilation succeeded
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}


		//Create a shader program to link the two shaders together
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		//Check if the shader program linked properly
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}


		//Delete shader objects since they are now linked into the program and no longer individually needed
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};

	//Activate the shader
	void use() {
		glUseProgram(ID);
	};

	//Uniform functions for easy use
	void setBool(const string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
	};
	void setInt(const string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	};
	void setFloat(const string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	};

};

#endif