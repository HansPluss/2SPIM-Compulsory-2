#include "Grid.h"
#include "iostream"



Grid::Grid(int width, int height, int cellSize) : m_width(width), m_height(height), m_cellSize(cellSize)
{
	m_numXCells = ceil(static_cast<float>(m_width) / cellSize);
	m_numYCells = ceil(static_cast<float>(m_height) / cellSize);
	m_cells.resize(m_numYCells * m_numXCells);



}

Grid::~Grid()
{
}

void Grid::AddBaLL(Draw* ball)
{
	Cell* cell = getCell(ball->GetPosition()); 
	cell->balls.push_back(ball); 
	ball->ownerCell = cell;
	ball->cellvectorindex = cell->balls.size() - 1; 
}

void Grid::AddBaLL(Draw* ball, Cell* cell)
{
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellvectorindex = cell->balls.size() - 1;
}

Cell* Grid::getCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells;

	return &m_cells[y * m_numXCells + x];
}

Cell* Grid::getCell(const glm::vec3& pos)
{
	if (pos.x < 0 || pos.x >= m_width || pos.z < 0 || pos.z >= m_height)
		return &m_cells[0];

	int cellX = pos.x / m_cellSize;
	int cellY = pos.z / m_cellSize;

	return getCell(cellX, cellY);
}



void Grid::RemoveBallFromCell(Draw* ball)
{
    if (!ball->ownerCell || ball->ownerCell->balls.empty())
        return;

    if (ball->cellvectorindex < 0 || ball->cellvectorindex >= static_cast<int>(ball->ownerCell->balls.size()))
        return;

    if (!ball->ownerCell->balls.empty())
    {
		//std::cout << ball->cellvectorindex << std::endl;
        ball->ownerCell->balls[ball->cellvectorindex] = ball->ownerCell->balls.back();
        ball->ownerCell->balls.pop_back();
        if (ball->cellvectorindex < ball->ownerCell->balls.size())
        {
            ball->ownerCell->balls[ball->cellvectorindex]->cellvectorindex = ball->cellvectorindex;
        }
    }
    ball->cellvectorindex = -1;
    ball->ownerCell = nullptr;
}


