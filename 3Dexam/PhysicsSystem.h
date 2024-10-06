#pragma once
#include "Draw.h"
#include "Entity.h"
#include "Grid.h"
class grid;
class PhysicsSystem {
public:
	Draw draw;
	
	void Update(Entity& entity,float deltTime, Grid *grid) {
		auto* positionComponent = entity.GetComponent<PositionComponent>();
		auto* velocityComponent = entity.GetComponent<VelocityComponent>();
		auto* accelerationComponent = entity.GetComponent<AccelerationComponent>();
		if (positionComponent && velocityComponent) {
			draw.Update(deltTime, grid);

		}
	}
	void ApplyForce(Entity& entity, glm::vec3 force) {
		auto* velocityComponent = entity.GetComponent<VelocityComponent>();
		if (velocityComponent) {

			draw.ApplyForce(force);
		}
	}

};