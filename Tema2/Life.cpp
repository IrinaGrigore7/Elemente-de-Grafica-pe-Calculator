#include "Life.h"
#include "Transform2D.h"
#include <include/glm.h>

#include <Core/Engine.h>

Mesh* Life::Create(std::string name, float length, glm::vec3 color, bool fill)
{
	Mesh* mesh = new Mesh(name);
	float radius = length;
	float lineWidth = 2;
	float lineRadius = 0.7f * radius;
	int step = 2;
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	float theta;
	float theta2;
	float startAngle = 0;
	float endAngle = 180;
	float fullLength = 3 * radius;
	int index = 2;
	
	glm::vec3 centre1(-radius, 0, 0);
	glm::vec3 centre2(radius, 0, 0);
	glm::vec3 centre(0, 0, 0);
	vertices.push_back(VertexFormat(centre1, color));

	vertices.push_back(VertexFormat(centre2, color));


	for (int i = startAngle; i < endAngle; i += step) {
		indices.push_back(0);
		theta = i * 3.142 / 180;
		theta2 = (i + step) * 3.142 / 180;
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(theta) + centre1.x, radius * sin(theta) + centre1.y, 0), color));
		indices.push_back(index++);

		vertices.push_back(VertexFormat(glm::vec3(radius * cos(theta2) + centre1.x, radius * sin(theta2) + centre1.y, 0), color));
		indices.push_back(index++);
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(theta) + centre2.x, radius * sin(theta) + centre2.y, 0), color));
		indices.push_back(1);
		indices.push_back(index++);
	
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(theta2) + centre2.x, radius * sin(theta2) + centre2.y, 0), color));
		indices.push_back(index++);
	}
	endAngle *= 3.142 / 180;
	startAngle *= 3.142 / 180;
	vertices.push_back(VertexFormat(glm::vec3(radius * cos(endAngle) + centre1.x, radius * sin(endAngle) + centre1.y, 0), color));
	indices.push_back(index++);
	vertices.push_back(VertexFormat(glm::vec3(radius * cos(startAngle) + centre2.x, radius * sin(startAngle) + centre2.y, 0), color));
	indices.push_back(index++);
	
	
	vertices.push_back(VertexFormat(glm::vec3(0, -fullLength, 0), color));
	indices.push_back(index++);




	mesh->SetDrawMode(GL_TRIANGLES);

	mesh->InitFromData(vertices, indices);
	return mesh;
}

void Life::Update(float deltaTime)
{
	float theta = rotation * 3.142 / 180;
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(position.x, position.y);
	modelMatrix *= Transform2D::Rotate(theta);


}





