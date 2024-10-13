#pragma once
#include "Entity.h"
#include "Component.h"
class Projectile : public Entity {
public:
	Projectile() {
		AddComponent<PositionComponent>();
		AddComponent<RenderComponent>(glm::vec3(0.0f), glm::vec3(1.0f), "sphere");
	}
	void MoveProjectile(Entity& basePosition) {
		auto* position = basePosition.GetComponent<PositionComponent>();
		GetComponent<PositionComponent>()->position = position->position;



	}


};