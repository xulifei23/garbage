#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

using namespace std;

Shader::Shader(const char* vertex, const char* frag)
{
	string vertexCode, fragCode;
	ifstream vertexf, fragf;

	vertexf.exceptions(ifstream::failbit | ifstream::badbit);
	fragf.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		vertexf.open(vertex);
		fragf.open(frag);
		stringstream vertexS, fragS;
		vertexS << vertexf.rdbuf();
		fragS << fragf.rdbuf();

		vertexf.close();
		fragf.close();

		vertexCode = vertexS.str();
		fragCode = fragS.str();
	}
	catch (const ifstream::failure& e)
	{
		(void)e;
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
	}

	const char* verC = vertexCode.c_str();
	const char* fragC = fragCode.c_str();

	unsigned int verShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(verShader, 1, &verC, nullptr);
	glShaderSource(fragShader, 1, &fragC, nullptr);

	int success = GL_FALSE;
	char info[512] = { 0 };

	glCompileShader(verShader);
	glGetShaderiv(verShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(verShader, 512, nullptr, info);
		cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED" << endl << info << endl;
	}

	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, nullptr, info);
		cout << "ERROR::SHADER::FRAGMENT::COMPILE_FAILED" << endl << info << endl;
	}

	id = glCreateProgram();
	glAttachShader(id, verShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, nullptr, info);
		cout << "ERROR::PROGRAM::LINK_FAILED" << endl << info << endl;
	}

	glDeleteShader(verShader);
	glDeleteShader(fragShader);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::setBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const 
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}


