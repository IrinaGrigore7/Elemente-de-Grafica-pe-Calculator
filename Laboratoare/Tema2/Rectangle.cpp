#include "Rectangle.h"
#include "Transform2D.h"
#include <include/glm.h>

#include <Core/Engine.h>

Mesh* Rectangle2D::Create(std::string name, glm::vec3 color, float width, float height)
{
	Mesh* mesh = new Mesh(name);

	glm::vec3 centre(width / 2, height / 2, 0);
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	
	vertices =
	{
		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(0, height,  0), color),
		VertexFormat(glm::vec3(width, 0,  0), color),
		VertexFormat(glm::vec3(width, height,  0), color),

	};
	
	indices = { 0, 1, 3, 2};

	mesh->SetDrawMode(GL_QUADS);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

void Rectangle2D::Update(float deltaTime)
{
	float progressWidth = map(progress, minimum, maximum, 0, 1.0f);
	if (progressWidth < 0)
		progressWidth = 0;
	float theta = rotation * 3.142 / 180;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(position.x, position.y);
	modelMatrix *= Transform2D::Scale(progressWidth, scale);
	modelMatrix *= Transform2D::Rotate(theta);
	


}

