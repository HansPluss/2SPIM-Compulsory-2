#pragma once
#include "vector"
class Draw; 
class Grid; 

class Collision

{
public:

	Collision();
	~Collision();
	//|-----------------------------------------------------------------------------|
	//|									Public Functions							|
	//|-----------------------------------------------------------------------------|
	void UpdateCollision(Grid* grid, float dt); 
	bool AABBCollision(Draw &objA, Draw &objB, float DeltaTime);
	bool InvAABBCollision(Draw& objA, Draw& objB, float DeltaTime);
	
	void calculateBarycentricCoordinates(Draw& ball, Draw& drawObject);
private:
	//|-----------------------------------------------------------------------------|
	//|									Private Functions							|
	//|-----------------------------------------------------------------------------|
	bool SphereCollison(Draw& objA, Draw& objB, float DeltaTime);
	void CollisionCalculations(Draw& objA, Draw& objB, float DeltaTime);
	void ObjectCollisionResponse(Draw& objA, Draw& objB);
	void CheckCollision(Draw* ball, std::vector<Draw*>& BallToCheck, int startingIndex, float dt);
	
};

