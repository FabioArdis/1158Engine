#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H


class ShaderManager;

/**
 * @struct Character
 * @brief Represents a single character's texture and properties.
 */
struct Character {
	unsigned int    TextureID;  /**ID handle of the glyph texture. */
	glm::ivec2      Size;       /**Size of glyph. */
	glm::ivec2      Bearing;    /**Offset from baseline to left/top of glyph. */
	unsigned int    Advance;    /**Offset to advance to next glyph. */
};

/**
 * @class TextRenderer
 * @brief Renders text using FreeType and OpenGL.
 * 
 * The TextRenderer class handles the initialization of font rendering 
 * using FreeType, manages character textures, and renders text 
 * onto the screen using OpenGL.
 */
class TextRenderer {
public:
	/**
	 * @brief Constructs a TextRenderer instance with a reference to ShaderManager.
	 * 
	 * @param shaderManager Pointer to the ShaderManager for shader handling.
	 */
	TextRenderer(ShaderManager* shaderManager);

	/**
	 * @brief Destructs the TextRenderer and frees associated resources.
	 * 
	 * Deletes the vertex array and buffer objects used for rendering text.
	 */
	~TextRenderer();

	/**
	 * @brief Initializes the text renderer with the specified font and size.
	 * 
	 * @param fontPath The file path to the font file.
	 * @param fontSize The size of the font to be rendered.
	 * @return True if the initialization was successful; otherwise, false.
	 */
	bool Initialize(const std::string& fontPath, unsigned int fontSize);

	/**
	 * @brief Renders the specified text at the given position and scale.
	 * 
	 * @param text The text string to render.
	 * @param x The x coordinate for the starting position of the text.
	 * @param y The y coordinate for the starting position of the text.
	 * @param scale The scale factor for the text size.
	 * @param color The color of the text in RGB format.
	 */
	void RenderText(const std::string& text, float x, float y, float scale, glm::vec3 color);

	/**
	 * @brief Sets the projection matrix for the text rendering.
	 * 
	 * @param projection The projection matrix to set.
	 */
	void SetProjection(const glm::mat4& projection);

private:
	ShaderManager* m_shaderManager;         /**Pointer to the ShaderManager instance. */
	std::map<char, Character> Characters;   /**Map of characters and their corresponding properties. */
	unsigned int VAO;                       /**Vertex Array Object ID.*/
	unsigned int VBO;                       /**Vertex Buffer Object ID.*/
	glm::mat4 m_projection;                 /**Projection matrix for rendering text. */};