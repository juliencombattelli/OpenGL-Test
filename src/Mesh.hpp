#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Vertex
{
    enum Attrib : GLuint
    {
        AttribPosition = 0,
        AttribNormal = 1,
        AttribTexCoord = 2,
        AttribTangent = 3,
        AttribBitangent = 4,
    };

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture
{
    GLuint id;
    std::string type;
    std::string path;
};

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class Mesh
{
public:

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, const Material& material)
    {
        m_vertices = vertices;
        m_indices = indices;
        m_textures = textures;
        m_material = material;

        setupMesh();
    }

    ~Mesh()
    {
    	// FIXME: release OpenGL resources
    }

    void Draw(Shader& shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (std::size_t i = 0; i < m_textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = m_textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);

            shader.setInt((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
        }

        shader.setVec3("material.ambient", m_material.ambient);
        shader.setVec3("material.diffuse", m_material.diffuse);
        shader.setVec3("material.specular", m_material.specular);
        shader.setFloat("material.shininess", m_material.shininess);

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

private:

    void setupMesh()
    {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(Vertex::AttribPosition);
        glVertexAttribPointer(Vertex::AttribPosition, m_vertices[0].position.length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(Vertex::AttribNormal);
        glVertexAttribPointer(Vertex::AttribNormal, m_vertices[0].normal.length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(Vertex::AttribTexCoord);
        glVertexAttribPointer(Vertex::AttribTexCoord, m_vertices[0].texCoord.length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glEnableVertexAttribArray(Vertex::AttribTangent);
        glVertexAttribPointer(Vertex::AttribTangent, m_vertices[0].tangent.length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray(Vertex::AttribBitangent);
        glVertexAttribPointer(Vertex::AttribBitangent, m_vertices[0].bitangent.length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        glBindVertexArray(0);
    }

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
    Material m_material;
    GLuint m_vao, m_vbo, m_ebo;
};

#endif

