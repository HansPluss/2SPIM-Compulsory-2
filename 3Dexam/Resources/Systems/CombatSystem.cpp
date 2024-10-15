#include "CombatSystem.h"

CombatSystem::CombatSystem()
{
	damageCooldown = 0.5f;
	lastDamageTime = 0.0f;
}

void CombatSystem::Update(float deltaTime)
{
	lastDamageTime += deltaTime;
}

void CombatSystem::TakeDamage(Entity& entity, float damage)
{
	auto* healthComponent = entity.GetComponent<HealthComponent>();
	healthComponent->health -= damage;
	std::cout << "Health: " << healthComponent->health << std::endl;
	if (healthComponent->health <= 0) {
		
		entity.isMarkedForDeletion = true;
		
	}
}

void CombatSystem::DealDamage(Entity& attacker, Entity& defender)
{
	if (lastDamageTime >= damageCooldown) {
		auto* damageComponent = attacker.GetComponent<DamageComponent>();
		TakeDamage(defender, damageComponent->damage);
		lastDamageTime = 0.0f;
	}
	
	
}
