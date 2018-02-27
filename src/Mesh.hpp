#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    //glm::vec3 tangent;
    //glm::vec3 bitangent;
};

struct Texture
{
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh
{
public:

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
    GLuint m_vao;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
    {
        m_vertices = vertices;
        m_indices = indices;
        m_textures = textures;

        setupMesh();
    }

    void Draw(Shader shader)
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

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

private:

    GLuint vbo, ebo;

    void setupMesh()
    {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(Vertex::AttribPosition);
        glVertexAttribPointer(Vertex::AttribPosition, decltype(std::declval<Vertex>().position)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableVertexAttribArray(Vertex::AttribNormal);
        glVertexAttribPointer(Vertex::AttribNormal, decltype(std::declval<Vertex>().normal)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(Vertex::AttribTexCoord);
        glVertexAttribPointer(Vertex::AttribTexCoord, decltype(std::declval<Vertex>().texCoord)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        //glEnableVertexAttribArray(Vertex::AttribTangent);
        //glVertexAttribPointer(Vertex::AttribTangent, decltype(std::declval<Vertex>().tangent)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        //glEnableVertexAttribArray(Vertex::AttribBitangent);
        //glVertexAttribPointer(Vertex::AttribBiangent, decltype(std::declval<Vertex>().bitangent)::length(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        glBindVertexArray(0);
    }
};
#endif

