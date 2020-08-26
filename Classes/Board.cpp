#include "Board.h"
#include "Constants.h"
#include "Grid.h"
#include "Piece.h"

#include "base/CCEventListener.h"
#include "base/CCEvent.h"

USING_NS_CC;

Board::Board()
{
}

Board::~Board()
{

}

bool Board::init()
{
  if (!CCLayer::init()) {

    return false;
  }

  // Board image
  auto boardSprite = Sprite::createWithSpriteFrameName(Constants::BOARD_PNG);
  Size boardSize = boardSprite->getContentSize();
  m_boardSize = boardSize;
  this->addChild(boardSprite, 1);

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

void Board::loadAllPiece(std::vector<std::vector<Piece*>>& pieces, int zOrder)
{
  for (int i = 0; i < pieces.size(); i++)
  {
    std::vector<Piece*> row = pieces[i];
    for (int j = 0; j < row.size(); j++)
    {
      if (row[j] != 0)
      {
        Piece* pPiece = row[j];
        m_grid->addChild(pPiece, zOrder);
        pPiece->setPosition(Vec2(m_grid->getPointByCell(j, i)));
      }
    }
  }
}

void Board::addPiece(Piece* piece, Size& cellIJ, int zOrder)
{
  piece->setPosition(Vec2(m_grid->getPointByCell(cellIJ.height, cellIJ.width)));
  m_grid->addChild(piece, zOrder);
}

void Board::removePiece(Piece* piece)
{
  m_grid->removeChild(piece);
}

void Board::addGrid(Grid* grid)
{
  m_grid = grid;
}

void Board::movePieceTo(Piece* piece, const cocos2d::Size& newPos)
{
  Vector<Node*> allNodes = this->getChildren();
  for (auto& node : allNodes) {
    if (dynamic_cast<Piece*>(node) == piece) { //It is Sprite 
      Piece* target = dynamic_cast<Piece*>(node);
      target->setPosition(m_grid->getPointByCell(newPos.width, newPos.height));
    }
  }
}

bool Board::onTouchBegan(Touch* touch, Event* event)
{
  bool stop = true;
  /*float x = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).x;
  float y = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).y;
  m_labelTouchInfo->setPosition(Vec2(x, y));*/


  /*Size cellIJ{};
  float cellSize{ m_cellSize };*/

  //calculateCellByPoint(Vec2(x, y), cellSize, cellIJ);

  //m_labelTouchInfo->setString("You Touched Here" + std::to_string(int(cellIJ.width)) + std::to_string(int(cellIJ.height)));

  return true;
}

void Board::onTouchEnded(Touch* touch, Event* event)
{
  cocos2d::log("touch ended");
}

void Board::onTouchMoved(Touch* touch, Event* event)
{
  bool stop = true;




  /*if (m_currentPiece) {
    m_currentPiece->setPosition(m_currentPiece->getPosition() + touch->getDelta());
  }*/


  /*m_delta.x = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).x;
  m_delta.y = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).y;
  cocos2d::log("touch moved");*/
  //m_labelTouchInfo->setString(std::to_string(int(m_location.x))+ " , " + std::to_string(int(m_location.y)));
  /*m_labelTouchInfo->setString(std::to_string(int(m_delta.x))+ " , " + std::to_string(int(m_delta.y)));
  if (m_currentPiece) {
    m_curPosPiece.x = m_prevPosPiece.x + m_delta.x;
    m_curPosPiece.y = m_prevPosPiece.y + m_delta.y;
    m_currentPiece->setPosition(m_curPosPiece);
  }*/
}

