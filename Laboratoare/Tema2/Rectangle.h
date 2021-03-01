#pragma once

#include <string>
#include "Object2D.h"

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

class Rectangle2D : public Object2D
{
private:
	float width;
	float height;
	float progress;
	float maximum;
	float minimum;

public:
	static Mesh* Create(std::string name, glm::vec3 color, float width, float height);
	Rectangle2D() {
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
	Rectangle2D(Mesh* mesh_, glm::vec3 color_, glm::vec2 position_, float width_, float height_, float maximum_, float minimum_) : Object2D(mesh_, color_, position_){
		width = width_;
		height = height_;
		maximum = maximum_;
		minimum = minimum_;
		progress = 0;

	}
	void Update(float deltaTime) override;

	void setProgress(float value) {
		if (progress < minimum) {
			progress = minimum;
			return;
		}
		progress = value;
	}

	float map(float x, float in_min, float in_max, float out_min, float out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
};

