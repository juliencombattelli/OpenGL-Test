#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:

    enum Type
    {
        Vertex      = GL_VERTEX_SHADER,
        Fragment    = GL_FRAGMENT_SHADER,
        Geometry    = GL_GEOMETRY_SHADER
    };

    Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& geometryShaderFilename = "")
    {
        auto vertexShaderId = compile(vertexShaderFilename, Type::Vertex);
        checkCompileErrors(vertexShaderId, "VERTEX");

        auto fragmentShaderId = compile(fragmentShaderFilename, Type::Fragment);
        checkCompileErrors(fragmentShaderId, "FRAGMENT");

        m_programId = glCreateProgram();
        glAttachShader(m_programId, vertexShaderId);
        glAttachShader(m_programId, fragmentShaderId);

        glLinkProgram(m_programId);
        checkCompileErrors(m_programId, "PROGRAM");

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    ~Shader()
    {
        glDeleteProgram(m_programId);
    }

    void use()
    {
        glUseProgram(m_programId);
    }

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:

    GLuint compile(const std::string& shaderFilename, Shader::Type type)
    {
        std::ifstream shaderFile;
        std::string shaderCode;

        try
        {
            shaderFile.open(shaderFilename);
            shaderCode.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
            shaderFile.close();
        }
        catch(std::exception& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ : " << e.what() << std::endl;
        }

        GLuint shaderId = glCreateShader(type);
        auto shaderCodePtr = shaderCode.c_str();
        glShaderSource(shaderId, 1, &shaderCodePtr, nullptr);
        glCompileShader(shaderId);

        return shaderId;
    }

    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    GLuint m_programId;
};

#endif
