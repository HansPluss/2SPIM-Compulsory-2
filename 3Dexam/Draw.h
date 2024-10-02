#pragma once
#include <vector>
#include "Resources/Shaders/VAO.h"
#include "Resources/Shaders/VBO.h"
#include "Resources/Shaders/EBO.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/glm.hpp"


class Shader;
struct Cell; 
class Grid; 
struct Vertex
{

	float x, y, z;
	float r, g, b;
	float u, v;
	float normalx, normaly, normalz;


};
class Draw {
public:
	Draw();

	//|-----------------------------------------------------------------------------|
	//|									Initalizing									|
	//|-----------------------------------------------------------------------------|
	void DrawCube(glm::vec3 Color, glm::vec3 pos, glm::vec3 size);
	void DrawPlane(glm::vec3 Color, glm::vec3 pos, glm::vec3 size);
	void DrawBoundingBox(glm::vec3 Color, glm::vec3 pos, glm::vec3 size);
	void DrawSphere(glm::vec3 Color, glm::vec3 pos, glm::vec3 size);

	//|-----------------------------------------------------------------------------|
	//|									Public Functions							|
	//|-----------------------------------------------------------------------------|		
	void Render(Shader shader, glm::mat4 viewproj);
	void Update(float deltaTime, Grid* grid);
	void ApplyForce(glm::vec3 force);
	void MoveXdir();
	void Delete();
	void RotateCube(float deltaTime);
	void CalculateGravity(float inclineAngle, glm::vec3 slopeVector, glm::vec3 normal);
	void FollowPlayer(Draw& ball, float speed);

	//|-----------------------------------------------------------------------------|
	//|									Getters										|
	//|-----------------------------------------------------------------------------|

	glm::vec3 GetPosition();
	float GetMass();
	glm::vec3 GetVelocity();
	glm::vec3 GetNormal();
	glm::vec3 GetAngularVelocity();
	glm::vec3 GetSize();
	std::vector<Vertex> GetVertices() { return vertices; };
	float GetGravity() { return gravity; };

	//|-----------------------------------------------------------------------------|
	//|									Setters										|
	//|-----------------------------------------------------------------------------|

	void SetPosition(glm::vec3 newPos);
	void SetMass(float newMass);
	void SetVelocity(glm::vec3 newVelocity);
	void SetNormalVector(glm::vec3 normal);
	void SetAngularVelocity(glm::vec3 angularVelocity);
	void SetGravity(float newGravity) { gravity = newGravity;}

	//|-----------------------------------------------------------------------------|
	//|									Grid										|
	//|-----------------------------------------------------------------------------|

	//what cell in the grid it's located
	Cell* ownerCell = nullptr; 
	//what position in the cellvector it's located
	int cellvectorindex = -1; 

private:
	//|-----------------------------------------------------------------------------|
	//|									Private Functions							|
	//|-----------------------------------------------------------------------------|
	void Initalize();
	//|-----------------------------------------------------------------------------|
	//|								Private variables								|
	//|-----------------------------------------------------------------------------|
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::mat4 rotation = glm::mat4(1.0f);
	glm::vec3 objSize = glm::vec3(1, 1, 1);
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 Acceleration = glm::vec3(0, 0, 0);
	glm::vec3 AngularVelocity = glm::vec3(0, 0, 0);
	glm::vec3 AngularAcceleration = glm::vec3(0, 0, 0);
	glm::vec3 normalvector = glm::vec3(0, 0, 0);
	float mass = 1.0f;
	float gravity = -9.81;

	//|-----------------------------------------------------------------------------|
	//|								Class initalizing								|
	//|-----------------------------------------------------------------------------|
	VAO VAO;
	VBO VBO;
	EBO EBO1;

	//|-----------------------------------------------------------------------------|
	//|									??????										|
	//|-----------------------------------------------------------------------------|

	glm::quat Quaternion = glm::quat(1.0, 0.0, 0.0, 0.0);

};