#include "Arrow.h"
#include "Transform2D.h"
#include <include/glm.h>

#include <Core/Engine.h>

Mesh* Arrow::Create(std::string name, float length, float tipLength, glm::vec3 color)
{
	Mesh* mesh = new Mesh(name);
	float lineWidth = 4;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	

	vertices =
	{
		VertexFormat(glm::vec3(0, lineWidth / 2,  0), color),
		VertexFormat(glm::vec3(length, lineWidth / 2,  0), color),
		VertexFormat(glm::vec3(length, lineWidth / 2 + lineWidth,  0), color),
		VertexFormat(glm::vec3(length + tipLength, 0, 0), color),
		VertexFormat(glm::vec3(length, -(lineWidth / 2 + lineWidth),  0), color),
		VertexFormat(glm::vec3(length, -lineWidth / 2,  0), color),
		VertexFormat(glm::vec3(0, -lineWidth / 2,  0), color)
	};
	indices = { 0, 1, 5, 
		0, 5, 6,
		2, 3, 4};

	mesh->SetDrawMode(GL_TRIANGLES);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

void Arrow::CalculateCircleCollision()
{
	float lineWidth = 4;
	int x = position.x + length + tipLength;
	int y = position.y + lineWidth;
	int radius = 0;
	boundingBox = glm::vec3(x, y, radius);
}

void Arrow::Update(float deltaTime)
{
	float theta = rotation * 3.142 / 180;
	
	if (release) {
		theta = fixedRotation * 3.142 / 180;
		float xSpeed = cos(theta);
		float ySpeed = sin(theta);
		position.x += arrowSpeed * xSpeed;
		position.y += arrowSpeed * ySpeed;
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(position.x, position.y);
	modelMatrix *= Transform2D::Rotate(theta);
}

void Arrow::MoveUp(float deltaTime)
{
	int height = 720 - 100; // -radius1
	if (!release) {
		if (position.y < height)
			position.y += 100 * deltaTime;
	}
}

void Arrow::MoveDown(float deltaTime)
{
	int height = 100;
	if (!release) {
		if (position.y > height)
			position.y -= 100 * deltaTime;
	}

}

void Arrow::Release(int speed)
{
	if (!release) {
		release = true;
		fixedRotation = rotation;
		arrowSpeed = speed;
	}
}
