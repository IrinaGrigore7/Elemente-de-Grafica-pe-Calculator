#pragma once

#include "Object.h"
#include "Bow.h"
#include "Arrow.h"
#include "Baloon.h"
#include "Shuriken.h"
#include "ProgressBar.h"
#include "Heart.h"
#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>
#include <ctime>
#include <chrono>

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void CleanUp();
		void CheckCollisions();
		void SpawnBaloons();
		void SpawnShurikens();
		void Reload();
		void Reset();
		

	protected:
		std::vector <Object*> objects;
		Bow* bow;
		Arrow* currentArrow;
		ProgressBar* progressBar;
		std::vector <Arrow*> arrows;
		std::vector <Baloon*> baloons;
		std::vector <Shuriken*> shurikens;
		std::vector <Heart*> hearts;
		bool mousePress;
		bool hasArrow;
		int lives;
		int score;
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point reloadStartTime;
};