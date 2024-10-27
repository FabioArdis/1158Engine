#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

/**
 * @class Shader
 * @brief Manages the OpenGL shader program.
 * 
 * The Shader class is responsible for loading, compiling, and linking 
 * vertex and fragment shaders, as well as setting uniform variables 
 * for rendering.
 */
class Shader
{
public:
	/**
	* @brief Constructs a Shader object from the specified vertex and fragment shader paths.
	* 
	* This constructor reads shader source code from the specified file paths,
	* compiles the shaders, and links them into a program.
	* 
	* @param vertexPath Path to the vertex shader source file.
	* @param fragmentPath Path to the fragment shader source file.
	*/
	Shader(const char* vertexPath, const char* fragmentPath);
	
	/**
	* @brief Destructs the Shader object and frees resources.
	* 
	* Deletes the shader program from the GPU.
	*/
	~Shader();

	/**
	* @brief Activates the shader program for rendering.
	* 
	* This method must be called before using the shader to render objects.
	*/
	void Use();

	/**
	* @brief Retrieves the OpenGL ID of the shader program.
	* 
	* @return The OpenGL ID of the shader program.
	*/
	unsigned int GetID();

	/**
	* @brief Sets a 4x4 matrix uniform in the shader program.
	* 
	* @param name The name of the uniform variable in the shader.
	* @param mat The 4x4 matrix to set the uniform to.
	*/
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
	unsigned int m_ID;	/**The OpenGL ID of the shader program. */
};

