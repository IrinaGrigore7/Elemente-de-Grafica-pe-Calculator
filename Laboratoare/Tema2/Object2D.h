#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>


class Object2D
{



public:

	Object2D() {
		name = "object";
		position = glm::vec2(0, 0);
		color = glm::vec3(1, 1, 1);
		modelMatrix = glm::mat3(1);
		mesh = nullptr;
		rotation = 0;
		scale = 1;
	}
	Object2D(Mesh* mesh_, glm::vec3 color_, glm::vec2 position_) {
		mesh = mesh_;
		name = "object";
		color = color_;
		position = position_;
		modelMatrix = glm::mat3(1);
		rotation = 0;
		scale = 1;

	}

	virtual void Update(float deltatIme) {};

	void setRotation(const int& value) {
		rotation = value;
	}

	int getRotation() {
		return rotation;
	}

	Mesh* getMesh() {
		return mesh;
	}

	std::string getName() {
		return name;
	}

	glm::vec3 getPosition() {
		return glm::vec3(position.x, position.y, 0);
	}

	void setPosition(int x, int y) {
		position = glm::vec3(x, y, 0);
	}

	glm::vec3 getColor() {
		return color;
	}

	
	
	glm::mat3 getModelMatrix() {
		return modelMatrix;
	}


	void setScale(float scale_) {
		scale = scale_;
	}

	float getScale() {
		return scale;
	}

	glm::vec3 getScaleVector() {
		return glm::vec3(scale, scale, 0);
	}

protected:
	Mesh* mesh;
	std::string name;
	glm::vec2 position;
	glm::vec3 color;
	float rotation;
	glm::mat3 modelMatrix;
	float scale;
	
};

