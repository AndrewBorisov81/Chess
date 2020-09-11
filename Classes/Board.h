#pragma once

#include "Grid.h"

#include "cocos2d.h"

#include <vector>

class Piece;
class Grid;

class Board : public Grid
{
public:
  Board();
  Board(float cellSize, int rows, int columns);
  virtual ~Board();

  virtual bool init();

  virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);

  void loadAllPieces(std::vector<std::vector<Piece*>>& piece, int zOrder);

  void addPieceN(int type, bool isWhite, const cocos2d::Size& cell, int zOrder);
  void removePieceN(const cocos2d::Size& cell);
  void movePieceFromToN(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell);
  void updatePieceCellN(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell);

  Piece* getPieceFromCell(int row, int column);

  cocos2d::Size m_boardSize;

  Piece* createPieceFileName(int type, bool isWhite);

private:
  Grid* m_grid{ nullptr };

  std::vector<std::vector<Piece*>> m_pieces;

  std::vector<Piece*> m_whitePieces;
  std::vector<Piece*> m_blackPieces;

  cocos2d::Label* m_labelTouchInfo;
};
