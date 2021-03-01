#pragma once

#include <string>
#include "Obj.h"

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <chrono>

struct PlayerEffect
{

	enum EffectType
	{
		None = 0,
		Fuel = 1,
		LoseFuel = 2,
		SpeedLock = 4,
		Died = 8,
		God = 16
	};
	EffectType type;
	float duration;
	std::chrono::steady_clock::time_point startTime;
	PlayerEffect() {
		type = None;
		duration = 0;
	}

	PlayerEffect(EffectType type_, float duration_) {
		type = type_;
		duration = duration_;
		startTime = std::chrono::steady_clock::now();
	}

	bool HasEnded() {
		auto end = std::chrono::steady_clock::now();
		int seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - startTime).count() / 1000;
		if (seconds > duration) {
			return true;
		} 
		return false;
	}
};

class Player : public Obj
{

public:

	static Mesh* Create(std::string name);

	Player() {
		name = "player";
		position = glm::vec3(0, 0, 0);
		mesh = nullptr;
		color = glm::vec3(1, 1, 1);
		movementSpeed = 0.05f;
		acceleration = 0.001f;
		jumping = false;
		maxJump = 2;
		fuel = 100;
		dead = false;
		lives = 3;
		needRespawn = false;
		dieing = false;
	}

	Player(Mesh* mesh_, glm::vec3 color_, glm::vec3 position_) : Obj(mesh_, position_, color_) {
		name = "player";
		movementSpeed = 0.05f;
		acceleration = 0.001f;
		jumping = false;
		maxJump = 1.5;
		fuel = 100;
		dead = false;
		lives = 3;
		needRespawn = false;
		dieing = false;
	}

	void setTranslateX(float value) {
		translateX = value;
	}

	void setTranslateZ(float value) {
		translateZ = value;
	}

	float getTranslateX() {
		return translateX;
	}

	float getMovementSpeed() {
		return movementSpeed;
	}

	void setMovementSpeed(float value) {
		movementSpeed = value;
	}

	float getAcceleration() {
		return acceleration;
	}

	void setAcceleration(float value) {
		acceleration = value;
	}

	void setGroundLevel(float value) {
		groundLevel = value;
	}

	float getFuel() {
		return fuel;
	}

	void Update(int n);
	void Jump() {
		if (dead || dieing) {
			return;
		}
		if (IsOnGround()) {
			jumping = true;
		}
		
	}
	bool IsOnGround() {
		return position.y <= groundLevel;
	}
	void Accelerate() {
		if (hasEffect(PlayerEffect::EffectType::SpeedLock)) {
			return;
		}
		if (!dead && !dieing) {
			movementSpeed += acceleration;
		}
	}
	void Decelerate() {
		if (hasEffect(PlayerEffect::EffectType::SpeedLock)) {
			return;
		}
		if (!dead && !dieing) {
			movementSpeed -= acceleration;
		}
	}
	
	bool IsDead() {
		return dead;
	}
	void Die(){
		if (dieing)
			return;
		if (needRespawn)
			return;
		if (lives > 1) {
			needRespawn = true;
			return;
		}
		dieing = true;
		position.y = 1.5f;
		printf("You died!\n");
	}
	void ApplyEffect(PlayerEffect::EffectType type) {

		switch (type)
		{
		case PlayerEffect::None:
			break;
		case PlayerEffect::Fuel: {
			fuel += 3;
			if (fuel > 100) {
				fuel = 100;
			}
		}
			break;
		case PlayerEffect::LoseFuel: {
			if (hasEffect(PlayerEffect::God)) {
				return;
			}
			fuel -= 1.0f;
			if (fuel < 0) {
				dead = true;
			}
		}
			break;
		case PlayerEffect::SpeedLock: {
			if (hasEffect(PlayerEffect::God)) {
				return;
			}
			if (!hasEffect(PlayerEffect::SpeedLock)) {
				effects.emplace_back(PlayerEffect::SpeedLock, 3);
				oldMovementSpeed = movementSpeed;
				movementSpeed = 0.15f;
			}
			else {
				getEffect(PlayerEffect::SpeedLock).duration += 3;
			}
		}
			break;
		case PlayerEffect::Died: {
			if (hasEffect(PlayerEffect::God)) {
				return;
			}
			lives = 0;
			Die();
		}
			break;
		case PlayerEffect::God: {
			if (!hasEffect(PlayerEffect::God)) {
				effects.clear();
				effects.emplace_back(PlayerEffect::God, 7);
			}
			else {
				getEffect(PlayerEffect::God).duration += 7;
			}
		}
			break;
		default:
			break;
		}

	}

	bool hasEffect(PlayerEffect::EffectType effect) {
		for (int i = 0; i < effects.size(); ++i) {
			if (effects[i].type == effect) {
				return true;
			}
		}
		return false;
	}
	PlayerEffect& getEffect(PlayerEffect::EffectType effect) {
		PlayerEffect result;
		for (int i = 0; i < effects.size(); ++i) {
			if (effects[i].type == effect) {
				return effects[i];
			}
		}
		return result;
	}

	bool hasEffects() {
		return effects.size() != 0;
	}

	bool isGod() {
		return hasEffect(PlayerEffect::God);
	}

	void Respawn(glm::fvec3 pos) {
		effects.clear();
		fuel = 100;
		lives--;
		position = pos;
		movementSpeed = 0.05f;
		needRespawn = false;
		printf("respawn\n");
	}

	int getLives() {
		return lives;
	}

	bool needsRespawn() {
		return needRespawn;
	}

	bool isDieing() {
		return dieing;
	}

protected:
	float translateX;
	float translateZ;
	float movementSpeed;
	float oldMovementSpeed;
	float acceleration;
	float groundLevel;
	float maxJump;
	bool jumping;
	float fuel;
	std::vector<PlayerEffect> effects;
	bool dead;
	int lives;
	bool needRespawn;
	bool dieing;

	void CleanUp();
};