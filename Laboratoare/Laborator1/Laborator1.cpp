#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

bool var;
int contor = 0;
int contor_f_key = 0;
float miscare_axa_x = -1;
float miscare_axa_y = 0.5f;
float miscare_axa_z = 0;

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
		Mesh* mesh1 = new Mesh("sphere");
		mesh1->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh1->GetMeshID()] = mesh1;
		Mesh* mesh2 = new Mesh("quad");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "quad.obj");
		meshes[mesh2->GetMeshID()] = mesh2;
		Mesh* mesh3 = new Mesh("teapot");
		mesh3->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh3->GetMeshID()] = mesh3;
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	if (contor_f_key == 1) {
		glClearColor(0, 1, 0, 1);
	} else {
		glClearColor(0, 0, 0, 1);
	}

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(miscare_axa_x, miscare_axa_y, miscare_axa_z));

	// render the object again but with different properties
	//RenderMesh(meshes["sphere"], glm::vec3(2, 1, 0));

	switch (contor) {
	case 0: RenderMesh(meshes["sphere"], glm::vec3(2, 1, 0));
		break;
	case 1: RenderMesh(meshes["quad"], glm::vec3(2, 1, 0));
		break;
	case 2: RenderMesh(meshes["teapot"], glm::vec3(2, 1, 0));
		break;
	default: 
		break;
	}

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (window->KeyHold(GLFW_KEY_W)) {
		miscare_axa_x = miscare_axa_x + deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		miscare_axa_x = miscare_axa_x - deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_Q)) {
		miscare_axa_y = miscare_axa_y + deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_E)) {
		miscare_axa_y = miscare_axa_y - deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		miscare_axa_z = miscare_axa_z - deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		miscare_axa_z = miscare_axa_z + deltaTime;
	}
		

};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F && contor_f_key == 0) {
		var = true;
		contor_f_key = 1;
	} else if (key == GLFW_KEY_F && contor_f_key == 1) {
		var = false;
		contor_f_key = 0;
	}

	if (key == GLFW_KEY_X && contor != 3) {
		contor++;
	}

	if (key == GLFW_KEY_X && contor == 3) {
		contor = 0;
	}
	
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event

};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
