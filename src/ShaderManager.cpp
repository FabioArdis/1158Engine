#include "ShaderManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

ShaderManager::ShaderManager() = default;

ShaderManager::~ShaderManager() {
  m_shaders.clear();
}

bool ShaderManager::LoadShader(const std::string& name, const char* vertexPath,
                               const char* fragmentPath) {
  try {
    auto shader = std::make_unique<Shader>(vertexPath, fragmentPath);
    m_shaders[name] = std::move(shader);  // Store shader in map
    return true;
  } catch (std::exception& e) {
    std::cerr << "Failed to load shader '" << name << "': " << e.what() << "\n";
    return false;
  }
}

unsigned int ShaderManager::GetShaderID(const std::string& name) {
  auto it = m_shaders.find(name);
  if (it != m_shaders.end()) {
    return it->second->ID;  // Return shader ID
  } else {
    std::cerr << "Shader not found: " << name << "\n";
    return 0;
  }
}

void ShaderManager::UseShader(const std::string& name) {
  // std::cout << "shader " << name << " successfully selected." << std::endl;
  // Will reuse this into a Log dock

  auto it = m_shaders.find(name);
  if (it != m_shaders.end()) {
    if (m_currentShader != it->second) {
      m_currentShader = it->second;
      m_currentShader->Use();  // Activate shader
    }
  } else {
    std::cerr << "Shader '" << name << "' not found." << "\n";
  }
}

void ShaderManager::SetBool(const std::string& name, bool value) {
  glUniform1i(GetUniformLocation(name), static_cast<int>(value));
}

void ShaderManager::SetInt(const std::string& name, int value) {
  glUniform1i(GetUniformLocation(name), value);
}

void ShaderManager::SetFloat(const std::string& name, float value) {
  glUniform1f(GetUniformLocation(name), value);
}

void ShaderManager::SetVector3(const std::string& name,
                               const glm::vec3& value) {
  glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderManager::SetMatrix4(const std::string& name,
                               const glm::mat4& value) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,
                     glm::value_ptr(value));
}

unsigned int ShaderManager::GetUniformLocation(const std::string& name) {
  if (m_currentShader != nullptr) {
    return glGetUniformLocation(m_currentShader->ID, name.c_str());
  } else {
    std::cerr << "No shader currently in use." << "\n";
    return -1;
  }
}

void ShaderManager::Shader::Use() const {
  glUseProgram(ID);
}

ShaderManager::Shader::Shader(const char* vertexPath,
                              const char* fragmentPath) {
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();    // Store vertex code
    fragmentCode = fShaderStream.str();  // Store fragment code
  } catch (std::ifstream::failure& e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
              << "\n";
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // Compile shaders
  unsigned int vertex, fragment;

  // Vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  // Shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  // Cleanup
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void ShaderManager::Shader::checkCompileErrors(unsigned int shader,
                                               const std::string& type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog
                << "\n -- --------------------------------------------------- "
                << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog
                << "\n -- --------------------------------------------------- "
                << std::endl;
    }
  }
}