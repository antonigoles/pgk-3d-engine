#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <glad/glad.h>
#include <Engine/Misc/Types.hpp>

namespace Engine {
    class ShaderRepository {
    private:
        unsigned int lastLoadedShader = -1;

        std::unordered_map<std::string, int> shaderMap;

        std::unordered_map<EngineID, unsigned int> objectShaderMap;

        std::unordered_map<GLint, std::unordered_map<std::string, GLint>> locationCache;

        unsigned int compileShader(const std::string& source, GLenum type);
        std::string loadFile(const std::string &path);

        GLint getUnfiromLocation(const std::string &name);
    public:
        std::unordered_map<unsigned int, std::vector<EngineID>> shaderToObjectIDsMap;

        ShaderRepository();

        unsigned int getShaderProgram(std::string label);

        void assignShaderIDToGameObjectID(unsigned int shaderID, EngineID gameObjectID);

        unsigned int getShaderProgramByGameObject(EngineID gameObjectID);

        unsigned int loadShaderProgramAs(
            std::string label,
            std::string vertexCodePath, 
            std::string fragmentCodePath
        );

        void useShaderWithDataByLabel(
            const std::string& label,
            const std::unordered_map<std::string, float>& floatVariables,
            const std::unordered_map<std::string, glm::vec2>& vec2Variables
        );

        void useShaderWithDataByID(
            unsigned int shaderID,
            const std::unordered_map<std::string, float>& floatVariables,
            const std::unordered_map<std::string, glm::vec2>& vec2Variables
        );

        void setUniformMat4(const std::string &name, const glm::mat4 &mat);
        void setUniformVec3(const std::string &name, const glm::vec3 &vec);
    };

    extern ShaderRepository shaderRepository;
};