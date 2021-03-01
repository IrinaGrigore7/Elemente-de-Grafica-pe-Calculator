#pragma once

#include <string>
#include "Object.h"

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Shuriken : public Object
{
private:
	float width;
	float rotationSpeed;
	float movementSpeed;
	void CalculateCircleCollision() override;

public:
	static Mesh* Create(std::string name, float width, glm::vec3 color);
	Shuriken() {
		name = "shuriken";
		position = glm::vec2(0, 0);
		width = 50;
		color = glm::vec3(1, 1, 1);
		rotationSpeed = 20;
		mesh = nullptr;
		type = Enemy | Colidable;
		coliders = Player | Colidable;
		movementSpeed = 3;

	}
	Shuriken(Mesh* mesh_, float width_, glm::vec3 color_, glm::vec2 position_, float movementSpeed_) : Object(mesh_, color_, position_){
		name = "shuriken";
		width = width_;
		movementSpeed = movementSpeed_;
		rotationSpeed = (movementSpeed / width) * (180 / 3.142);
		type = Enemy | Colidable;
		coliders = Player | Colidable;

	}
	void Update(float deltaTime) override;
};

