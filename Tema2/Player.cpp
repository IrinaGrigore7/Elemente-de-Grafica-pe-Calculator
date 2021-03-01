#include "Player.h"
#include <include/glm.h>

#include <Core/Engine.h>

Mesh* Player::Create(std::string name) {
	Mesh* mesh = new Mesh(name);
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	return mesh;
}

void Player::Update(int n)
{
	if (dead) {
		return;
	}
	CleanUp();
	fuel -= 0.01;
	if (fuel < 0) {
		dead = true;
	}
	if (dieing) {
		position.z -= 0.015f;
	}
	if (jumping) {
		position.y += 0.015f;
	}
	else {
		position.y -= 0.015f;
	}
	if (position.y >= maxJump) {
		jumping = false;
	}
	if (position.y < groundLevel && !dieing) {
		position.y = groundLevel;
	}
	if (position.y < -3 * groundLevel && dieing) {
		dead = true;
	}
	
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
}

void Player::CleanUp()
{
	std::vector <int> toRemove;

	for (int i = 0; i < effects.size(); ++i) {
		if (effects[i].HasEnded()) {
			if (effects[i].type == PlayerEffect::EffectType::SpeedLock) {
				movementSpeed = oldMovementSpeed;
			}
			toRemove.push_back(i);
		}
	}

	for (int i = toRemove.size() - 1; i >= 0; --i) {
		effects.erase(effects.begin() + toRemove[i]);
	}
}





