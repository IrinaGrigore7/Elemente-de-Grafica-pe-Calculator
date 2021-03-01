#include "Baloon.h"
#include "Transform2D.h"
#include <include/glm.h>

#include <Core/Engine.h>

Mesh* Baloon::Create(std::string name, float radius1, float radius2, glm::vec3 color, int lines)
{
	Mesh* mesh = new Mesh(name);
	float triangleHeight = 0.4f * radius1;
	float lineWidth = 2;
	float lineRadius = 0.7f * radius1;
	float step = 5;
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	float theta;
	float theta2;
	int index = 0;
	glm::vec3 centre(0, 0, 0);
	vertices.push_back(VertexFormat(centre, color));
	index++;

	for (int i = -180; i < 180 ; i += step) {
		theta = i * 3.142 / 180;
		theta2 = (i + step) * 3.142 / 180;
		indices.push_back(0);
		vertices.push_back(VertexFormat(glm::vec3(radius1 * cos(theta), radius2 * sin(theta), 0), color));
		indices.push_back(index++);
		vertices.push_back(VertexFormat(glm::vec3(radius1 * cos(theta2), radius2 * sin(theta2), 0), color));
		indices.push_back(index++);
	}

	vertices.push_back(VertexFormat(glm::vec3(0, -radius2 + triangleHeight / 2, 0), color));
	indices.push_back(index++);
	vertices.push_back(VertexFormat(glm::vec3(-triangleHeight / 3, -radius2 - triangleHeight, 0), color));
	indices.push_back(index++);
	vertices.push_back(VertexFormat(glm::vec3(triangleHeight / 3, -radius2 - triangleHeight, 0), color));
	indices.push_back(index++);

	bool left = true;
	glm::vec3 res(0, -radius2, 0);
	for (int i = 0; i < lines; ++i) {
		res = addLine(vertices, indices, index, res, radius1, glm::vec3(0, 0, 0), left);
		left = !left;
	}

	mesh->SetDrawMode(GL_TRIANGLES);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

glm::vec3 Baloon::addLine(std::vector<VertexFormat>& vertices, std::vector<unsigned short>& indices, int& index, glm::vec3 start, float radius1, glm::vec3 color, bool left)
{
	float lineWidth = 2;
	float lineRadius = 0.5f * radius1;
	glm::vec3 result(0, 0, 0);
	
	if (left) {
		vertices.push_back(VertexFormat(glm::vec3(start.x -lineWidth / 2, start.y + lineWidth / 2, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(start.x + lineWidth / 2, start.y - lineWidth / 2, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(start.x -lineRadius - lineWidth / 2, start.y - lineRadius / 2 + lineWidth / 2, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(start.x -lineRadius + lineWidth / 2, start.y - lineRadius / 2 - lineWidth / 2, 0), color));
		result = glm::vec3(start.x - lineRadius, start.y - lineRadius / 2, 0);
	}
	else {
		vertices.push_back(VertexFormat(glm::vec3(start.x + lineWidth / 2, start.y + lineWidth / 2, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(start.x - lineWidth / 2, start.y - lineWidth / 2, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(start.x + lineRadius - lineWidth / 2, start.y - lineRadius / 2 - lineWidth / 2, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(start.x + lineRadius + lineWidth / 2, start.y - lineRadius / 2 + lineWidth / 2, 0), color));
		result = glm::vec3(start.x + lineRadius, start.y - lineRadius / 2, 0);
	}
	indices.push_back(index);
	indices.push_back(index + 1);
	indices.push_back(index + 2);
	indices.push_back(index + 2);
	indices.push_back(index + 3);
	indices.push_back(index);
	index += 4;
	return result;

}

void Baloon::CalculateCircleCollision()
{
	int x = position.x;
	int y = position.y;
	int radius = radius2 * scale;
	boundingBox = glm::vec3(x, y, radius);
}

void Baloon::Update(float deltaTime)
{
	if(!falling)
		position.y += speed;
	else {
		position.y -= 10;
	}
	modelMatrix = glm::mat3(1);
	
	modelMatrix *= Transform2D::Translate(position.x, position.y);
	if (falling)
		modelMatrix *= Transform2D::Scale(scale / 4, scale);
	else
		modelMatrix *= Transform2D::Scale(scale, scale);
}


