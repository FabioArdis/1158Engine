#pragma once
#include <glad/glad.h>
#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

enum class MeshType : std::uint8_t { Cube, Plane, Capsule, Custom, Count };

constexpr std::array<std::string_view, 4> MeshTypeNames = {"Cube", "Plane",
                                                           "Capsule", "Custom"};

/**
 * @class Mesh
 * @brief Represents a 3D mesh that can be rendered using OpenGL.
 * 
 * The Mesh class encapsulates the data and methods necessary to create and render 
 * 3D geometries such as cubes and planes. It manages vertex array objects (VAO),
 * vertex buffer objects (VBO), and element buffer objects (EBO).
 */
class Mesh {
 public:
  /**
	* @brief Default constructor for the Mesh class.
	*/
  Mesh();

  /**
	* @brief Destructor for the Mesh class.
	* 
	* Cleans up OpenGL resources associated with the mesh.
	*/
  ~Mesh();

  /**
	* @brief Constructs a Mesh of the specified type.
	* 
	* @param type The type of mesh to create (Cube or Plane).
	*/
  Mesh(MeshType type);

  /**
	* @brief Creates a cube mesh.
	* 
	* This method defines the vertices and indices for a cube and initializes 
	* the OpenGL buffers accordingly.
	*/
  void CreateCube();

  /**
	* @brief Creates a plane mesh.
	* 
	* This method defines the vertices and indices for a plane and initializes 
	* the OpenGL buffers accordingly.
	*/
  void CreatePlane();

  void CreateCapsule(float radius, float height, int segments, int rings);

  /**
	* @brief Renders the mesh.
	* 
	* Binds the appropriate VAO and draws the mesh using the stored indices.
	*/
  void Draw();

  /**
	* @brief Clears the mesh data.
	* 
	* Deletes the VAO, VBO, and EBO from the GPU and clears the vertex and 
	* index vectors.
	*/
  void Clear();

  /**
	* @brief Retrieves the VAO associated with the mesh.
	* 
	* @return The VAO identifier.
	*/
  [[nodiscard]] unsigned int GetVAO() const { return m_VAO; }

  /**
	* @brief Retrieves the count of indices used in the mesh.
	* 
	* @return The number of indices as an unsigned int.
	*/
  [[nodiscard]] unsigned int GetIndexCount() const {
    return static_cast<unsigned int>(m_indices.size());
  }

  [[nodiscard]] MeshType GetType() const;

 private:
  unsigned int m_VAO; /**Vertex Array Object identifier. */
  unsigned int m_VBO; /**Vertex Buffer Object identifier. */
  unsigned int m_EBO; /**Element Buffer Object identifier. */

  /**Array of vertex data (positions, normals, etc.). */
  std::vector<float> m_vertices;
  /**Array of indices for indexed drawing. */
  std::vector<unsigned int> m_indices;

  MeshType m_meshType;
};
