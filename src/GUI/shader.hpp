#ifndef SHADER_HPP
#define SHADER_HPP
#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

/**
 * @brief Manages and encapsulates an OpenGL shader program.
 * * This class handles reading shader source files, compiling and linking them into
 * a program, and providing utility functions for setting uniform variables.
 */
class Shader {
    public: 
        /** @brief The unique OpenGL ID for the compiled shader program. */
        unsigned int ID;

        /**
         * @brief Constructs and compiles a shader program from file paths.
         * * Reads, compiles, links, and checks for errors in the Vertex and Fragment shaders.
         * @param vertexPath Path to the Vertex Shader source file.
         * @param fragmentPath Path to the Fragment Shader source file.
         */
        Shader(const char* vertexPath, const char* fragmentPath);

        /** @brief Default constructor. */
        Shader() = default;

        /**
         * @brief Activates the shader program for rendering.
         */
        void Use();

        /**
         * @brief Deletes the shader program from the GPU.
         */
        void Del();

        /// Uniform Setter Methods
        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetVec2(const std::string &name, const glm::vec2 &value) const;
        void SetVec2(const std::string &name, float x, float y) const;
        void SetVec3(const std::string &name, const glm::vec3 &value) const;
        void SetVec3(const std::string &name, float x, float y, float z) const;
        void SetVec4(const std::string &name, const glm::vec4 &value) const;
        void SetVec4(const std::string &name, float x, float y, float z, float w) const;
        void SetMat2(const std::string &name, const glm::mat2 &mat) const;
        void SetMat3(const std::string &name, const glm::mat3 &mat) const;
        void SetMat4(const std::string &name, const glm::mat4 &mat) const;
    
    private: 
        /**
         * @brief Checks the compile/link status of a shader or program.
         * * Prints error messages to stdout if compilation or linking fails.
         * @param shader The ID of the shader object or program object.
         * @param type A string indicating the object type ("VERTEX", "FRAGMENT", or "PROGRAM").
         */
        void CheckCompileErrors(unsigned int shader, std::string type);
};

#endif