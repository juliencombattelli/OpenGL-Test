#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include "Mesh.hpp"
#include "Shader.hpp"

#include "MathUtilities.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const std::string& filename, const std::string& directory, bool gamma = false);

class Model
{
public:

    Model(const std::string& path, bool gamma = false) :
            m_gammaCorrection(gamma)
    {
        loadModel(path);
    }

    void draw(Shader& shader)
    {
        for (unsigned int i = 0; i < m_meshes.size(); i++)
            m_meshes[i].Draw(shader);
    }

private:

    void loadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
        {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
            m_meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));

        for (unsigned int i = 0; i < node->mNumChildren; i++)
            processNode(node->mChildren[i], scene);
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        Material material;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

            vertex.position = mesh->HasPositions() ? math::to_glmvec3(mesh->mVertices[i]) : glm::vec3 { 0.0f, 0.0f, 0.0f };
            vertex.normal = mesh->HasNormals() ? math::to_glmvec3(mesh->mNormals[i]) : glm::vec3 { 0.0f, 0.0f, 0.0f };
            vertex.texCoord = mesh->HasTextureCoords(0) ? math::to_glmvec2(mesh->mTextureCoords[0][i]) : glm::vec2 { 0.0f, 0.0f };
            vertex.tangent = mesh->HasTangentsAndBitangents() ? math::to_glmvec3(mesh->mTangents[i]) : glm::vec3 { 0.0f, 0.0f, 0.0f };
            vertex.bitangent = mesh->HasTangentsAndBitangents() ? math::to_glmvec3(mesh->mBitangents[i]) : glm::vec3 { 0.0f, 0.0f, 0.0f };

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(mat, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(mat, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        aiColor3D clr;

        mat->Get(AI_MATKEY_COLOR_AMBIENT, clr);
        material.ambient = math::to_glmvec3(clr);

        mat->Get(AI_MATKEY_COLOR_DIFFUSE, clr);
        material.diffuse = math::to_glmvec3(clr);

        mat->Get(AI_MATKEY_COLOR_SPECULAR, clr);
        material.specular = math::to_glmvec3(clr);

        mat->Get(AI_MATKEY_SHININESS, material.shininess);

        return Mesh(vertices, indices, textures, material);
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for (unsigned int j = 0; j < m_textures_loaded.size(); j++)
            {
                if (std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(m_textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), m_directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                m_textures_loaded.push_back(texture);
            }
        }
        return textures;
    }

    std::vector<Texture> m_textures_loaded;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    bool m_gammaCorrection;
};

unsigned int TextureFromFile(const std::string& filename, const std::string& directory, bool gamma)
{
    std::string fullFilename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(fullFilename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        std::cerr << "ERROR::STB_IMAGE : Texture failed to load at path " << filename << std::endl;

    stbi_image_free(data);

    return textureID;
}
#endif

