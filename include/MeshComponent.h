#pragma once
#include "Component.h"
#include "Mesh.h"

/**
 * @class MeshComponent
 * @brief Represents a component that manages a mesh for a GameObject.
 * 
 * The MeshComponent class is responsible for holding and rendering 
 * a Mesh instance associated with a GameObject. It inherits from 
 * the base Component class.
 */
class MeshComponent : public Component {
 public:
  /**
	* @brief Constructs a MeshComponent with a specified owner and mesh.
	* 
	* @param owner The GameObject that owns this component.
	* @param mesh The mesh to be associated with this component.
	*/
  MeshComponent(std::shared_ptr<GameObject> owner, std::shared_ptr<Mesh> mesh)
      : Component(owner), m_mesh(mesh) {}

  /**
	* @brief Default constructor for the MeshComponent.
	* 
	* Initializes a MeshComponent with a default cube mesh.
	* 
	* @param owner The GameObject that owns this component.
	*/
  MeshComponent(std::shared_ptr<GameObject> owner) : Component(owner) {
    m_mesh = std::make_shared<Mesh>(MeshType::Cube);
  }

  /**
	* @brief Renders the mesh associated with the component.
	* 
	* This method is responsible for rendering the mesh in the current frame.
	* It overrides the base class's Render method.
	*/
  void Render() override {
    //m_mesh->Draw();
  }

  /**
	* @brief Updates the component's state.
	* 
	* This method is called every frame to update the component's logic.
	* 
	* @param deltaTime The time elapsed since the last update.
	*/
  void Update(float deltaTime) override {}

  /**
	* @brief Sets the mesh associated with the component.
	* 
	* @param mesh The new mesh to be associated with this component.
	*/
  void SetMesh(const std::shared_ptr<Mesh>& mesh);

  /**
	* @brief Clears the current mesh's data.
	* 
	* This method releases the resources held by the current mesh.
	*/
  void ClearMesh();

  /**
	* @brief Retrieves the mesh associated with the component.
	* 
	* @return A pointer to the associated mesh.
	*/
  std::shared_ptr<Mesh> GetMesh();

  [[nodiscard]] MeshType GetMeshType() const;

 private:
  std::shared_ptr<Mesh> m_mesh;
};
