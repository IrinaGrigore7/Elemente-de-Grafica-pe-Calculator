#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>


class Obj
{

public:
	Obj() {
		name = "object";
		mesh = nullptr;
		position = glm::vec3(0, 0, 0);
		color = glm::vec3(1, 1, 1);
		modelMatrix = glm::mat4(1);
	}

	Obj(Mesh* mesh_, glm::vec3 position_, glm::vec3 color_) {
		name = "object";
		mesh = mesh_;
		position = position_;
		color = color_;
		modelMatrix = glm::mat4(1);
	}

	virtual void Update(float deltatIme) {};

	glm::fvec3 getPosition() {
		return glm::fvec3(position.x, position.y, position.z);
	}

	void setPosition(float x, float y, float z) {
		position = glm::fvec3(x, y, z);
	}

	glm::vec3 getColor() {
		return color;
	}
	void setColor(glm::vec3 color_) {
		color = color_;
	}
	glm::mat4 getModelMatrix() {
		return modelMatrix;
	}

	Mesh* getMesh() {
		return mesh;
	}

protected:
	std::string name;
	glm::fvec3 position;
	glm::vec3 color;
	glm::mat4 modelMatrix;
	Mesh* mesh;
	

};

