#pragma once

#include <string>
#include "Object2D.h"

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Life : public Object2D
{
private:
	float radius1;

public:
	static Mesh* Create(std::string name, float length, glm::vec3 color, bool fill);
	Life() {
		name = "heart";
		position = glm::vec2(0, 0);
		radius1 = 50;
		color = glm::vec3(1, 1, 1);
		mesh = nullptr;
	}
	Life(Mesh* mesh_, float radius1_, glm::vec3 color_, glm::vec2 position_) : Object2D(mesh_, color_, position_){
		name = "heart";
		radius1 = radius1_;
	}
	void Update(float deltaTime) override;
};

