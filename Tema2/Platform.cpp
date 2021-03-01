#include "Platform.h"
#include <include/glm.h>
#include <algorithm>
#include <Core/Engine.h>

Mesh* Platform::Create(std::string name) {
	Mesh* mesh = new Mesh(name);
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	return mesh;
}

void Platform::Update(int n)
{
	position.z += (movementSpeed + auxMovementSpeed);
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.02f, 3));
}

bool Platform::Intersect(glm::vec3 point, float radius)
{
	float width = 1;
	float height = 3;
	float depth = 0.02f;
	float minX = position.x - width / 2;
	float maxX = position.x + width / 2;
	float minZ = position.z - height / 2;
	float maxZ = position.z + height / 2;
	float minY = position.y - depth / 2;
	float maxY = position.y + depth / 2;
	float x = std::max<float>(minX, std::min<float>(point.x, maxX));
	float y = std::max<float>(minY, std::min<float>(point.y, maxY));
	float z = std::max<float>(minZ, std::min<float>(point.z, maxZ));
	float distace = std::sqrt((x - point.x) * (x - point.x) + (y - point.y) * (y - point.y) + (z - point.z) * (z - point.z));
	
	return distace < radius + depth / 2;
}





