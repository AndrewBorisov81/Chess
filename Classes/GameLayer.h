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
  PIECE = 303,
  TOUCH_AND_DRAG = 404,
  PROMOTION = 505,
  HUD = 606
};


#include "cocos2d.h"

#include <vector>

class Board;
class Grid;
class Piece;
class TouchAndDragLayer;
class PieceMoveLogic;
class PromotionLayer;
class HudLayer;

struct EnPassant;
struct Castling;
struct Promotion;

struct DataChess
{
  std::vector<std::vector<Piece*>> pieces;
  std::vector<std::vector<int>> board;
};

class GameLayer : public cocos2d::Layer
{
public:
  GameLayer();
  virtual ~GameLayer();

  virtual bool init();

  void update(float);

  void updateBoard(int typePiece, const cocos2d::Size& presentCell, const cocos2d::Size& futureCell);

  // Represent the piece in the board
  void removePieceBoard(const cocos2d::Size& presentCell);
  void movePieceBoard(int typePiece, const cocos2d::Size& presentCell, const cocos2d::Size& futureCell);
  void addPieceBoard(int typePiece, cocos2d::Size& futureCell);

  void setPieceToNewPos(Piece* piece, const cocos2d::Size& futureCell);

  void movePieceToPos(Piece* piece, const cocos2d::Size& futureCell);

  void makeTheMove(const cocos2d::Size& present, const cocos2d::Size& future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);
  void movePiece(const cocos2d::Size& move_from, const cocos2d::Size& move_to);

  bool checkPieceMove(Piece* piece, const cocos2d::Size& prevCellIJ, const cocos2d::Size& curCellIJ);

  void undoMove();

  TouchAndDragLayer* getTouchAndDragLayer();
  const std::vector<std::vector<Piece*>>& getPiece();
  DataChess& getDataChess();
  Board* getBoard();

  Piece* createPieceFileName(int type, bool isWhite);

private:
  cocos2d::Size m_screenSize{ 0.0f, 0.0f };
  cocos2d::Size m_cellSize{ 0.0f, 0.0f };
  cocos2d::Size m_gridSize{ 0.0f, 0.0f };

  Board* m_board{ nullptr };
  Grid* m_grid{ nullptr };

  std::vector<std::vector<Piece*>> m_pieces;
  DataChess m_dataChess;
  int m_piece_board[8][8];

  //Promotion
  int m_lastPromotedPiece;

  PieceMoveLogic* m_pieceMoveLogic{ nullptr };

  TouchAndDragLayer* m_touchAndDragLayer{ nullptr };
  PromotionLayer* m_promotionLayer{ nullptr };
  HudLayer* m_hudLayer{ nullptr };

  Grid* createGrid(float cellSize, int rows, int columns);
  Board* createBoard(float cellSize, int rows, int columns);

  std::vector<std::vector<Piece*>> createPiece(const int piece_board[8][8], int rows, int columns);
  PieceMoveLogic* createPieceMoveLogic(GameLayer* gameField);

  TouchAndDragLayer* createTouchAndDragLayer(Grid* grid);
  PromotionLayer* createPromotionLayer();
  HudLayer* createHudLayer();

  void setBackPieceToPrevPos(Piece* piece, const cocos2d::Size& prevPos);

  // Promotion
  int applyPromotion(int typePiece);
  void movePromotion(cocos2d::Size& present, cocos2d::Size& future, Promotion& promotion, int typePromotionPiece);

  void createTestPiece();

  /*cocos2d::EventListenerMouse* m_mouseListener;

  void onMouseDown(cocos2d::Event* event);
  void onMouseUp(cocos2d::Event* event);
  void onMouseMove(cocos2d::Event* event);

  cocos2d::Vec2 m_delta{};

  Piece* m_currentPiece{ nullptr };*/
};