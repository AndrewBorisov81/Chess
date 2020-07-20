#include "GameLayer.h"
#include "Board.h"
#include "Grid.h"
#include "Figure.h"
#include "FiguresMoveLogic.h"
#include "TouchAndDragLayer.h"
#include "Constants.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <functional>

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

  // Copy figures_board
  std::copy(&Constants::INITIAL_FIGURES_BOARD[0][0], &Constants::INITIAL_FIGURES_BOARD[0][0]+Constants::ROWS*Constants::COLUMNS, &m_figures_board[0][0]);

  // Create Board
  Board* board = createBoard();
  this->addChild(board, static_cast<int>(ZOrderGame::BOARD));
  m_board = board;
  board->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

  // Create Grid
  Grid* grid = createGrid(Constants::CELL_SIZE, Constants::ROWS, Constants::COLUMNS);
  board->addChild(grid, static_cast<int>(ZOrderGame::GRID));
  m_grid = grid;
  board->addGrid(m_grid);
  float deltaGridX = -(Constants::CELL_SIZE * Constants::COLUMNS / 2);
  float deltaGridY = -(Constants::CELL_SIZE * Constants::ROWS / 2);
  grid->setPosition(Vec2(-(Constants::CELL_SIZE * Constants::COLUMNS/2), -(Constants::CELL_SIZE * Constants::ROWS/2)));

  // Create Figures
  m_figures = createFigures(Constants::INITIAL_FIGURES_BOARD, Constants::ROWS, Constants::COLUMNS);
  m_dataChess.figures = m_figures;
  // Load Figures
  board->loadAllFigures(m_figures, static_cast<int>(ZOrderGame::FIGURES));

  // Create TouchAndDragLayer
  TouchAndDragLayer* touchAndDragLayer = createTouchAndDrag(this, grid);
  board->addChild(touchAndDragLayer, static_cast<int>(ZOrderGame::TOUCH_AND_DRAG));
  m_touchAndDragLayer = touchAndDragLayer;
  touchAndDragLayer->setPosition(grid->getPosition());


  std::vector<std::vector<Figure*>> &figures = m_dataChess.figures;

  // Set callBack
  auto lfUpdateFiguresBoard = [&, this](Figure* figure, Size& prevPos, Size& newPos)->void
  {
    bool isMoveValid = this->checkFigureMove(figure, prevPos, newPos);

    if (isMoveValid)
    {
      updateBoardChess(figure, prevPos, newPos);
    }
    else
    {
      setBackFigureToPrevPos(figure, prevPos);
    } 
  };

  touchAndDragLayer->callBackUpdateBoardFigures(lfUpdateFiguresBoard);

  FiguresMoveLogic* figuresMoveLogic = createFiguresMoveLogic(this);
  m_figuresMoveLogic = figuresMoveLogic;

  // Create Test Figures
  //createTestFigures();

  /*m_mouseListener = EventListenerMouse::create();
  m_mouseListener->onMouseMove = CC_CALLBACK_1(GameLayer::onMouseMove, this);
  m_mouseListener->onMouseUp = CC_CALLBACK_1(GameLayer::onMouseUp, this);
  m_mouseListener->onMouseDown = CC_CALLBACK_1(GameLayer::onMouseDown, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(m_mouseListener, this);*/

  return true;
}

void GameLayer::update(float delta) {
  /*if (m_currentFigure) {
    m_currentFigure->setPosition(m_grid->getLocation());
  }*/
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

TouchAndDragLayer* GameLayer::createTouchAndDrag(GameLayer* gameLayer, Grid* grid)
{
  TouchAndDragLayer* pTouchAndDrag = new(std::nothrow) TouchAndDragLayer(gameLayer, grid);
  if (pTouchAndDrag && pTouchAndDrag->init())
  {
    pTouchAndDrag->autorelease();
    return pTouchAndDrag;
  }
  else
  {
    delete pTouchAndDrag;
    pTouchAndDrag = nullptr;
    return nullptr;
  }
}

FiguresMoveLogic* GameLayer::createFiguresMoveLogic(GameLayer* gameLayer)
{
  FiguresMoveLogic* pFiguresMoveLogic = new(std::nothrow) FiguresMoveLogic(gameLayer);
  if (pFiguresMoveLogic && pFiguresMoveLogic->init())
  {
    pFiguresMoveLogic->autorelease();
    return pFiguresMoveLogic;
  }
  else
  {
    delete pFiguresMoveLogic;
    pFiguresMoveLogic = nullptr;
    return nullptr;
  }
}

void GameLayer::setBackFigureToPrevPos(Figure* figure, const Size& prevPos)
{
  Vec2 prevPosFigure = m_grid->getPointByCell(int(prevPos.height), int(prevPos.width));
  figure->setPosition(prevPosFigure);
}

void GameLayer::setFigureToNewPos(Figure* figure, const cocos2d::Size& newPos)
{
  m_dataChess.figures[newPos.width][newPos.height] = figure;
}

DataChess& GameLayer::getDataChess()
{
  return m_dataChess;
}

void GameLayer::updateBoardChess(Figure* figure, const Size& prevPos, const Size& newPos)
{
  m_dataChess.figures[newPos.width][newPos.height] = figure;
  m_dataChess.figures[prevPos.width][prevPos.height] = 0;
}

void GameLayer::removeFigureBoard(const Size& pos)
{
  m_dataChess.figures[pos.width][pos.height] = 0;
}

TouchAndDragLayer* GameLayer::getTouchAndDragLayer()
{
  return m_touchAndDragLayer;
}

const std::vector<std::vector<Figure*>>& GameLayer::getFigures()
{
  return m_figures;
}

Figure* GameLayer::createFigureFileName(int type, bool isWhite)
{
  std::string fileName(Constants::WHITE_PAWN_PNG);
  TypeFigure typeFigure{TypeFigure::PAWN};

  switch (type)
  {
    case 1:
      typeFigure = TypeFigure::ROOK;
      fileName = (isWhite) ? Constants::WHITE_ROOK_PNG : Constants::BLACK_ROOK_PNG;
      break;

    case 2:
      typeFigure = TypeFigure::KNIGHT;
      fileName = (isWhite) ? Constants::WHITE_KNIGHT_PNG : Constants::BLACK_KNIGHT_PNG;
      break;

    case 3:
      typeFigure = TypeFigure::BISHOP;
      fileName = (isWhite) ? Constants::WHITE_BISHOP_PNG : Constants::BLACK_BISHOP_PNG;
      break;

    case 4:
      typeFigure = TypeFigure::QUEEN;
      fileName = (isWhite) ? Constants::WHITE_QUEEN_PNG : Constants::BLACK_QUEEN_PNG;
      break;

    case 5:
      typeFigure = TypeFigure::KING;
      fileName =(isWhite) ? Constants::WHITE_KING_PNG : Constants::BLACK_KING_PNG;
      break;

    case 6:
      typeFigure = TypeFigure::PAWN;
      fileName = (isWhite) ? Constants::WHITE_PAWN_PNG : Constants::BLACK_PAWN_PNG;
      break;
 }

  Figure* pFigure = Figure::createFigure(type, isWhite, fileName);
  pFigure->setType(typeFigure);

  return pFigure;
}

std::vector<std::vector<Figure*>> GameLayer::createFigures(const int figures_board[8][8], int rows, int columns)
{
  std::vector<std::vector<Figure*>> figures;
  Figure* pFigure{ nullptr };

  for (int i = 0; i < rows; i++)
  {
    std::vector<Figure*> row;
    for (int j = 0; j < columns; j++)
    {
      int figure = figures_board[i][j];

      int type{ abs(figure) };
      bool isWhite = (figure < 0) ? false : true;

      if (figure > 0 || figure < 0)
      {
        pFigure = createFigureFileName(type, isWhite);
        row.push_back(pFigure);
      }
      else
      {
        row.push_back(0);
      }
    }
    figures.push_back(row);
  }
  return figures;
}

void GameLayer::createTestFigures()
{
  Grid* grid = m_grid;

  // Create Figure
 /*Figure* figure1 = Figure::createFigure(TypeFigure::ROOK, Constants::BLACK_ROOK_PNG);
 grid->addChild(figure1, static_cast<int>(ZOrderGame::FIGURE));
 figure1->setPosition(grid->getPointByCell(0, 0));
 });*/

 // Create Figure
  //Figure* figure2 = Figure::createFigure(TypeFigure::HORSE, ColourFigure::WHITE, Constants::BLACK_HORSE_PNG);
  Figure* figure2 = Figure::createFigure(2, true, Constants::BLACK_KING_PNG);
  grid->addChild(figure2, static_cast<int>(ZOrderGame::FIGURES));
  figure2->setPosition(grid->getPointByCell(1, 0));
  figure2->setTouchEnabled(true);
  /*figure2->addClickEventListener([=](Ref*) {
    //figure2->setRotation(45);
    //m_currentFigure = figure2;
    if (m_grid->getCurrentFigure() == nullptr) {
      m_grid->setCurrentFigure(figure2);
    }
  });*/

  // Create Figure
  /*Figure* figure3 = Figure::createFigure(TypeFigure::OFFICER, Constants::BLACK_OFFICER_PNG);
  grid->addChild(figure3, static_cast<int>(ZOrderGame::FIGURE));
  figure3->setPosition(grid->getPointByCell(2, 0));

  // Create Figure
  Figure* figure4 = Figure::createFigure(TypeFigure::QUEEN, Constants::BLACK_QUEEN_PNG);
  grid->addChild(figure4, static_cast<int>(ZOrderGame::FIGURE));
  figure4->setPosition(grid->getPointByCell(3, 0));

  // Create Figure
  Figure* figure5 = Figure::createFigure(TypeFigure::KING, Constants::BLACK_KING_PNG);
  grid->addChild(figure5, static_cast<int>(ZOrderGame::FIGURE));
  figure5->setPosition(grid->getPointByCell(4, 0));

  // Create Figure
  Figure* figure6 = Figure::createFigure(TypeFigure::PAWN, Constants::BLACK_PAWN_PNG);
  grid->addChild(figure6, static_cast<int>(ZOrderGame::FIGURE));
  figure6->setPosition(grid->getPointByCell(4, 1));
  figure6->activateClickEvent();*/

  /*DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 20, 360, 20, true, 1, 1, Color4F::MAGENTA);
  figure1->addChild(drawnode);*/

}

bool GameLayer::checkFigureMove(Figure* figure, Size prevCellIJ, Size curCellIJ)
{
  EnPassant  S_enPassant = { 0 };
  Castling   S_castling = { 0 };
  Promotion  S_promotion = { 0 };

  Position present;
  present.iRow = (int)(prevCellIJ.width);
  present.iColumn = (int)(prevCellIJ.height);

  Position future;
  future.iRow = (int)(curCellIJ.width);
  future.iColumn = (int)(curCellIJ.height);

  //????????????????????????????????????????????????
  //????????????????????????????????????????????????
  m_figuresMoveLogic->m_gameLayer = this;
  m_figuresMoveLogic->m_currentTurn = 0;

  bool isMoveValid = m_figuresMoveLogic->isMoveValid(figure, present, future, &S_enPassant, &S_castling, &S_promotion);

  return isMoveValid;
}

/*void GameLayer::onMouseDown(Event* event)
{
  EventMouse* e = (EventMouse*)event;
}

void GameLayer::onMouseUp(Event* event)
{
  // to illustrate the event....
  EventMouse* e = (EventMouse*)event;
  //m_currentFigure = nullptr;
  m_delta.x = 0;
  m_delta.y = 0;
}

void GameLayer::onMouseMove(Event* event)
{
  //m_delta = e->getDelta();

  /*if (m_currentFigure)
  {
    EventMouse* e = (EventMouse*)event;
    Vec2 curFig(m_currentFigure->getPosition());
    m_currentFigure->setPosition(m_currentFigure->getPosition() + e->getDelta());
  }
}*/


