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
    template<typename T, typename... Args>
    T& CreateEntityDerivedFromClass(Args&&... args) {
        static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
        entities.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return *static_cast<T*>(entities.back().get());
    }
    void DeleteEntities(std::vector<Entity*>& entities) {
        for (auto it = entities.begin(); it != entities.end();) {
            if ((*it)->isMarkedForDeletion) {
                
                it = entities.erase(it);
            }
            else {
                ++it; 
            }
        }

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