#include <Engine/ShaderRepository.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Engine/Misc/Types.hpp>

namespace Engine {
    ShaderRepository::ShaderRepository() {};

    unsigned int ShaderRepository::getShaderProgram(std::string label) {
        if (this->shaderMap.find(label) != this->shaderMap.end()) {
            return this->shaderMap[label];
        }

        throw std::invalid_argument("No such shader program as \"" + label + "\"");
    };

    std::string ShaderRepository::loadFile(const std::string &path) {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error(std::string("Could not open shader file: ") + path);

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    };

    unsigned int ShaderRepository::compileShader(const std::string& source, GLenum type) {
        unsigned int shader = glCreateShader(type);

        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compilation error: " << infoLog << std::endl;
            throw std::runtime_error("Shader compilation failed!");
        }

        return shader;
    };



    unsigned int ShaderRepository::loadShaderProgramAs(
        std::string label,
        std::string vertexCodePath, 
        std::string fragmentCodePath
    ) {
        std::string vertexCode = loadFile(vertexCodePath);
        std::string fragmentCode = loadFile(fragmentCodePath);

        unsigned int vertex = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        unsigned int fragment = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

        unsigned int ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        int success;
        char infoLog[512];
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cerr << "Linking error: " << infoLog << std::endl;
            throw std::runtime_error("Shader linking failed!");
        }

        this->shaderMap[label] = ID;

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        this->locationCache[ID] = std::unordered_map<std::string, GLint>();
        return ID;
    };

    void ShaderRepository::useShaderWithDataByLabel(
        const std::string& label,
        const std::unordered_map<std::string, float>& floatVariables,
        const std::unordered_map<std::string, glm::vec2>& vec2Variables
    ) {
        unsigned int shaderID = this->getShaderProgram(label);
        this->useShaderWithDataByID(shaderID, floatVariables, vec2Variables);
    };

    void ShaderRepository::useShaderWithDataByID(
        unsigned int shaderID,
        const std::unordered_map<std::string, float>& floatVariables,
        const std::unordered_map<std::string, glm::vec2>& vec2Variables
    ) {
        if (this->lastLoadedShader != shaderID) glUseProgram(shaderID);
        this->lastLoadedShader = shaderID;

        for (auto variable : floatVariables) {
            glUniform1f(
                glGetUniformLocation(shaderID, variable.first.c_str()), 
                variable.second
            );
        }

        for (auto variable : vec2Variables) {
            glUniform2f(
                glGetUniformLocation(shaderID, variable.first.c_str()), 
                variable.second.x, 
                variable.second.y
            );
        }
    };

    void ShaderRepository::assignShaderIDToGameObjectID(unsigned int shaderID, EngineID gameObjectID) {
        this->objectShaderMap[gameObjectID] = shaderID;
        this->shaderToObjectIDsMap[shaderID].push_back(gameObjectID);
    };

    unsigned int ShaderRepository::getShaderProgramByGameObject(EngineID gameObjectID) {
        return this->objectShaderMap[gameObjectID];
    };

    void ShaderRepository::setUniformMat4(const std::string &name, const glm::mat4 &mat) {
        glUniformMatrix4fv(
            this->getUnfiromLocation(name), 
            1,
            GL_FALSE,
            &mat[0][0]
        );
    };

    void ShaderRepository::setUniformVec3(const std::string &name, const glm::vec3 &vec) {
        glUniform3fv(
            this->getUnfiromLocation(name), 
            1,
            &vec[0]
        );
    };

    GLint ShaderRepository::getUnfiromLocation(const std::string &name) {
        static int cacheMiss = 0;
        if (this->locationCache[this->lastLoadedShader].contains(name)) {
            return this->locationCache[this->lastLoadedShader][name];
        }
        cacheMiss++;
        GLint location = glGetUniformLocation(this->lastLoadedShader, name.c_str());
        this->locationCache[this->lastLoadedShader][name] = location;
        return location;
    };

    ShaderRepository shaderRepository = ShaderRepository();
};