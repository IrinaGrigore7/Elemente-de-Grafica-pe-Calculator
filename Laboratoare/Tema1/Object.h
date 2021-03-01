#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>


class Object
{



public:
	enum  EntityType
	{
		None = 0,
		Player = 1,
		Projectile = 2,
		Enemy = 4,
		Shootabe = 8,
		Colidable = 16
	};

	Object() {
		name = "object";
		position = glm::vec2(0, 0);
		color = glm::vec3(1, 1, 1);
		modelMatrix = glm::mat3(1);
		mesh = nullptr;
		rotation = 0;
		toRemove = false;
		type = None;
		coliders = None;
		scale = 1;
		falling = false;
	}
	Object(Mesh* mesh_, glm::vec3 color_, glm::vec2 position_) {
		mesh = mesh_;
		name = "object";
		color = color_;
		position = position_;
		modelMatrix = glm::mat3(1);
		rotation = 0;
		toRemove = false;
		type = None;
		coliders = None;
		scale = 1;
		falling = false;

	}

	virtual void Update(float deltatIme) {};
	virtual void MoveUp(float delta) {};
	virtual void MoveDown(float delta) {};
	bool IsColidable() {
		return (type & Colidable) == Colidable;
	}
	bool ColidesWith(int value) {
		return (coliders & value) == value;
	}

	bool checkColision(Object* obj) {
		glm::vec3 a = getBoundingBox();
		glm::vec3 b = obj->getBoundingBox();
		int dx = a.x - b.x;
		int dy = a.y - b.y;
		float distance = sqrt(dx * dx + dy * dy);
		return (distance < (float)(a.z + b.z));
	}
	void CheckOutOfBounds() {
		glm::vec2 resolution(1280, 720);
		glm::vec2 pos = getPosition();
		if (pos.x > resolution.x || pos.y > resolution.y || pos.x < 0 || pos.y < 0) {
			toRemove = true;
		}
	}

	bool isPlayer() {
		return (type & Player) == Player;
	}

	void setRotation(const int& value) {
		rotation = value;
	}

	int getRotation() {
		return rotation;
	}

	void setToRemove(const bool& value) {
		toRemove = value;
	}

	bool getToRemove() {
		return toRemove;
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

	glm::vec3 getBoundingBox() {
		CalculateCircleCollision();
		return boundingBox;
	}

	int getType() {
		return type;
	}

	int getColiders() {
		return coliders;
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

	bool getFalling() {
		return falling;
	}

	void setFalling(bool value) {
		falling = value;
	}

protected:
	Mesh* mesh;
	std::string name;
	glm::vec2 position;
	glm::vec3 color;
	float rotation;
	glm::mat3 modelMatrix;
	bool toRemove;
	glm::vec3 boundingBox;
	int type;
	int coliders;
	float scale;
	bool falling;

	virtual void CalculateCircleCollision() {};
	
};

