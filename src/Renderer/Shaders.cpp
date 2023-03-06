#include "Shaders.h"

Renderer::Shader::Shader(const std::string& resoursePath, const char* vertexName, const char* fragmentName, const char* geometryName)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream vShaderStream, fShaderStream, gShaderStream;
        
        vertexCode = GetShaderCode(resoursePath, vertexName, vShaderFile);
        fragmentCode = GetShaderCode(resoursePath, fragmentName, fShaderFile);
        if (geometryName != nullptr) {
            geometryCode = GetShaderCode(resoursePath, geometryName, gShaderFile);
        }
    }
    catch (std::ifstream::failure& e)
    {  
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }

    unsigned int vertex, fragment, geometry;
    ID = glCreateProgram();
    
    const char* vShaderCode = vertexCode.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glAttachShader(ID, vertex);
    
    const char* fShaderCode = fragmentCode.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glAttachShader(ID, fragment);

    if (geometryName != nullptr) {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryName != nullptr){
        glDeleteShader(geometry);
    }
}

void Renderer::Shader::Use()
{
    glUseProgram(ID);
}

void Renderer::Shader::SetUniform(std::string name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Renderer::Shader::SetUniform(std::string name, int data) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), data);
}

void Renderer::Shader::SetUniform(std::string name, unsigned int data) const {
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), data);
}

void Renderer::Shader::SetUniform(std::string name, float data) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), data);
}

void Renderer::Shader::SetUniform(std::string name, glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Renderer::Shader::SetUniform(std::string name, glm::vec2& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Renderer::Shader::SetUniform(std::string name, glm::mat4& data) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(data));
}

std::string Renderer::Shader::GetShaderCode(std::string resoursePath, const char* shaderName, std::ifstream& shaderFile)
{
    std::stringstream shaderStream;

    shaderFile.open(resoursePath + "shaders/" + shaderName);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}

