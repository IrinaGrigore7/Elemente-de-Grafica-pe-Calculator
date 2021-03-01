#include "Bow.h"
#include "Transform2D.h"
#include <include/glm.h>

#include <Core/Engine.h>

Mesh* Bow::Create(std::string name, float radius1, float radius2, glm::vec3 color)
{
	Mesh* mesh = new Mesh(name);
	glm::vec3 clr(color);
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	float theta;
	int index = 0;
	for (int i = -90; i < 91; i += 3) {
		theta = i * 3.142 / 180;
		vertices.push_back(VertexFormat(glm::vec3(radius1 * cos(theta), radius2 * sin(theta), 0), color));
		indices.push_back(index++);
	}

	mesh->SetDrawMode(GL_LINE_LOOP);
	mesh->InitFromData(vertices, indices);
	return mesh;
}
void Bow::CalculateCircleCollision()
{
	int x = position.x;
	int y = position.y;
	int z = radius2;
	boundingBox = glm::vec3(x, y, z);
}

void Bow::Update(float deltaTime)
{
	float theta = rotation * 3.142 / 180;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(position.x, position.y);
	modelMatrix *= Transform2D::Rotate(theta);


}

void Bow::MoveUp(float deltaTime)
{

	int height = 720 - radius2;
	if (position.y < height)
		position.y += 100 * deltaTime;
}
void Bow::MoveDown(float deltaTime)
{
	if (position.y > radius2)
		position.y -= 100 * deltaTime;


}




