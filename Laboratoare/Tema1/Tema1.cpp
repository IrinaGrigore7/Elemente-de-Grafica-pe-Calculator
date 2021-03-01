#include "Tema1.h"
#include "Bow.h"
#include "Arrow.h"
#include "Shuriken.h"
#include "Baloon.h"
#include "Heart.h"
#include "Object.h"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cmath>

#include <Core/Engine.h>
#include "Transform2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	float radius1 = 40;
	float radius2 = 50;

	mousePress = false;
	hasArrow = true;
	lives = 3;
	score = 0;

	AddMeshToList(Bow::Create("bow", 75, 100, glm::vec3(0, 0, 0)));
	AddMeshToList(Arrow::Create("arrow", 100, 16, glm::vec3(0, 0, 0)));
	AddMeshToList(Shuriken::Create("shuriken", 50, glm::vec3(0, 0, 0)));
	AddMeshToList(Baloon::Create("baloonRed", radius1, radius2, glm::vec3(1, 0, 0)));
	AddMeshToList(Baloon::Create("baloonYellow", radius1, radius2, glm::vec3(1, 1, 0)));
	AddMeshToList(ProgressBar::Create("pBar", glm::vec3(0, 1, 0), 200, 30, 5, 0));
	AddMeshToList(Heart::Create("heart", 7, glm::vec3(1, 0, 0), true));

	progressBar = new ProgressBar(meshes["pBar"], glm::vec3(0, 1, 0), glm::vec2(10, 10), 200, 30, 5, 0);

	hearts.push_back(new Heart(meshes["heart"], 7, glm::vec3(1, 0, 0), glm::vec2(25, 710)));
	hearts.push_back(new Heart(meshes["heart"], 7, glm::vec3(1, 0, 0), glm::vec2(60, 710)));
	hearts.push_back(new Heart(meshes["heart"], 7, glm::vec3(1, 0, 0), glm::vec2(95, 710)));

	bow = new Bow(meshes["bow"], 75, 100, glm::vec3(0, 0, 0), glm::vec2(50, resolution.y / 2));
	objects.push_back(bow);
	
	currentArrow = new Arrow(meshes["arrow"], 100, 16, glm::vec3(0, 0, 0), glm::vec2(50, resolution.y / 2));
	objects.push_back(currentArrow);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.9, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{

	if (lives == 0) {
		return;
	}
	auto end = std::chrono::steady_clock::now();
	if (mousePress) {
		int seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - startTime).count() / 100;
		if (seconds > 5) {
			seconds = 5;
		}
		progressBar->setProgress(seconds);
	}
	else {
		progressBar->setProgress(0);
	}
	/*currentArrow->setPosition(bow->getPosition().x, bow->getPosition().y);
	currentArrow->setRotation(bow->getRotation());*/
	int secondsReload = std::chrono::duration_cast<std::chrono::milliseconds>(end - reloadStartTime).count() / 1000;
	if (!hasArrow && secondsReload >= 1) {
		Reload();
		hasArrow = true;
	}

	for (int i = 0; i < lives; ++i) {
		hearts[i]->Update(deltaTimeSeconds);
		RenderMesh2D(hearts[i]->getMesh(), shaders["VertexColor"], hearts[i]->getModelMatrix());
	}

	progressBar->Update(deltaTimeSeconds);
	RenderMesh2D(progressBar->getMesh(), shaders["VertexColor"], progressBar->getModelMatrix());

	SpawnBaloons();
	SpawnShurikens();
	
	for (int i = 0; i < objects.size(); ++i) {
		if (!objects[i]->getToRemove()) {
			objects[i]->Update(deltaTimeSeconds);
			objects[i]->CheckOutOfBounds();
		}
	}
	CheckCollisions();

	for (int i = 0; i < objects.size(); ++i) {
		RenderMesh2D(objects[i]->getMesh(), shaders["VertexColor"], objects[i]->getModelMatrix());
	}
	CleanUp();

}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		bow->MoveUp(deltaTime);
		if(hasArrow)
			currentArrow->MoveUp(deltaTime);
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		bow->MoveDown(deltaTime);
		if (hasArrow)
			currentArrow->MoveDown(deltaTime);
	}
	
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE) {
		if (lives == 0)
			Reset();
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	int y = 720 - mouseY - bow->getPosition().y;
	int x = mouseX - bow->getPosition().x;
	double angle = 90 - (atan2(x, y) * 180 / 3.142);
	bow->setRotation((int)angle);
	currentArrow->setRotation((int)angle);
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	mousePress = true;
	startTime = std::chrono::steady_clock::now();
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	mousePress = false;
	hasArrow = false;
	reloadStartTime = std::chrono::steady_clock::now();
	auto end = std::chrono::steady_clock::now();
	int seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - startTime).count() / 100;
	if (seconds > 5) {
		seconds = 5;
	}
	currentArrow->Release(seconds * 5);
	Arrow* oldArrow = currentArrow;
	arrows.push_back(oldArrow);
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

void Tema1::CleanUp()
{
	std::vector <int> toRemove;
	
	for (int i = 0; i < arrows.size(); ++i) {
		if (arrows[i]->getToRemove()) {
			toRemove.push_back(i);
		}
	}

	for (int i = toRemove.size() - 1; i >=0; --i) {
		arrows.erase(arrows.begin() + toRemove[i]);
	}

	toRemove.clear();
	for (int i = 0; i < baloons.size(); ++i) {
		if (baloons[i]->getToRemove()) {
			toRemove.push_back(i);
		}
	}

	for (int i = toRemove.size() - 1; i >= 0; --i) {
		baloons.erase(baloons.begin() + toRemove[i]);
	}
	toRemove.clear();
	for (int i = 0; i < shurikens.size(); ++i) {
		if (shurikens[i]->getToRemove()) {
			toRemove.push_back(i);
		}
	}

	for (int i = toRemove.size() - 1; i >= 0; --i) {
		shurikens.erase(shurikens.begin() + toRemove[i]);
	}

	toRemove.clear();

	for (int i = 0; i < objects.size(); ++i) {
		if (objects[i] == nullptr || objects[i]->getToRemove()) {
			toRemove.push_back(i);

		}
	}
	
	for (int i = toRemove.size() - 1; i >= 0; --i) {
		delete objects[toRemove[i]];
		objects.erase(objects.begin() + toRemove[i]);
	}
}

void Tema1::CheckCollisions()
{
	glm::vec3 black(0, 0, 0);
	glm::vec3 red(1, 0, 0);
	glm::vec3 yellow(1, 1, 0);

	for (int i = 0; i < arrows.size(); ++i) {
		for (int j = 0; j < objects.size(); ++j) {
			if (objects[j]->IsColidable()) {
				if (arrows[i]->ColidesWith(objects[j]->getType()) && !objects[j]->getToRemove() && !arrows[i]->getToRemove() && !objects[j]->getFalling()) {
					if (arrows[i]->checkColision(objects[j])) {
						if (objects[j]->getColor() == yellow) {
							score -= 5;
						}
						else {
							score += 10;
						}
						if (objects[j]->getColor() == yellow || objects[j]->getColor() == red) {
							objects[j]->setFalling(true);
						}
						else {
							objects[j]->setToRemove(true);
						}
						printf("Score: %d\n", score);
						arrows[i]->setToRemove(true);
						
					}
				}
			}
		}
	}
	
	for (int i = 0; i < shurikens.size(); ++i) {
		if (shurikens[i]->ColidesWith(bow->getType()) && !shurikens[i]->getToRemove()) {
			if (shurikens[i]->checkColision(bow)) {
				shurikens[i]->setToRemove(true);
				if (lives != 0) {
					lives--;
					printf("Lives: %d\n", lives);
				}
				if(lives == 0) {
					printf("Game Over!\n");
					printf("Score: %d\n", score);
					printf("Press Space to Reset!\n");
				}
			}
		}
	}
}

void Tema1::SpawnBaloons()
{
	if (baloons.size() != 0) {
		return;
	}
	int number = (rand() % 10 + 1);
	Baloon* baloon;
	for (int i = 0; i < number; i++) {
		float radius1 = 40;
		float radius2 = 50;
		int x = (rand() % 780 + 500);
		int y = 10;
		int lines = rand() % 4 + 3;
		int color = rand() % 2;
		float scale = 0.5f; //(rand() % 100 + 1) / 100.0f;

		if(color == 0)
			baloon = new Baloon(meshes["baloonRed"], radius1, radius2, glm::vec3(1, 0, 0), glm::vec2(x, y), lines);
		else 
			baloon = new Baloon(meshes["baloonYellow"], radius1, radius2, glm::vec3(1, 1, 0), glm::vec2(x, y), lines);
		baloon->setScale(0.5f + scale);
		objects.push_back(baloon);
		baloons.push_back(baloon);
	}
}

void Tema1::SpawnShurikens()
{
	if (shurikens.size() != 0) {
		return;
	}
	int number = (rand() % 3 + 1);
	
	Shuriken* shuriken;
	for (int i = 0; i < number; ++i) {
		int x = 1275;
		int y = (rand() % 700 + 1);
		float movementSpeed = (rand() % 5 + 1);
		shuriken = new Shuriken(meshes["shuriken"], 100, glm::vec3(0, 0, 0), glm::vec2(x, y), movementSpeed);
		objects.push_back(shuriken);
		shurikens.push_back(shuriken);
	}
}

void Tema1::Reload()
{
	if (!hasArrow) {
		hasArrow = true;
		currentArrow = new Arrow(meshes["arrow"], 100, 16, glm::vec3(0, 0, 0), bow->getPosition());
		objects.push_back(currentArrow);
	}
	

}

void Tema1::Reset()
{
	objects.clear();
	shurikens.clear();
	arrows.clear();
	baloons.clear();
	lives = 3;
	score = 0;
	bow->setPosition(50, 360);
	bow->setRotation(0);
	objects.push_back(bow);
	if (currentArrow != nullptr) {
		currentArrow->setPosition(50, 360);
		currentArrow->setRotation(0);
		objects.push_back(currentArrow);
	}
	else {
		Reload();
	}

}
