#include "Board.h"
#include "Constants.h"
#include "Grid.h"
#include "Piece.h"

#include "base/CCEventListener.h"
#include "base/CCEvent.h"

#include <algorithm>
#include <iterator>

USING_NS_CC;

Board::Board()
{
}

Board::Board(float cellSize, int rows, int columns)
  : Grid(cellSize, rows, columns)
{
}

Board::~Board()
{

}

bool Board::init()
{
  Grid::init();

  if (!CCLayer::init()) {

    return false;
  }

  // Board image
  auto boardSprite = Sprite::createWithSpriteFrameName(Constants::BOARD_PNG);
  Size boardSize = boardSprite->getContentSize();
  m_boardSize = boardSize;
  this->addChild(boardSprite, 1);

  float deltaGridX = -(Constants::CELL_SIZE * Constants::COLUMNS / 2);
  float deltaGridY = -(Constants::CELL_SIZE * Constants::ROWS / 2);
  boardSprite->setPosition(Vec2((Constants::CELL_SIZE * Constants::COLUMNS / 2), (Constants::CELL_SIZE * Constants::ROWS / 2)));

  /*DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 50, 360, 20, true, 1, 1, Color4F::RED);
  boardSprite->addChild(drawnode);*/

  Label* labelTouchInfo = Label::createWithSystemFont("Touch or clicksomewhere to begin", "Arial", 30);
  m_labelTouchInfo = labelTouchInfo;

  labelTouchInfo->setPosition(Vec2(
    Director::getInstance()->getVisibleSize().width / 2,
    Director::getInstance()->getVisibleSize().height / 2));

  auto touchListener = EventListenerTouchOneByOne::create();
  touchListener->onTouchBegan = CC_CALLBACK_2(Board::onTouchBegan, this);
  touchListener->onTouchEnded = CC_CALLBACK_2(Board::onTouchEnded, this);
  touchListener->onTouchMoved = CC_CALLBACK_2(Board::onTouchMoved, this);
  touchListener->onTouchCancelled = CC_CALLBACK_2(Board::onTouchCancelled, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

  return true;
}

void Board::loadAllPieces(std::vector<std::vector<Piece*>>& pieces, int zOrder)
{
  for (unsigned int i = 0; i < pieces.size(); i++)
  {
    std::vector<Piece*> row = pieces[i];

    for (unsigned int j = 0; j < row.size(); j++)
    {
      if (row[j] != 0)
      {
        Piece* pPiece = row[j];

        pPiece->setCell(Size(i, j));

        this->addChild(pPiece, zOrder);

        pPiece->setPosition(Vec2(getPointByCell(j, i)));

        if (pPiece->isWhite())
          m_whitePieces.push_back(pPiece);
        else
          m_blackPieces.push_back(pPiece);
      }
    }
  }
}

void Board::addPieceN(int type, bool isWhite, const cocos2d::Size& cell, int zOrder)
{
  Piece* piece = createPieceFileName(type, isWhite);

  piece->setPosition(getPointByCell(cell.height, cell.width));

  piece->setCell(cell);

  this->addChild(piece, zOrder);

  if (isWhite)
  {
    m_whitePieces.push_back(piece);
  }
  else
  {
    m_blackPieces.push_back(piece);
  }
}

void Board::removePieceN(const cocos2d::Size& cell)
{
  Piece* piece = getPieceFromCell(cell.width, cell.height);

  this->removeChild(piece);
}

void Board::movePieceFromToN(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)
{
  Piece* piece = getPieceFromCell(presentCell.width, presentCell.height);

  if (piece)
  {
    piece->setPosition(getPointByCell(futureCell.height, futureCell.width));

    piece->setCell(futureCell);
  }
}

void Board::updatePieceCellN(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)
{
  Piece* piece = getPieceFromCell(presentCell.width, presentCell.height);
  if(piece)
    piece->setCell(futureCell);
}

Piece* Board::getPieceFromCell(int row, int column)
{
  for (auto el : m_blackPieces)
  {
    Size cellPiece = el->getCell();

    if (cellPiece.width == row && cellPiece.height == column)
      return el;
  }

  for (auto el : m_whitePieces)
  {
    Size cellPiece = el->getCell();

    if (cellPiece.width == row && cellPiece.height == column)
      return el;
  }

  return nullptr;
}

bool Board::onTouchBegan(Touch* touch, Event* event)
{
  return true;
}

void Board::onTouchEnded(Touch* touch, Event* event)
{
  //cocos2d::log("touch ended");
}

void Board::onTouchMoved(Touch* touch, Event* event)
{

}

Piece* Board::createPieceFileName(int type, bool isWhite)
{
  std::string fileName(Constants::WHITE_PAWN_PNG);
  TypePiece typePiece{ TypePiece::PAWN };

  switch (type)
  {
  case 1:
    typePiece = TypePiece::ROOK;
    fileName = (isWhite) ? Constants::WHITE_ROOK_PNG : Constants::BLACK_ROOK_PNG;
    break;

  case 2:
    typePiece = TypePiece::KNIGHT;
    fileName = (isWhite) ? Constants::WHITE_KNIGHT_PNG : Constants::BLACK_KNIGHT_PNG;
    break;

  case 3:
    typePiece = TypePiece::BISHOP;
    fileName = (isWhite) ? Constants::WHITE_BISHOP_PNG : Constants::BLACK_BISHOP_PNG;
    break;

  case 4:
    typePiece = TypePiece::QUEEN;
    fileName = (isWhite) ? Constants::WHITE_QUEEN_PNG : Constants::BLACK_QUEEN_PNG;
    break;

  case 5:
    typePiece = TypePiece::KING;
    fileName = (isWhite) ? Constants::WHITE_KING_PNG : Constants::BLACK_KING_PNG;
    break;

  case 6:
    typePiece = TypePiece::PAWN;
    fileName = (isWhite) ? Constants::WHITE_PAWN_PNG : Constants::BLACK_PAWN_PNG;
    break;
  }

  Piece* pPiece = Piece::createPiece(type, isWhite, fileName);
  pPiece->setType(typePiece);

  return pPiece;
}

