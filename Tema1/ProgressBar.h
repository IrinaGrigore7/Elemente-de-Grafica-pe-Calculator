#pragma once

#include <string>
#include "Object.h"

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class ProgressBar : public Object
{
private:
	float width;
	float height;
	float progress;
	float maximum;
	float minimum;
	void CalculateCircleCollision() override;

public:
	static Mesh* Create(std::string name, glm::vec3 color, float width, float height, float maximum, float minimum);
	ProgressBar() {
		name = "progressBar";
		position = glm::vec2(0, 0);
		width = 100;
		height = 20;
		color = glm::vec3(1, 1, 1);
		mesh = nullptr;
		progress = 0;
		maximum = 100;
		minimum = 0;

	}
	ProgressBar(Mesh* mesh_, glm::vec3 color_, glm::vec2 position_, float width_, float height_, float maximum_, float minimum_) : Object(mesh_, color_, position_){
		width = width_;
		height = height_;
		maximum = maximum_;
		minimum = minimum_;
		progress = 0;
		type = Enemy | Colidable;
		coliders = Player | Colidable;

	}
	void Update(float deltaTime) override;

	void setProgress(float value) {
		progress = value;
	}

	float map(float x, float in_min, float in_max, float out_min, float out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
};

