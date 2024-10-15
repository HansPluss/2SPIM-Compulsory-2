#pragma once
#include "Entity.h"
#include "Component.h"
class Item : public Entity {
public:
	Item() {
		AddComponent<PositionComponent>();
		AddComponent<RenderComponent>(glm::vec3(0.0f), glm::vec3(1.0f), "cube");
		AddComponent<CollisionComponent>();


	}
	void Pickup() {


	}

};