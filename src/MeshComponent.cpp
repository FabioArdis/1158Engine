#include "MeshComponent.h"

void MeshComponent::SetMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

void MeshComponent::ClearMesh()
{
	m_mesh->Clear();
}

Mesh* MeshComponent::GetMesh()
{
	return m_mesh;
}

MeshType MeshComponent::GetMeshType() const
{
    return m_mesh->GetType();
}
