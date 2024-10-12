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

    void CleanupEntities(std::vector<Entity*>& entities) {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [](Entity* entity) {
                return entity->isMarkedForDeletion;  // Delete marked entities
            }), entities.end());
    }

    void MarkForDeletion(Entity& entity) {
        entity.isMarkedForDeletion = true;

    }
    std::vector<std::unique_ptr<Entity>>& GetEntities() {
        return entities;
    }
private:
    std::vector<std::unique_ptr<Entity>> entities;

};