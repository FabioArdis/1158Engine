#include "Mesh.h"
#include <math.h>
// #include <iostream> // Unused, can be removed

Mesh::Mesh() : m_VAO(0), m_VBO(0), m_EBO(0)
{
	m_meshType = MeshType::Custom;
}

Mesh::~Mesh()
{
	// Delete GPU resources
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

Mesh::Mesh(MeshType type) : m_VAO(0), m_VBO(0), m_EBO(0) // Delegating constructor
{
	if (type == MeshType::Cube)
	{
		CreateCube();
		m_meshType = MeshType::Cube;
	}
	else if (type == MeshType::Plane)
	{
		CreatePlane();
		m_meshType = MeshType::Plane;
	}
	else if (type == MeshType::Capsule)
	{
		CreateCapsule(0.5f, 2.0f, 32, 16);
		m_meshType = MeshType::Capsule;
	}
}

void Mesh::CreateCube()
{
	// Vertex data for a cube (positions and normals)
	m_vertices = {
		// positions          // normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	// Indices to draw the cube
	m_indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};

	// Setup VAO, VBO, and EBO for the cube
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	// Vertex buffer setup
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	// Index buffer setup
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::CreatePlane()
{
	// Vertex data for a plane (positions, normals, and texture coordinates)
	m_vertices = {
		// Positions          // Normals           // Texture Coords
		-0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,  // Vertex 1
		 0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f,  // Vertex 2
		 0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // Vertex 3
		-0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f   // Vertex 4
	};

	// Indices to draw two triangles forming the plane
	m_indices = {
		0, 1, 2,  // First triangle
		2, 3, 0   // Second triangle
	};

	// Setup VAO, VBO, and EBO for the plane
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	// Vertex buffer setup
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	// Index buffer setup
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::CreateCapsule(float radius, float height, int segments, int rings)
{
	m_vertices.clear();
    m_indices.clear();

    const float PI = 3.14159265359f;
    const int verticalSegments = segments;
    const int horizontalSegments = rings;
    const float halfHeight = height * 0.5f;

    for (int ring = 0; ring <= horizontalSegments / 2; ++ring)
	{
        float phi = PI * float(ring) / float(horizontalSegments);
        for (int segment = 0; segment <= verticalSegments; ++segment)
		{
            float theta = 2.0f * PI * float(segment) / float(verticalSegments);

            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi) + halfHeight;
            float z = radius * sin(phi) * sin(theta);

            float nx = sin(phi) * cos(theta);
            float ny = cos(phi);
            float nz = sin(phi) * sin(theta);

            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);
            m_vertices.push_back(nx);
            m_vertices.push_back(ny);
            m_vertices.push_back(nz);
        }
    }

    for (int ring = 0; ring <= 1; ++ring)
	{
        float y = halfHeight - ring * height;
        for (int segment = 0; segment <= verticalSegments; ++segment)
		{
            float theta = 2.0f * PI * float(segment) / float(verticalSegments);

            float x = radius * cos(theta);
            float z = radius * sin(theta);

            float nx = cos(theta);
            float ny = 0.0f;
            float nz = sin(theta);

            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);
            m_vertices.push_back(nx);
            m_vertices.push_back(ny);
            m_vertices.push_back(nz);
        }
    }

    for (int ring = horizontalSegments / 2; ring <= horizontalSegments; ++ring) 
	{
        float phi = PI * float(ring) / float(horizontalSegments);
        for (int segment = 0; segment <= verticalSegments; ++segment) 
		{
            float theta = 2.0f * PI * float(segment) / float(verticalSegments);

            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi) - halfHeight;
            float z = radius * sin(phi) * sin(theta);

            float nx = sin(phi) * cos(theta);
            float ny = cos(phi);
            float nz = sin(phi) * sin(theta);

            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);
            m_vertices.push_back(nx);
            m_vertices.push_back(ny);
            m_vertices.push_back(nz);
        }
    }

    int topOffset = 0;
    for (int ring = 0; ring < horizontalSegments / 2; ++ring) 
	{
        for (int segment = 0; segment < verticalSegments; ++segment) 
		{
            int current = ring * (verticalSegments + 1) + segment;
            int next = current + (verticalSegments + 1);

            m_indices.push_back(topOffset + current);
            m_indices.push_back(topOffset + next);
            m_indices.push_back(topOffset + current + 1);

            m_indices.push_back(topOffset + current + 1);
            m_indices.push_back(topOffset + next);
            m_indices.push_back(topOffset + next + 1);
        }
    }

    int cylinderOffset = (horizontalSegments / 2 + 1) * (verticalSegments + 1);
    for (int segment = 0; segment < verticalSegments; ++segment) 
	{
        int current = segment;
        int next = current + (verticalSegments + 1);

        m_indices.push_back(cylinderOffset + current);
        m_indices.push_back(cylinderOffset + next);
        m_indices.push_back(cylinderOffset + current + 1);

        m_indices.push_back(cylinderOffset + current + 1);
        m_indices.push_back(cylinderOffset + next);
        m_indices.push_back(cylinderOffset + next + 1);
    }

    int bottomOffset = cylinderOffset + 2 * (verticalSegments + 1);
    for (int ring = 0; ring < horizontalSegments / 2; ++ring)
	{
        for (int segment = 0; segment < verticalSegments; ++segment)
		{
            int current = ring * (verticalSegments + 1) + segment;
            int next = current + (verticalSegments + 1);

            m_indices.push_back(bottomOffset + current);
            m_indices.push_back(bottomOffset + next);
            m_indices.push_back(bottomOffset + current + 1);

            m_indices.push_back(bottomOffset + current + 1);
            m_indices.push_back(bottomOffset + next);
            m_indices.push_back(bottomOffset + next + 1);
        }
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Draw()
{
	// Bind VAO and draw the mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Clear()
{
	// Delete VAO, VBO, and EBO from GPU
	if (m_VAO) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}

	if (m_VBO) {
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
	}

	if (m_EBO) {
		glDeleteBuffers(1, &m_EBO);
		m_EBO = 0;
	}

	// Clear vertex and index data
	m_vertices.clear();
	m_indices.clear();
}

MeshType Mesh::GetType() const
{
    return m_meshType;
}
