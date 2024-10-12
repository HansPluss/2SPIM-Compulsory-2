#pragma once
#include "Entity.h"
#include <memory>
class EntityManager {
public:
    EntityManager() = default;


    Entity& CreateEntity() {
        entities.push_back(std::make_unique<Entity>());
        return *entities.back();
    }

    void CleanupEntities(std::vector<Entity> entities) {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [](const std::unique_ptr<Entity>& entity) {
                //return entity->isMarkedForDeletion;  // Delete marked entities
            }), entities.end());
    }

private:
    std::vector<std::unique_ptr<Entity>> entities;

};