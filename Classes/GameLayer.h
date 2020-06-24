#pragma once

enum class GameMode
{
  HOME,
  PLAY,
  END,
  PLAY_AGAIN
};

enum class GameState
{
  STARTING,
  PREPARING,
  READY,
  PAUSE,
  FINISH,
  END
};

enum class ZOrderGame
{
  BOARD = 101,
  GRID = 202,
  FIGURE = 303
};

#include "cocos2d.h"

class Board;
class Grid;

class GameLayer : public cocos2d::Layer
{
public:
  GameLayer();
  virtual ~GameLayer();

  virtual bool init();

private:
  Board* m_board{ nullptr };
  Grid* m_grid{ nullptr };

  cocos2d::Size m_screenSize{ 0.0f, 0.0f };
  cocos2d::Size m_cellSize{ 0.0f, 0.0f };
  cocos2d::Size m_gridSize{ 0.0f, 0.0f };

  Board* createBoard();
  Grid* createGrid(float cellSize, int rows, int columns);
  void createTestFigures();
};
