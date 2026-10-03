#ifndef MESH_CLASS_HPP
#define MESH_CLASS_HPP

#include <glad/glad.h>
#include <vector>
#include "shader.hpp"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

/**
 * @brief Represents a single vertex in a 3D mesh.
 * * Contains the essential attributes needed for rendering: position and texture coordinates.
 */
struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
};


class Mesh
{
    public:
        /** @brief The OpenGL Vertex Array Object (VAO) ID. */
        GLuint VAO;
        
        /**
         * @brief Constructs a Mesh object, initializing OpenGL buffers.
         *  Uploads vertex and optional index data to the GPU and configures
         * the vertex attribute pointers for position and texture coordinates.
         * @param Vertices A vector of Vertex structures containing the geometry data.
         * @param Indices A vector of unsigned integers for indexed drawing. Defaults to empty.
         */
        Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices = {});

        /**
         * @brief Default constructor.
         */
        Mesh() = default;

        /**
         * @brief Renders the mesh geometry.
         * * Binds the VAO and calls glDrawElements (if indices are present) or 
         * glDrawArrays (otherwise) to render the triangles.
         */
        void Draw() const;

    private:
        std::vector<Vertex> vertices;         ///< Storage for the mesh's vertex data in system memory.
        std::vector<unsigned int> indices;    ///< Storage for the mesh's index data in system memory.
        GLuint VBO, EBO;                      ///< The OpenGL Buffer Object IDs (Vertex and Element).
};


#endif

