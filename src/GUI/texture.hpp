#ifndef TEXTURE_CLASS_HPP
#define TEXTURE_CLASS_HPP

#include <glad/glad.h>
#include "shader.hpp"
#include "stb/stb_image.h"

/**
 * @brief Manages and encapsulates an OpenGL texture object.
 * * Handles loading image data using stb_image, generating the texture object on the GPU,
 * setting parameters, and managing texture binding.
 */
class Texture
{
public:
    GLuint ID;///< The unique OpenGL ID for the texture object.
    GLenum type;///< The type of the texture (e.g., GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP).

    /**
     * @brief Constructs and loads a texture from a file.
     * * * Generates the texture object, loads image data using stb_image, sets wrapping and filtering parameters,
     * uploads the data to the GPU, and generates mipmaps.
     * @param filepath The path to the image file to load.
     * @param texType The type of texture (e.g., GL_TEXTURE_2D).
     * @param slot The texture unit to activate and bind the texture to (e.g., GL_TEXTURE0).
     * @param format The format of the source image data (e.g., GL_RGB, GL_RGBA).
     * @param pixelType The data type of the pixel data (e.g., GL_UNSIGNED_BYTE).
     */
    Texture(const char* filepath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    /** @brief Default constructor. */
    Texture() = default;

    /**
     * @brief Sets the texture unit uniform in a shader program.
     * * * Uses the provided shader to set an integer uniform variable to the specified texture unit number.
     * @param shader The shader program to modify.
     * @param uniform The name of the sampler uniform variable in the shader (e.g., "tex").
     * @param unit The texture unit number (e.g., 0 for GL_TEXTURE0).
     */
    void TexUnit(Shader shader, const char* uniform, GLuint unit);

    /**
     * @brief Binds the texture object to its specified texture target type.
     */
    void Bind();

    /**
     * @brief Unbinds the texture object from its specified texture target type.
     */
    void Unbind();

    /**
     * @brief Deletes the OpenGL texture object from the GPU.
     */
    void DelTexture();
};

#endif