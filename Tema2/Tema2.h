#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Platform.h"
#include "Player.h"
#include "Rectangle.h"
#include "Life.h"
#include <vector>
#include <chrono>

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		void GenerateMap();
		void CleanUp();
		void GenerateRow(float z);
		Platform* GetCurrentPlatform(glm::fvec3 position);
		void UpdatePlayerEffect();
		void MoveLeft();
		void MoveRight();
		void DrawUI();
		void DrawLives();
		float map(float x, float in_min, float in_max, float out_min, float out_max)
		{
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}

		glm::fvec3 mapColor(float r, float g, float b) {
			return glm::fvec3(map(r, 0, 255, 0, 1), map(g, 0, 255, 0, 1), map(b, 0, 255, 0, 1));
		}


	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		bool renderCameraTarget;
		bool projectionType;
		std::vector <Platform*> platforms;
		bool needRow;
		std::chrono::steady_clock::time_point startTime;
		float spawnChance;
		float maxChance;
		float harmChance;
		float harmMax;
		Player* player;
		float xOffset;
		int numberOfPlatforms;
		int currentColumn;
		float startX;
		bool isFirstPerson;
		glm::fvec3 initialPos;
		Rectangle2D* bg;
		Rectangle2D* progress;
		Life* heart;

		float right;
		float left;
		float bottom;
		float top;
		float fov;
		float Z_FAR;
		float Z_NEAR;
};
