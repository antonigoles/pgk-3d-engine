#include <Engine/MeshRepository.hpp>
#include <unordered_map>
#include <Engine/Misc/Types.hpp>
#include <Engine/GameObject.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include "glm/gtx/hash.hpp"
#include <Engine/Math/Math.hpp>
#include <glad/glad.h>


namespace Engine 
{
    MeshRepository::MeshRepository() {};
 
    EngineID MeshRepository::getNextMeshIndex() {
        return this->meshCounter++;
    };

    EngineID MeshRepository::loadMeshFromOBJFileWithPreprocessing(const std::string &path, glm::quat rotation) {
        // TODO: Either implement or use Assimp
        return {};
    };

    EngineID MeshRepository::loadMeshFromOBJFile(const std::string &path) {
        return this->loadMeshFromOBJFileWithPreprocessing(path, {0.0f, 1.0f, 0.0f, 0.0f});
    };

    EngineID MeshRepository::loadMeshFromFileWithPreprocessing(const std::string &path, glm::quat rotation) {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error(std::string("Could not open mesh file: ") + path);

        std::string line;

        std::vector<glm::vec3> vectors;
        Mesh mesh;

        while (getline (file, line)) {
            if (line.size() < 0) continue;
            if (line[0] == 'v') {
                float x, y, z;
                std::vector<std::string> ns = {};
                std::string parsed = "";
                for (int i = 1; i<line.size(); i++) {
                    if (line[i] == ' ') {
                        if (parsed == "") continue;
                        ns.push_back(parsed);
                        parsed = "";
                    } else {
                        parsed += line[i];
                    }
                }
                if (parsed != "") ns.push_back(parsed);
                try {
                    x = std::stof(ns[0]);
                    y = std::stof(ns[1]);
                    z = std::stof(ns[2]);
                }
                catch(const std::exception& e) {
                    std::cerr << "Error while reading Mesh:" << e.what() << '\n';
                    return -1;
                }
                vectors.push_back({x,y,z});
            }

            if (line[0] == 't') {
                int x, y, z;
                std::vector<std::string> ns = {};
                std::string parsed = "";
                for (int i = 1; i<line.size(); i++) {
                    if (line[i] == ' ') {
                        if (parsed == "") continue;
                        ns.push_back(parsed);
                        parsed = "";
                    } else {
                        parsed += line[i];
                    }
                }
                if (parsed != "") ns.push_back(parsed);
                try {
                    x = std::stoi(ns[0]);
                    y = std::stoi(ns[1]);
                    z = std::stoi(ns[2]);
                }
                catch(const std::exception& e) {
                    std::cerr << "Error while reading Mesh:" << e.what() << '\n';
                    return -1;
                }
                mesh.push_back(vectors[x].x);
                mesh.push_back(vectors[x].y);
                mesh.push_back(vectors[x].z);

                mesh.push_back(vectors[y].x);
                mesh.push_back(vectors[y].y);
                mesh.push_back(vectors[y].z);

                mesh.push_back(vectors[z].x);
                mesh.push_back(vectors[z].y);
                mesh.push_back(vectors[z].z);
            }
        }
        
        file.close();
        Engine::Math::meshRotate(mesh, rotation);
        return this->loadMesh(mesh);
    };  

    EngineID MeshRepository::loadMeshFromFile(const std::string &path) {
        return this->loadMeshFromFileWithPreprocessing(path, {0.0f, 1.0f, 0.0f, 0.0f});
    };

    EngineID MeshRepository::loadMeshWithNormals(const Mesh nmesh) {
        EngineID engineID = this->getNextMeshIndex();
        this->meshMap[engineID] = nmesh;

        // Build VAO
        EngineID meshVAO, meshVBO;
        glGenVertexArrays(1, &meshVAO);
        glGenBuffers(1, &meshVBO);

        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nmesh.size(), nmesh.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);  
        glBindVertexArray(0);
        
        this->VAOMap[engineID] = meshVAO;
        return engineID;
    }

    
    EngineID MeshRepository::loadMesh(const Mesh nnmesh) {
        EngineID engineID = this->getNextMeshIndex();
        // calculate normals
        Mesh nmesh;
        std::unordered_map<glm::vec3, glm::vec3> normals;
        
        for (int i = 0; i<nnmesh.size(); i+=9) {
            glm::vec3 v1{nnmesh[i], nnmesh[i+1], nnmesh[i+2]};
            glm::vec3 v2{nnmesh[i+3], nnmesh[i+4], nnmesh[i+5]};
            glm::vec3 v3{nnmesh[i+6], nnmesh[i+7], nnmesh[i+8]};
            glm::vec3 normal = glm::cross(v3 - v1, v2 - v1);
            normal = glm::normalize(normal);
            
            if (!normals.contains(v1)) normals[v1] = glm::vec3{0,0,0};
            if (!normals.contains(v2)) normals[v2] = glm::vec3{0,0,0};
            if (!normals.contains(v3)) normals[v3] = glm::vec3{0,0,0};

            normals[v1] += normal;
            normals[v2] += normal;
            normals[v3] += normal;
        }

        for (int i = 0; i<nnmesh.size(); i+=9) {
            glm::vec3 v1{nnmesh[i], nnmesh[i+1], nnmesh[i+2]};
            glm::vec3 v2{nnmesh[i+3], nnmesh[i+4], nnmesh[i+5]};
            glm::vec3 v3{nnmesh[i+6], nnmesh[i+7], nnmesh[i+8]};

            normals[v1] = glm::normalize(v1);
            normals[v2] = glm::normalize(v2);
            normals[v3] = glm::normalize(v3);

            nmesh.insert(nmesh.begin(), {
                v1.x, v1.y, v1.z, normals[v1].x, normals[v1].y, normals[v1].z,
                v2.x, v2.y, v2.z, normals[v2].x, normals[v2].y, normals[v2].z,
                v3.x, v3.y, v3.z, normals[v3].x, normals[v3].y, normals[v3].z
            });
        }

        this->meshMap[engineID] = nmesh;

        // Build VAO
        EngineID meshVAO, meshVBO;
        glGenVertexArrays(1, &meshVAO);
        glGenBuffers(1, &meshVBO);

        glBindVertexArray(meshVAO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nmesh.size(), nmesh.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);  
        glBindVertexArray(0);
        
        this->VAOMap[engineID] = meshVAO;
        return engineID;
    };
    
    void MeshRepository::assignMesh(GameObject* gameObject, EngineID meshId) {
        this->objectMap[gameObject] = meshId;
        this->meshToGameObjects[meshId].push_back(gameObject);
    };

    unsigned int MeshRepository::getMeshVAO(EngineID meshId) {
        return this->VAOMap[meshId];
    }; 

    size_t MeshRepository::getMeshSize(EngineID meshId) {
        return this->meshMap[meshId].size() / 3;
    };

    EngineID MeshRepository::getMeshIDByGameObject(GameObject *gameObject) {
        return this->objectMap[gameObject];
    }

    std::pair<glm::vec3, glm::vec3> MeshRepository::getMeshCuboid(EngineID mesh, float padding) {
        float maxX = std::numeric_limits<float>::min();
        float maxY = std::numeric_limits<float>::min();
        float maxZ = std::numeric_limits<float>::min();

        float minX = std::numeric_limits<float>::max();
        float minY = std::numeric_limits<float>::max();
        float minZ = std::numeric_limits<float>::max();

        std::vector<float>& meshRef = this->meshMap[mesh];
        
        for (int i = 0; i<meshRef.size(); i+=6) {
            float x = meshRef[i];
            float y = meshRef[i+1];
            float z = meshRef[i+2];
            
            maxX = std::max(x, maxX);
            maxY = std::max(x, maxY);
            maxZ = std::max(x, maxZ);

            minX = std::min(x, minX);
            minY = std::min(x, minY);
            minZ = std::min(x, minZ);
        }

        return {{minX - padding, minY - padding, minZ - padding}, {maxX + padding, maxY + padding, maxZ + padding}};
    };

    MeshRepository meshRepository = MeshRepository();
};

