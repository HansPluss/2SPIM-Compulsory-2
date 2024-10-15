#pragma once
#include "../../Entity.h"
class CombatSystem {
public:
	CombatSystem();
	void Update(float deltaTime);
	void DealDamage(Entity& attacker, Entity& defender);
	void TakeDamage(Entity& entity, float damage);


private:
	float damageCooldown;  // Time between damage applications
	float lastDamageTime;  // Tracks time since last damage
	



};