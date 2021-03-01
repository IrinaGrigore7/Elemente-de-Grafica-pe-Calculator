#pragma once

#include <string>
#include "Object.h"

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Bow : public Object
{
private:
	float radius1;
	float radius2;
	void CalculateCircleCollision() override;

public:
	static Mesh* Create(std::string name, float radius1, float radius2, glm::vec3 color);
	Bow() {
		name = "bow";
		position = glm::vec2(0, 0);
		radius1 = 50;
		radius2 = 100;
		color = glm::vec3(1, 1, 1);
		mesh = nullptr;
		type = Player | Colidable;
	}
	Bow(Mesh* mesh_, float radius1_, float radius2_, glm::vec3 color_, glm::vec2 position_) : Object(mesh_, color_, position_){
		name = "bow";
		radius1 = radius1_;
		radius2 = radius2_;
		type = Player | Colidable;
	}
	void Update(float deltaTime) override;
	void MoveUp(float delta) override;
	void MoveDown(float delta) override;

};

