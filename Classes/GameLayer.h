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
  FIGURE = 303,
  TOUCH_AND_DRAG = 404,
};

#include "cocos2d.h"

class Board;
class Grid;
class Figure;
class TouchAndDragLayer;
class FiguresMoveLogic;

class GameLayer : public cocos2d::Layer
{
public:
  GameLayer();
  virtual ~GameLayer();

  virtual bool init();

  void update(float);

  TouchAndDragLayer* getTouchAndDragLayer();
  const std::vector<std::vector<Figure*>>& getFigures();

  bool checkFigureMove(cocos2d::Size prevCellIJ, cocos2d::Size curCellIJ);

private:
  cocos2d::Size m_screenSize{ 0.0f, 0.0f };
  cocos2d::Size m_cellSize{ 0.0f, 0.0f };
  cocos2d::Size m_gridSize{ 0.0f, 0.0f };

  int m_figures_board[8][8];
  Board* m_board{ nullptr };
  Grid* m_grid{ nullptr };
  FiguresMoveLogic* m_figuresMoveLogic{ nullptr };
  TouchAndDragLayer* m_touchAndDragLayer{ nullptr };
  std::vector<std::vector<Figure*>> m_figures;

  Board* createBoard();
  Grid* createGrid(float cellSize, int rows, int columns);
  std::vector<std::vector<Figure*>> createFigures(const int figures_board[8][8], int rows, int columns);
  Figure* createFigureFileName(int type, bool isWhite);
  TouchAndDragLayer* createTouchAndDrag(std::vector<std::vector<Figure*>>& figures, GameLayer* gameLayer, Grid* grid);
  FiguresMoveLogic* createFiguresMoveLogic(GameLayer* gameField);

  void createTestFigures();

  /*cocos2d::EventListenerMouse* m_mouseListener;

  void onMouseDown(cocos2d::Event* event);
  void onMouseUp(cocos2d::Event* event);
  void onMouseMove(cocos2d::Event* event);

  cocos2d::Vec2 m_delta{};

  Figure* m_currentFigure{ nullptr };*/
};