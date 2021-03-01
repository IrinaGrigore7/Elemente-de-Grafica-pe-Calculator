#include "Tema2.h"
#include "Obj.h"
#include "Platform.h"
#include "Rectangle.h"
#include "LabCamera.h"
#include "Life.h"


#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	renderCameraTarget = false;
	projectionType = true;

	needRow = false;
	startTime = std::chrono::steady_clock::now();
	spawnChance = 5;
	maxChance = 30;
	harmChance = 10;
	harmMax = 30;
	xOffset = 1.1f;
	numberOfPlatforms = 5;
	currentColumn = std::ceil(numberOfPlatforms / 2);
	isFirstPerson = false;
	

	right = 10.f;
	left = .01f;
	bottom = .01f;
	top = 10.f;
	fov = 60.f;
	Z_FAR = 100.f;
	Z_NEAR = .01f;


	
		 
	AddMeshToList(Platform::Create("platform"));
	AddMeshToList(Player::Create("player"));
	AddMeshToList(Rectangle2D::Create("background", glm::vec3(1, 1, 1), 200, 30));
	AddMeshToList(Rectangle2D::Create("pBar", glm::vec3(1, 0, 0), 196, 26));
	AddMeshToList(Life::Create("heart", 7, glm::vec3(1, 0, 0), true));
	heart = new Life(meshes["heart"], 7, glm::vec3(1, 0, 0), glm::vec2(25, 710));
	player = new Player(meshes["player"], mapColor(122, 127, 128), glm::vec3(0, 0.5f, 0));
	startX = -numberOfPlatforms / 2 * xOffset;
	player->setPosition(startX + currentColumn * xOffset, 2, 0);
	player->setGroundLevel(0.76f);
	initialPos = player->getPosition();
	bg = new Rectangle2D(meshes["background"], glm::vec3(1, 1, 1), glm::vec2(10, 10), 200, 30, 100, 0);
	progress = new Rectangle2D(meshes["pBar"], glm::vec3(1, 0, 0), glm::vec2(12, 12), 196, 26, 100, 0);
	progress->setProgress(100);
	bg->setProgress(100);
	bg->Update(0);
		
	{
		Shader* shader = new Shader("Shader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 1.75f, 3.0f),player->getPosition(), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
		if (player->IsDead()) {
			return;
		}
		if (player->needsRespawn()) {
			Platform* plat;
			for (int i = 0; i < numberOfPlatforms; ++i) {
				glm::fvec3 pos(startX + i * xOffset, 0.4f, 0);
				plat = GetCurrentPlatform(pos);
				if (plat != nullptr && plat->isGood()) {
					player->Respawn(pos);
					currentColumn = i;
					break;
				}
			}
		}
		if (!player->hasEffects()) {
			player->setColor(mapColor(122, 127, 128));
		}
		player->setPosition(startX + currentColumn * xOffset, player->getPosition().y, player->getPosition().z);
		player->Update(0);
		auto end = std::chrono::steady_clock::now();
		int seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - startTime).count() / 100;
		if (seconds > 5) {
			startTime = end;
			spawnChance += 0.1f;
			if (spawnChance > maxChance) {
				spawnChance = maxChance;
			}
		}

		GenerateMap();
		if (!isFirstPerson) {
			RenderSimpleMesh(player->getMesh(), shaders["Shader"], player->getModelMatrix(), player->getColor());
		}
		int chance = rand() % 100 + 1;
		for (int i = 0; i < platforms.size(); ++i) {
			if (seconds > 5 && chance > 90) {
				platforms[i]->Accelerate();
			}
			if (!player->isDieing()) {
				platforms[i]->setMovementSpeed(player->getMovementSpeed());
			}
			else {
				platforms[i]->setMovementSpeed(0);
			}
			platforms[i]->Update(i);
			RenderSimpleMesh(platforms[i]->getMesh(), shaders["Shader"], platforms[i]->getModelMatrix(), platforms[i]->getColor());
		}
		UpdatePlayerEffect();
		CleanUp();
		if (needRow) {
			needRow = false;
			float z = platforms[platforms.size() - 1]->getPosition().z - 3;
			GenerateRow(z);
		}
		DrawUI();
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
	//DrawCoordinatSystem();
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int modelLocation = glGetUniformLocation(shader->program, "Model");
	

	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	int viewLocation = glGetUniformLocation(shader->program, "View");

	// TODO : set shader uniform "View" to viewMatrix
	if (isFirstPerson) {
		glm::fvec3 pos = player->getPosition();
		pos.y += 0.25f;
		glm::fvec3 forward = pos;
		forward.z -= 3;
		camera->Set(player->getPosition(), forward, glm::vec3(0, 1, 0));
	}
	else {
		camera->Set(glm::vec3(0, 1.75f, 3.0f), initialPos, glm::vec3(0, 1, 0));
	}
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	int projectionLocation = glGetUniformLocation(shader->program, "Projection");
	
	// TODO : set shader uniform "Projection" to projectionMatrix
	//glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int colorLocation = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(colorLocation, 1, glm::value_ptr(color));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		player->Accelerate();
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		player->Decelerate();
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event

	if (key == GLFW_KEY_A) {
		// TODO : translate the camera to the left
		//camera->TranslateRight(-deltaTime * cameraSpeed);
		MoveLeft();
	}
	if (key == GLFW_KEY_D) {
		// TODO : translate the camera to the right
		//camera->TranslateRight(deltaTime * cameraSpeed);
		MoveRight();
	}
	if (key == GLFW_KEY_SPACE) {
		player->Jump();
	}

	if (key == GLFW_KEY_C) {
		isFirstPerson = !isFirstPerson;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_F) {
		printf("fuel: %f\n", player->getFuel());
	}
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

void Tema2::GenerateMap()
{
	if (platforms.size() > 0) {
		return;
	}
	
	for (int i = 0; i < 7; ++i) {
		float z = -3 * i;
		GenerateRow(z);
	}
}

void Tema2::CleanUp()
{
	std::vector <int> toRemove;

	for (int i = 0; i < platforms.size(); ++i) {
		if (platforms[i]->getPosition().z > 7.5f) {
			toRemove.push_back(i);
		}
	}
	if (toRemove.size() != 0) {
		needRow = true;
	}

	for (int i = toRemove.size() - 1; i >= 0; --i) {
		platforms.erase(platforms.begin() + toRemove[i]);
	}
}

void Tema2::GenerateRow(float z)
{
	Platform* platform = nullptr;
	float startX = -numberOfPlatforms / 2 * xOffset;
	float movementSpeed = 0.05f;
	float acceleration = 0.002f;
	float auxMovementSpeed = 0;
	if (platforms.size() > 0) {
		movementSpeed = platforms[platforms.size() - 1]->getMovementSpeed();
		acceleration = platforms[platforms.size() - 1]->getAcceleration();
		auxMovementSpeed = platforms[platforms.size() - 1]->getAuxMovementSpeed();
	}
	for (int j = 0; j < numberOfPlatforms; ++j) {
			int color = rand() % 5 + 1;
			int chance = rand() % 100 + 1;
			
			if (chance < 100.0f - spawnChance) {
				int harm = rand() % 100 + 1;
				if (harm < 100.0f - harmChance) {
					int fuelChance = rand() % 100 + 1;
					if (fuelChance < 85) {
						platform = new Platform(meshes["platform"], mapColor(47, 141, 255), glm::vec3(0, 0, 0));
						platform->setType(Platform::None);

					}
					else {
						if (fuelChance < 97) {
							platform = new Platform(meshes["platform"], mapColor(144, 238, 144), glm::vec3(0, 0, 0));
							platform->setType(Platform::Fuel);
						}
						else {
							platform = new Platform(meshes["platform"], mapColor(86, 19, 223), glm::vec3(0, 0, 0));
							platform->setType(Platform::God);
						}
						
					}
				}
				else {
					int harmLevel = rand() % 100 + 1;
					if (harmLevel < 70) {
						platform = new Platform(meshes["platform"], glm::vec3(1, 1, 0), glm::vec3(0, 0, 0));
						platform->setType(Platform::LoseFuel);
					} else{
						if (harmLevel < 85) {
							platform = new Platform(meshes["platform"], glm::vec3(1, 0.5f, 0), glm::vec3(0, 0, 0));
							platform->setType(Platform::LockSpeed);
						}
						else {
							platform = new Platform(meshes["platform"], glm::vec3(1, 0, 0), glm::vec3(0, 0, 0));
							platform->setType(Platform::Dead);
						}

					}
				}
				float currentX = startX + j * xOffset;
				platform->setPosition(currentX, 0.5f, z);
				platform->setMovementSpeed(movementSpeed);
				platform->setAcceleration(acceleration);
				platform->setAuxMovementSpeed(auxMovementSpeed);
				platforms.push_back(platform);
			}
	}
}

Platform* Tema2::GetCurrentPlatform(glm::fvec3 position)
{
	for (int i = 0; i < platforms.size(); ++i) {
		if (platforms[i]->Intersect(position, 0.25)) {
			return platforms[i];
		}
	}
	return nullptr;
}

void Tema2::UpdatePlayerEffect()
{
	if (!player->IsOnGround()) {
		return;
	}
	if (player->isDieing() || player->IsDead()) {
		return;
	}
	Platform* currentPlatform = GetCurrentPlatform(player->getPosition());
	if (currentPlatform != nullptr) {
		switch (currentPlatform->getType())
		{
		case Platform::Fuel:
			player->ApplyEffect(PlayerEffect::Fuel);
			break;
		case Platform::LoseFuel:
			player->ApplyEffect(PlayerEffect::LoseFuel);
			break;
		case Platform::Dead:
			player->ApplyEffect(PlayerEffect::Died);
			break;
		case Platform::LockSpeed:
			player->ApplyEffect(PlayerEffect::SpeedLock);
			if (!player->isGod()) {
				player->setColor(glm::vec3(1, 0.5f, 0));
			}
			break;
		case Platform::God:
			player->ApplyEffect(PlayerEffect::God);
			player->setColor(mapColor(86, 19, 223));
			break;
		default:
			break;
		}
		currentPlatform->setType(Platform::Visited);
		currentPlatform->setColor(mapColor(113, 104, 205));
	}
	else {
		player->Die();
	}
}

void Tema2::MoveLeft()
{
	if (player->IsDead()) {
		return;
	}
	if (!player->IsOnGround()) {
		return;
	}
	if (player->isDieing()) {
		return;
	}
	int nextColumn = currentColumn - 1;
	if (nextColumn < 0) {
		nextColumn = 0;
	}
	currentColumn = nextColumn;


}

void Tema2::MoveRight()
{
	if (player->IsDead()) {
		return;
	}
	if (!player->IsOnGround()) {
		return;
	}
	if (player->isDieing()) {
		return;
	}
	int nextColumn = currentColumn + 1;
	if (nextColumn > numberOfPlatforms) {
		nextColumn = numberOfPlatforms;
	}
	currentColumn = nextColumn;
}

void Tema2::DrawUI()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera2D = GetSceneCamera();
	camera2D->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera2D->SetPosition(glm::vec3(0, 0, 50));
	camera2D->SetRotation(glm::vec3(0, 0, 0));
	camera2D->Update();
	glClearColor(0.9, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
	DrawLives();
	progress->setProgress(player->getFuel());
	progress->Update(0);
	RenderMesh2D(progress->getMesh(), shaders["VertexColor"], progress->getModelMatrix());
	RenderMesh2D(bg->getMesh(), shaders["VertexColor"], bg->getModelMatrix());
	
}

void Tema2::DrawLives()
{
	int initialX = 25;
	for (int i = 0; i < player->getLives(); ++i) {
		glm::vec2 pos = heart->getPosition();
		heart->setPosition(initialX, pos.y);
		initialX += 35;
		heart->Update(0);
		RenderMesh2D(heart->getMesh(), shaders["VertexColor"], heart->getModelMatrix());
	}
}
