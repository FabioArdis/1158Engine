#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <string>
#include <unordered_map>

/**
 * @class ShaderManager
 * @brief Manages multiple OpenGL shader programs.
 * 
 * The ShaderManager class handles loading, compiling, linking, and 
 * using shaders within an OpenGL context. It allows for easy access 
 * and management of multiple shader programs by name.
 */
class ShaderManager {
 public:
  /**
	 * @brief Constructs a ShaderManager instance.
	 */
  ShaderManager();

  /**
	 * @brief Destructs the ShaderManager and frees associated resources.
	 * 
	 * Deletes all loaded shaders to avoid memory leaks.
	 */
  ~ShaderManager();

  /**
	 * @brief Loads a shader from the specified vertex and fragment paths.
	 * 
	 * @param name The name to associate with the loaded shader.
	 * @param vertexPath The file path to the vertex shader source code.
	 * @param fragmentPath The file path to the fragment shader source code.
	 * @return True if the shader was successfully loaded; otherwise, false.
	 */
  bool LoadShader(const std::string& name, const char* vertexPath,
                  const char* fragmentPath);

  /**
	 * @brief Activates the shader program associated with the given name.
	 * 
	 * @param name The name of the shader to activate.
	 */
  void UseShader(const std::string& name);

  /**
	 * @brief Sets a boolean uniform in the currently active shader.
	 * 
	 * @param name The name of the uniform variable in the shader.
	 * @param value The boolean value to set.
	 */
  void SetBool(const std::string& name, bool value);

  /**
	 * @brief Sets an integer uniform in the currently active shader.
	 * 
	 * @param name The name of the uniform variable in the shader.
	 * @param value The integer value to set.
	 */
  void SetInt(const std::string& name, int value);

  /**
	 * @brief Sets a float uniform in the currently active shader.
	 * 
	 * @param name The name of the uniform variable in the shader.
	 * @param value The float value to set.
	 */
  void SetFloat(const std::string& name, float value);

  /**
	 * @brief Sets a vec3 uniform in the currently active shader.
	 * 
	 * @param name The name of the uniform variable in the shader.
	 * @param value The glm::vec3 value to set.
	 */
  void SetVector3(const std::string& name, const glm::vec3& value);

  /**
	 * @brief Sets a 4x4 matrix uniform in the currently active shader.
	 * 
	 * @param name The name of the uniform variable in the shader.
	 * @param value The glm::mat4 value to set.
	 */
  void SetMatrix4(const std::string& name, const glm::mat4& value);

  /**
	 * @brief Retrieves the location of a uniform variable in the currently active shader.
	 * 
	 * @param name The name of the uniform variable in the shader.
	 * @return The uniform location or -1 if the shader is not in use.
	 */
  unsigned int GetUniformLocation(const std::string& name);

  /**
	 * @brief Retrieves the OpenGL ID of a shader by its name.
	 * 
	 * @param name The name of the shader.
	 * @return The OpenGL ID of the shader or 0 if not found.
	 */
  unsigned int GetShaderID(const std::string& name);

 private:
  /**
	 * @struct Shader
	 * @brief Represents an OpenGL shader program.
	 */
  struct Shader {
    unsigned int ID; /**The OpenGL ID of the shader program. */

    /**
		 * @brief Constructs a Shader object from vertex and fragment shader paths.
		 * 
		 * @param vertexPath The file path to the vertex shader source code.
		 * @param fragmentPath The file path to the fragment shader source code.
		 */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
		 * @brief Activates the shader program for use.
		 */
    void Use() const;

    /**
		 * @brief Checks for compilation and linking errors in the shader.
		 * 
		 * @param shader The shader or program ID to check.
		 * @param type The type of shader or program (e.g., "VERTEX", "FRAGMENT", "PROGRAM").
		 */
    static void checkCompileErrors(unsigned int shader, const std::string& type);
  };

  /**Map of shader names to Shader objects. */
  std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
  /**Pointer to the currently active shader. */
  std::shared_ptr<Shader> m_currentShader{nullptr};
};
