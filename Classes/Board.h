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

  void loadAllPiece(std::vector<std::vector<Piece*>>& piece, int zOrder);
  void addPiece(Piece* piece, cocos2d::Size& cellIJ, int zOrder);
  void removePiece(Piece* piece);
  void addGrid(Grid* grid);

  void movePieceTo(Piece* piece, const cocos2d::Size& newPos);

  cocos2d::Size m_boardSize;

private:
  Grid* m_grid{ nullptr };
  std::vector<std::vector<Piece*>> m_pieces;

  cocos2d::Label* m_labelTouchInfo;
};
