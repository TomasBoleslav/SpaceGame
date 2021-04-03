#include "Shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    unsigned int vertexShader = compileShader(readFile(vertexPath), GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(readFile(fragmentPath), GL_FRAGMENT_SHADER);
    m_ID = linkProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::use() const
{
    glUseProgram(m_ID);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), v0, v1, v2, v3);
}

std::string Shader::readFile(const std::string& path) const
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "Could not read file " << path << std::endl;
        return ""; // TODO: throw
    }
}

unsigned int Shader::compileShader(const std::string& shaderSource, unsigned int shaderType) const
{
    const char* c_str = shaderSource.c_str();
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);
    checkShaderCompileErrors(shaderType);
    return shader;
}

unsigned int Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) const
{
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLinkingErrors(shaderProgram);
    return shaderProgram;
}

void Shader::checkShaderCompileErrors(unsigned int shader) const
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[c_LogSize];
        glGetShaderInfoLog(shader, c_LogSize, nullptr, infoLog);
        std::cerr
            << "Failed to compile " << getShaderType(shader) << " shader:" << std::endl
            << infoLog << std::endl;
        // TODO: throw
    }
}

void Shader::checkProgramLinkingErrors(unsigned int program) const
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[c_LogSize];
        glGetProgramInfoLog(program, c_LogSize, nullptr, infoLog);
        std::cerr
            << "Failed to link program:" << std::endl 
            << infoLog << std::endl;
        // TODO: throw
    }
}

std::string Shader::getShaderType(unsigned int shaderType) const
{
    switch (shaderType)
    {
    case GL_VERTEX_SHADER: return "vertex"; break;
    case GL_FRAGMENT_SHADER: return "fragment"; break;
    }
    return "";
}