#include "texture.hpp"

Texture::Texture(const char* filepath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    type = texType;
    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_set_flip_vertically_on_load(true);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(texType, 0, GL_RGBA, width, height, 0, format, pixelType, data);
        glGenerateMipmap(texType);
    }
    else
    {
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);
}

void Texture::TexUnit(Shader shader, const char* uniform, GLuint unit) {
    shader.Use();
    shader.SetInt(uniform, unit);
}

void Texture::Bind() {
    glBindTexture(type, ID);
}

void Texture::Unbind() {
    glBindTexture(type, 0);
}

void Texture::DelTexture() {
    glDeleteTextures(1, &ID);
}