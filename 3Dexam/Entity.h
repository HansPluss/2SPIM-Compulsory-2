#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <iostream>
#include <glm/glm.hpp>
#include "Component.h"

class Entity {
	

public:
	int id;
	bool isMarkedForDeletion;
	int GetId() {
		return id;
	}
	int SetId(int newId) {

		id = newId;
	}
	Entity() {
		id = idCounter++;
		isMarkedForDeletion = false;
		

	}
	template<typename T, typename... Args>
	void AddComponent(Args&&... args) {
		components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...);
	}
	template<typename T>
	T* GetComponent() {
		auto it = components.find(typeid(T));
		return (it != components.end()) ? static_cast<T*>(it->second.get()) : nullptr;
	}
	
private:
	static int idCounter;
	std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};