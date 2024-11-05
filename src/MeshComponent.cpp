#include "MeshComponent.h"

void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh) {
  m_mesh = mesh;
}

void MeshComponent::ClearMesh() {
  m_mesh->Clear();
}

std::shared_ptr<Mesh> MeshComponent::GetMesh() {
  return m_mesh;
}

MeshType MeshComponent::GetMeshType() const {
  return m_mesh->GetType();
}
