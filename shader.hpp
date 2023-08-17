#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class shaders
{
	public:
	unsigned int id1;
	//vertex/fragment
	shaders(const char* vPath, const char* fPath)
	{
		string vCode;
		string fCode;
		ifstream vShaderF;
		ifstream fShaderF;

		vShaderF.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderF.exceptions(ifstream::failbit | ifstream::badbit);
		try
		{
			vShaderF.open(vPath);
			fShaderF.open(fPath);
			stringstream vSS, fSS;
			vSS << vShaderF.rdbuf();
			fSS << fShaderF.rdbuf();
			vShaderF.close();
			fShaderF.close();
			vCode = vSS.str();
			fCode = fSS.str();
		}
		catch(ifstream::failure& e)
		{
			cout << "Shader File couldn't be read!!" << e.what() << endl;
		}

		const char* vShaderCode = vCode.c_str();
		const char* fShaderCode = fCode.c_str();

		unsigned int v;
		unsigned int f;

		v = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(v,1,&vShaderCode,NULL);
		glCompileShader(v);
		checkCompileErrors(v, "VERTEX");

		f = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(f,1,&fShaderCode,NULL);
		glCompileShader(f);
		checkCompileErrors(f,"FRAGMENT");

		id1 = glCreateProgram();
		glAttachShader(id1,v);
		glAttachShader(id1,f);
		glLinkProgram(id1);
		checkCompileErrors(id1,"PROGRAM");
		
		glDeleteShader(v);
		glDeleteShader(f);
	}
	
	void use()
	{
		glUseProgram(id1); 
	}
	
	void setBool(const string &name,bool value) const
	{
		glUniform1i(glGetUniformLocation(id1, name.c_str()), (int)value);
	}

	void setInt(const string &name,int value) const
	{
		glUniform1i(glGetUniformLocation(id1, name.c_str()), value);
	}

	void setFloat(const string &name,float value) const
	{
		glUniform1i(glGetUniformLocation(id1, name.c_str()), value);
	}

    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(id1, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(id1, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(id1, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(id1, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(id1, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(id1, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id1, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id1, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id1, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

	private:
	void checkCompileErrors(unsigned int shader, string type)
	{
		int success;
		char infoLog[1024];
		if(type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "Shader compilation error" << "||" << type << "||" << infoLog << endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if(!success)
			{
				cout << "error: progam linking error" << "||" << type << "||" << infoLog << endl;
			}
		}
	}
};