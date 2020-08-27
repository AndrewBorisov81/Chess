#pragma once

#include "cocos2d.h"

#include <vector>

class Piece;
class Grid;

class Board : public cocos2d::Layer
{
public:
  Board();
  virtual ~Board();

  virtual bool init();

  virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
  virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);

  void loadAllPieces(std::vector<std::vector<Piece*>>& piece, int zOrder);

  void addPiece(Piece* piece, cocos2d::Size& cellIJ, int zOrder);
  void removePiece(Piece* piece);
  void movePieceTo(Piece* piece, const cocos2d::Size& newPos);

  void addGrid(Grid* grid);

  cocos2d::Size m_boardSize;

private:
  Grid* m_grid{ nullptr };
  std::vector<std::vector<Piece*>> m_pieces;

  std::vector<Piece*> m_whitePieces;
  std::vector<Piece*> m_blackPieces;

  cocos2d::Label* m_labelTouchInfo;
};
