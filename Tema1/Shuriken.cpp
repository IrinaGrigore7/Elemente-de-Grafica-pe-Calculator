#include "Shuriken.h"
#include "Transform2D.h"
#include <include/glm.h>

#include <Core/Engine.h>

Mesh* Shuriken::Create(std::string name, float width, glm::vec3 color)
{
	Mesh* mesh = new Mesh(name);

	glm::vec3 centre(0, 0, 0);
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	

	vertices =
	{
		VertexFormat(centre, color),
		VertexFormat(glm::vec3(-width / 2, 0,  0), color),
		VertexFormat(glm::vec3(-width / 2, width / 2,  0), color),
		VertexFormat(glm::vec3(0, width / 2,  0), color),
		VertexFormat(glm::vec3(width / 2, width / 2, 0), color),
		VertexFormat(glm::vec3(width / 2, 0,  0), color),
		VertexFormat(glm::vec3(width / 2, -width / 2,  0), color),
		VertexFormat(glm::vec3(0, -width / 2,  0), color),
		VertexFormat(glm::vec3(-width / 2, -width / 2,  0), color)
	};


		indices = { 0, 2, 3,
			0, 4, 5,
			0, 6, 7,
			0, 8, 1 };

	mesh->SetDrawMode(GL_TRIANGLES);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

void Shuriken::Update(float deltaTime)
{
	
	float theta = rotation * 3.142 / 180;
	rotation += rotationSpeed;
	if (rotation == 360)
		rotation = 0;
	position.x -= movementSpeed;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Scale(scale, scale);
	modelMatrix *= Transform2D::Translate(position.x, position.y);
	modelMatrix *= Transform2D::Rotate(theta);
}

void Shuriken::CalculateCircleCollision()
{
	int x = position.x;
	int y = position.y;
	int radius = scale * (width / 2);
	boundingBox = glm::vec3(x, y, radius);
}
