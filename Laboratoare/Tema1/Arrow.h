#pragma once

#include <string>
#include "Object.h"

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Arrow : public Object
{
private:
	float length;
	float tipLength;
	bool release;
	int fixedRotation;
	int arrowSpeed;
	void CalculateCircleCollision() override;

public:
	static Mesh* Create(std::string name, float length, float tipLength, glm::vec3 color);
	Arrow(const Arrow& other) {
		name = other.name;
		position = glm::vec2(other.position);
		length = other.length;
		tipLength = other.tipLength;
		color = glm::vec3(other.color);
		mesh = other.mesh;
		release = other.release;
		fixedRotation = other.fixedRotation;
		rotation = other.rotation;
		type = other.type;
		coliders = other.coliders;
		arrowSpeed = other.arrowSpeed;

	}
	Arrow() {
		name = "arrow";
		position = glm::vec2(0, 0);
		length = 50;
		tipLength = 100;
		color = glm::vec3(1, 1, 1);
		mesh = nullptr;
		release = false;
		fixedRotation = 0;
		type = Projectile;
		coliders = Enemy | Colidable;
		arrowSpeed = 2;

	}
	Arrow(Mesh* mesh_, float length_, float tipLength_, glm::vec3 color_, glm::vec2 position_) : Object(mesh_, color_, position_){
		name = "arrow";
		length = length_;
		tipLength = tipLength_;
		release = false;
		fixedRotation = 0;
		type = Projectile;
		coliders = Enemy | Colidable;
		arrowSpeed = 2;

	}
	void Update(float deltaTime) override;
	void MoveUp(float deltaTime) override;
	void MoveDown(float deltaTime) override;
	void Release(int speed);
};

