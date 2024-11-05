#include "Shader.h"

#include <array>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;

  std::ifstream vShaderFile(vertexPath);
  std::ifstream fShaderFile(fragmentPath);

  std::stringstream vShaderStream;
  std::stringstream fShaderStream;

  unsigned int vertex;
  unsigned int fragment;
  int success;
  std::array<char, 512> infoLog;

  // Read shader files
  vShaderStream << vShaderFile.rdbuf();
  fShaderStream << fShaderFile.rdbuf();
  vertexCode = vShaderStream.str();
  fragmentCode = fShaderStream.str();

  vShaderFile.close();
  fShaderFile.close();

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // Compile Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, nullptr);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertex, 512, nullptr, infoLog.data());
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog.data() << "\n";
  }

  // Compile Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, nullptr);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragment, 512, nullptr, infoLog.data());
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog.data() << "\n";
  }

  // Link shaders into a program
  m_ID = glCreateProgram();

  glAttachShader(m_ID, vertex);
  glAttachShader(m_ID, fragment);
  glLinkProgram(m_ID);
  glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(m_ID, 512, nullptr, infoLog.data());
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog.data() << "\n";
  }

  // Clean up shaders as they're linked into the program
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() {
  glDeleteProgram(m_ID);
}

void Shader::Use() const {
  glUseProgram(m_ID);
}

unsigned int Shader::GetID() const {
  return m_ID;
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
  glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(mat));
}
