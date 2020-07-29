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
  FIGURES = 303,
  TOUCH_AND_DRAG = 404,
  HUD = 505
};


#include "cocos2d.h"

class Board;
class Grid;
class Figure;
class TouchAndDragLayer;
class FiguresMoveLogic;
class HudLayer;

struct EnPassant;
struct Castling;
struct Promotion;

struct DataChess
{
  std::vector<std::vector<Figure*>> figures;
};

class GameLayer : public cocos2d::Layer
{
public:
  GameLayer();
  virtual ~GameLayer();

  virtual bool init();

  void update(float);

  void updateBoardChess(Figure* figure, const cocos2d::Size& prevPos, const cocos2d::Size& newPos);
  void removeFigureBoard(const cocos2d::Size& pos);
  void setFigureToNewPos(Figure* figure, const cocos2d::Size& newPos);
  void moveFigureToPos(Figure* figure, const cocos2d::Size& pos);

  void makeTheMove(const cocos2d::Size& present, const cocos2d::Size& future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);
  void moveFigure(const cocos2d::Size& move_from, const cocos2d::Size& move_to);

  bool checkFigureMove(Figure* figure, cocos2d::Size prevCellIJ, cocos2d::Size curCellIJ);

  void undoMove();

  TouchAndDragLayer* getTouchAndDragLayer();
  const std::vector<std::vector<Figure*>>& getFigures();
  DataChess& getDataChess();
  Board* getBoard();

private:
  cocos2d::Size m_screenSize{ 0.0f, 0.0f };
  cocos2d::Size m_cellSize{ 0.0f, 0.0f };
  cocos2d::Size m_gridSize{ 0.0f, 0.0f };

  Board* m_board{ nullptr };
  Grid* m_grid{ nullptr };

  std::vector<std::vector<Figure*>> m_figures;
  DataChess m_dataChess;
  int m_figures_board[8][8];

  FiguresMoveLogic* m_figuresMoveLogic{ nullptr };
  TouchAndDragLayer* m_touchAndDragLayer{ nullptr };
  HudLayer* m_hudLayer{ nullptr };

  Grid* createGrid(float cellSize, int rows, int columns);
  TouchAndDragLayer* createTouchAndDrag(GameLayer* gameLayer, Grid* grid);
  Board* createBoard();

  std::vector<std::vector<Figure*>> createFigures(const int figures_board[8][8], int rows, int columns);
  Figure* createFigureFileName(int type, bool isWhite);
  FiguresMoveLogic* createFiguresMoveLogic(GameLayer* gameField);
  HudLayer* createHudLayer();

  void setBackFigureToPrevPos(Figure* figure, const cocos2d::Size& prevPos);

  //void updateFiguresBoard(DataChess& dataChess, const cocos2d::Size& oldPositionIJ, const cocos2d::Size& newPositionIJ);

  void createTestFigures();

  /*cocos2d::EventListenerMouse* m_mouseListener;

  void onMouseDown(cocos2d::Event* event);
  void onMouseUp(cocos2d::Event* event);
  void onMouseMove(cocos2d::Event* event);

  cocos2d::Vec2 m_delta{};

  Figure* m_currentFigure{ nullptr };*/
};