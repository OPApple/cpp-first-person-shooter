#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include <glad/glad.h>
#include <iostream>
#include <map>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader.hpp"
#include "gui_constants.hpp"

/**
 * @brief Stores texture and metric information for a single character glyph.
 */
struct Character {
    unsigned int TextureID;  ///< ID handle of the glyph texture (single-channel grayscale/red).
    glm::ivec2 Size;         ///< Size of glyph (width, height) in pixels.
    glm::ivec2 Bearing;      ///< Offset from baseline to left/top of glyph.
    unsigned int Advance;    ///< Horizontal offset to advance to next glyph.
};

// Base https://learnopengl.com/In-Practice/Text-Rendering

/**
 * @brief Handles loading fonts using FreeType and rendering text using OpenGL.
 * * This class pre-loads character glyphs into textures and uses a dynamic VBO
 * to efficiently render quads for each character.
 */
class TextRenderer {
    
public:
    /**
     * @brief Constructs and initializes the TextRenderer.
     * * * Sets up the VAO and VBO for rendering quads.
     * * Initializes FreeType, loads the specified font, sets pixel size (48x48),
     * and pre-loads the first 128 ASCII characters, storing their metrics and
     * texture IDs in the `Characters` map.
     * @param fontPath The file path to the TrueType Font (TTF) file. Defaults to "resources/fonts/ostrich-sans-black.ttf".
     */
    TextRenderer(std::string fontPath="resources/fonts/ostrich-sans-black.ttf") {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cout << "Could not init FreeType" << std::endl;
        }
        
        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            std::cout << "Failed to load font" << std::endl;
        }

        FT_Set_Pixel_Sizes(face, 48, 48);
        
        // disables byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "Failed to load Glyph: " << c << std::endl;
            }
            
            // Not using Texture bc we want to do some tomfoolery
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED, 
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                int(face->glyph->advance.x)
            };

            Characters.insert(std::pair<char, Character>(c, character));


        }

        // We love cleaning !!!!!
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    /**
     * @brief Renders a string of text to the screen.
     * * * Iterates over the characters, calculates the position and size of the quad for each glyph,
     * updates the VBO dynamically, and draws the textured quad.
     * @param text The string to be rendered.
     * @param x The starting X-coordinate (screen space).
     * @param y The starting Y-coordinate (screen space).
     * @param scale The scaling factor applied to the glyph size and advance.
     * @param color The color of the text (RGB).
     */
    void render(std::string text, float x, float y, float scale, glm::vec3 color) {
        shader.Use();
        shader.SetMat4("projection", projection);
        shader.SetVec3("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            Character ch = Characters[*c];

            float xPos = x + ch.Bearing.x * scale;
            float yPos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            float vertices[6][4] = {
                { xPos,     yPos + h,   0.0f, 1.0f },        
                { xPos,     yPos,       0.0f, 0.0f },
                { xPos + w, yPos,       1.0f, 0.0f },

                { xPos,     yPos + h,   0.0f, 1.0f },
                { xPos + w, yPos,       1.0f, 0.0f },
                { xPos + w, yPos + h,   1.0f, 1.0f }           
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
private:
    Shader shader = Shader("./resources/shaders/text.vs", "./resources/shaders/text.fs");  ///< Shader program used for text rendering.
    std::map<char, Character> Characters;                                                  ///< Map storing all loaded character glyphs and their metrics.
    glm::mat4 projection = glm::ortho(1.0f, (float)GUIConstants::game_width, 1.0f, (float)GUIConstants::game_height); ///< Orthographic projection matrix for screen-space rendering.
    unsigned int VAO, VBO;                                                                 ///< OpenGL Vertex Array Object and Vertex Buffer Object IDs.
};    

#endif