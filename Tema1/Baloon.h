#pragma once

#include <string>
#include "Object.h"
#include <ctime>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Baloon : public Object
{
private:
	float radius1;
	float radius2;
	int lines;
	float speed;
	static glm::vec3 addLine(std::vector<VertexFormat> &vertices, std::vector<unsigned short> &indices, int &index, glm::vec3 start, float radius1, glm::vec3 color, bool left = true);
	void CalculateCircleCollision() override;

public:
	static Mesh* Create(std::string name, float radius1, float radius2, glm::vec3 color, int lines = 4);
	Baloon() {
		name = "baloon";
		position = glm::vec2(0, 0);
		radius1 = 50;
		radius2 = 100;
		lines = 4;
		color = glm::vec3(1, 1, 1);
		mesh = nullptr;
		type = Enemy | Colidable;

	}
	Baloon(Mesh* mesh_, float radius1_, float radius2_, glm::vec3 color_, glm::vec2 position_, int lines_ = 4) : Object(mesh_, color_, position_){
		name = "baloon";
		radius1 = radius1_;
		radius2 = radius2_;
		lines = lines_;
		speed = (rand() % 3 + 1);
		type = Enemy | Colidable;

	}

	void Update(float deltaTime) override;

};

