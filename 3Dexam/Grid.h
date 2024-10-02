#pragma once
#include "vector"
#include "Draw.h"

struct Cell {
	std::vector<Draw*> balls;
};
class Grid
{
	friend class Collision;
public: 
	Grid(int width, int height, int cellSize); 
	~Grid(); 
	//|-----------------------------------------------------------------------------|
	//|								Public Functions								|
	//|-----------------------------------------------------------------------------|
	void AddBaLL(Draw* ball);
	void AddBaLL(Draw* ball, Cell* cell);
	void RemoveBallFromCell(Draw* ball);
	//|-----------------------------------------------------------------------------|
	//|								Getters											|
	//|-----------------------------------------------------------------------------|

	Cell* getCell(int x, int y);
	Cell* getCell(const glm::vec3& pos);

private: 
	//|-----------------------------------------------------------------------------|
	//|								Private variables								|
	//|-----------------------------------------------------------------------------|
	std::vector<Cell> m_cells; 
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells; 
};

