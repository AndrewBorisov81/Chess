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
  PROMT = 201,
  PIECE = 301,
  TOUCH_AND_DRAG = 401,
  PROMOTION = 501,
  HUD = 601
};


#include "cocos2d.h"

#include <vector>

class Board;
class Grid;
class Piece;
class PieceMove;
class PieceMoveLogic;
class TouchAndDragLayer;
class PromotionLayer;
class PromptLayer;
class HudLayer;
class Connector;
class AILogic;

struct EnPassant;
struct Castling;
struct Promotion;
enum class Player;

class GameLayer : public cocos2d::Layer
{
public:
  GameLayer();
  virtual ~GameLayer();

  virtual bool init();

  void update(float);

  void makeTheMove(const cocos2d::Size& present, const cocos2d::Size& future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion);

  void movePiece(const cocos2d::Size& move_from, const cocos2d::Size& move_to);

  void promotionPiece(int typePromotion, bool isWhite, const cocos2d::Size& to);

  bool checkPieceMove(const cocos2d::Size& prevCellIJ, const cocos2d::Size& curCellIJ, PieceMove& pieceMove);

  void undoMove();

  TouchAndDragLayer* getTouchAndDragLayer();

  const std::vector<std::vector<Piece*>>& getPiece();

  Board* getBoard();

  Piece* createPieceFileName(int type, bool isWhite);

private:
  cocos2d::Size m_screenSize{ 0.0f, 0.0f };
  cocos2d::Size m_cellSize{ 0.0f, 0.0f };
  cocos2d::Size m_gridSize{ 0.0f, 0.0f };

  Board* m_board{ nullptr };
  Grid* m_grid{ nullptr };

  std::vector<std::vector<Piece*>> m_pieces;

  PieceMoveLogic* m_pieceMoveLogic{ nullptr };

  TouchAndDragLayer* m_touchAndDragLayer{ nullptr };
  PromptLayer* m_promptLayer{ nullptr };
  PromotionLayer* m_promotionLayer{ nullptr };
  HudLayer* m_hudLayer{ nullptr };
  Connector* m_connector{ nullptr };
  AILogic* m_AILogic{ nullptr };

  // Created
  Grid* createGrid(float cellSize, int rows, int columns);
  Board* createBoard(float cellSize, int rows, int columns);
  std::vector<std::vector<Piece*>> createPieces(const int piece_board[8][8], int rows, int columns);
  PieceMoveLogic* createPieceMoveLogic(GameLayer* gameField);
  PromptLayer* createPromptPieceLayer(float cellSize, int rows, int columns);
  TouchAndDragLayer* createTouchAndDragLayer(float cellSize, int rows, int columns);
  PromotionLayer* createPromotionLayer();
  HudLayer* createHudLayer();
  AILogic* createAILogic(Player turn);

  // Promotion
  void applyPromotion();

  //to delete
  bool m_isFirstMove{ true };
};