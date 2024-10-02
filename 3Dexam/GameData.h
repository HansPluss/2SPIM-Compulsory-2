#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "glm/glm.hpp"
#include <vector>
struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

};
template <typename T>
struct Component {
    T data;
};

class GameData {
public:
    std::vector<Transform> transforms;  // Store positions/velocities for all objects together
    template <typename T>


    void AddComponent(T componentData);
    // Add new object data
    void AddEntity(Transform& transform);
    
};

template<typename T>
inline void GameData::AddComponent(T componentData)
{
    
}
