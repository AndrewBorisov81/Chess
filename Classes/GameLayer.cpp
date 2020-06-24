#include "GameLayer.h"
#include "Board.h"
#include "Grid.h"
#include "Constants.h"
#include "Figure.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

GameLayer::GameLayer()
{

}

GameLayer::~GameLayer()
{

}

bool GameLayer::init()
{
  if (!CCLayer::init()) {

    return false;
  }

  Size winSize = Director::getInstance()->getWinSize();
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  m_screenSize = visibleSize;

  // Create Board
  Board* board = createBoard();
  this->addChild(board, static_cast<int>(ZOrderGame::BOARD));
  m_board = board;
  board->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

  // Create Grid
  Grid* grid = createGrid(Constants::CELL_SIZE, Constants::ROWS, Constants::COLUMNS);
  board->addChild(grid, static_cast<int>(ZOrderGame::GRID));
  m_grid = grid;
  float deltaGridX = -(Constants::CELL_SIZE * Constants::COLUMNS / 2);
  float deltaGridY = -(Constants::CELL_SIZE * Constants::ROWS / 2);
  grid->setPosition(Vec2(-(Constants::CELL_SIZE * Constants::COLUMNS/2), -(Constants::CELL_SIZE * Constants::ROWS/2)));

  // Create Figure
  /*Figure* figure1 = Figure::createFigure(Constants::BLACK_ROOK_PNG);
  grid->addChild(figure1, static_cast<int>(ZOrderGame::FIGURE));
  figure1->setPosition(grid->getPointByCell(0, 0));
  });*/

  /*imageView->addClickEventListener([=](Ref*) {
    bool stop = true;
  });*/

  // Create Figure
  Figure* figure2 = Figure::createFigure(Constants::BLACK_HORSE_PNG);
  grid->addChild(figure2, static_cast<int>(ZOrderGame::FIGURE));
  figure2->setPosition(grid->getPointByCell(1, 0));
  figure2->setTouchEnabled(true);
  figure2->addClickEventListener([=](Ref*) {
    figure2->setRotation(45);
  });


  // Create Figure
  /*Figure* figure3 = Figure::createFigure(Constants::BLACK_OFFICER_PNG);
  grid->addChild(figure3, static_cast<int>(ZOrderGame::FIGURE));
  figure3->setPosition(grid->getPointByCell(2, 0));

  // Create Figure
  Figure* figure4 = Figure::createFigure(Constants::BLACK_QUEEN_PNG);
  grid->addChild(figure4, static_cast<int>(ZOrderGame::FIGURE));
  figure4->setPosition(grid->getPointByCell(3, 0));

  // Create Figure
  Figure* figure5 = Figure::createFigure(Constants::BLACK_KING_PNG);
  grid->addChild(figure5, static_cast<int>(ZOrderGame::FIGURE));
  figure5->setPosition(grid->getPointByCell(4, 0));

  // Create Figure
  Figure* figure6 = Figure::createFigure(Constants::BLACK_PAWN_PNG);
  grid->addChild(figure6, static_cast<int>(ZOrderGame::FIGURE));
  figure6->setPosition(grid->getPointByCell(4, 1));
  figure6->activateClickEvent();*/

  /*DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 20, 360, 20, true, 1, 1, Color4F::MAGENTA);
  figure1->addChild(drawnode);*/

  // Create the imageview
  /*ImageView* imageView = ImageView::create(Constants::BLACK_PAWN_PNG);
  imageView->setPosition(grid->getPointByCell(4, 1));
  grid->addChild(imageView, static_cast<int>(ZOrderGame::FIGURE));*/
 
  return true;
}

Board* GameLayer::createBoard()
{
  Board* pBoard = new(std::nothrow) Board();
  if (pBoard && pBoard->init())
  {
    pBoard->autorelease();
    return pBoard;
  }
  else
  {
    delete pBoard;
    pBoard = nullptr;
    return nullptr;
  }
}

Grid* GameLayer::createGrid(float cellSize, int rows, int columns)
{
  Grid* pGrid = new(std::nothrow) Grid(cellSize, rows, columns);
  if (pGrid && pGrid->init())
  {
    pGrid->autorelease();
    return pGrid;
  }
  else
  {
    delete pGrid;
    pGrid = nullptr;
    return nullptr;
  }
}



