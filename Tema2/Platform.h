#pragma once

#include <string>
#include "Obj.h"

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Platform : public Obj
{

public:
	enum  Type
	{
		None = 0,
		Fuel = 1,
		LoseFuel = 2,
		LockSpeed = 4,
		Dead = 8,
		Visited = 16,
		God = 32
	};
	static Mesh* Create(std::string name);

	Platform() {
		name = "platform";
		position = glm::vec3(0, 0, 0);
		mesh = nullptr;
		color = glm::vec3(1, 1, 1);
		movementSpeed = 0.01f;
		acceleration = 0.001f;
		type = None;
		auxMovementSpeed = 0;

	}

	Platform(Mesh* mesh_, glm::vec3 color_, glm::vec3 position_) : Obj(mesh_, position_, color_) {
		name = "platform";
		movementSpeed = 0.01f;
		acceleration = 0.001f;
		auxMovementSpeed = 0;
	}

	void setTranslateX(float value) {
		translateX = value;
	}

	void setTranslateZ(float value) {
		translateZ = value;
	}

	float getTranslateX() {
		return translateX;
	}

	float getMovementSpeed() {
		return movementSpeed;
	}

	void setMovementSpeed(float value) {
		movementSpeed = value;
	}

	float getAcceleration() {
		return acceleration;
	}

	void setAcceleration(float value) {
		acceleration = value;
	}

	Type getType() {
		return type;
	}

	void setType(Type value) {
		type = value;
	}
	void Update(int n);
	bool Intersect(glm::vec3 point, float radius);
	void Accelerate() {
		auxMovementSpeed += acceleration;
	}
	void Decelerate() {
		movementSpeed -= acceleration;
	}

	bool isGood() {
		return type == None || type == Fuel;
	}

	void setAuxMovementSpeed(float value) {
		auxMovementSpeed = value;
	}

	float getAuxMovementSpeed() {
		return auxMovementSpeed;
	}

protected:
	float translateX;
	float translateZ;
	float movementSpeed;
	float auxMovementSpeed;
	float acceleration;
	Type type;

};