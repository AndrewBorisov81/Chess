#include "GameLayer.h"
#include "Board.h"
#include "Grid.h"
#include "Figure.h"
#include "FiguresMoveLogic.h"
#include "TouchAndDragLayer.h"
#include "PromotionLayer.h"
#include "HudLayer.h"
#include "Constants.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <cmath>
#include <string>
#include <stdlib.h>

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

  //Bg
  LayerColor* bgColor = LayerColor::create(Color4B(84, 33, 30, 255));
  this->addChild(bgColor, -10);

  // Load the Sprite Sheet
  auto spritecache = SpriteFrameCache::getInstance();
  spritecache->addSpriteFramesWithFile("texture.plist");

  Size winSize = Director::getInstance()->getWinSize();
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  m_screenSize = visibleSize;

  // Copy figures_board
  std::copy(&Constants::INITIAL_FIGURES_BOARD[0][0], &Constants::INITIAL_FIGURES_BOARD[0][0] + Constants::ROWS*Constants::COLUMNS, &m_figures_board[0][0]);

  // Board presentation
  for (int i = 0; i < Constants::ROWS; i++)
  {
    std::vector<int> row;
    for (int j = 0; j < Constants::COLUMNS; j++)
    {
      row.push_back(0);
    }
    m_dataChess.board.push_back(row);
  }

  // Board presentation
  for (int i = 0; i < Constants::ROWS; i++)
  {
    for (int j = 0; j < Constants::COLUMNS; j++)
    {
      m_dataChess.board[i][j] = Constants::INITIAL_FIGURES_BOARD[i][j];
    }
  }

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

  // Create FiguresMoveLogic
  FiguresMoveLogic* pFiguresMoveLogic = createFiguresMoveLogic(this);
  m_figuresMoveLogic = pFiguresMoveLogic;
  this->addChild(pFiguresMoveLogic, 1);

  // Create Promotion Layer
  PromotionLayer* pPromotionLayer = createPromotionLayer();
  m_promotionLayer = pPromotionLayer;
  this->addChild(pPromotionLayer, static_cast<int>(ZOrderGame::PROMOTION));
  pPromotionLayer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

  // Set callBack
  auto lfClickFigure = [this](int typeFigure)
  {
    this->applyPromotion(typeFigure);
  };

  pPromotionLayer->callBackClickFigure(lfClickFigure);

  // Create TouchAndDragLayer
  TouchAndDragLayer* touchAndDragLayer = createTouchAndDragLayer(this, grid);
  board->addChild(touchAndDragLayer, static_cast<int>(ZOrderGame::TOUCH_AND_DRAG));
  m_touchAndDragLayer = touchAndDragLayer;
  touchAndDragLayer->setPosition(grid->getPosition());

  // Create HudLayer
  HudLayer* pHudLayer = createHudLayer();
  this->addChild(pHudLayer, static_cast<int>(ZOrderGame::HUD));
  m_hudLayer = pHudLayer;
  auto lfUndoMove = [this]() {
    this->undoMove();
  };
  pHudLayer->callBackUndoLastMove(lfUndoMove);

  std::vector<std::vector<Figure*>> &figures = m_dataChess.figures;

  // Set callBack
  auto lfUpdateFiguresBoard = [this](Figure* figure, Size& prevPos, Size& newPos)->void
  {
    //moveFigure(prevPos, newPos);
    bool isMoveValid = this->checkFigureMove(figure, prevPos, newPos);

    if (isMoveValid)
    {
      moveFigure(prevPos, newPos);
      //updateBoardChess(figure, prevPos, newPos);
    }
    else
    {
      setBackFigureToPrevPos(figure, prevPos);
    } 
  };

  touchAndDragLayer->callBackUpdateBoardFigures(lfUpdateFiguresBoard);

  // Create Test Figures
  //createTestFigures();

  /*m_mouseListener = EventListenerMouse::create();
  m_mouseListener->onMouseMove = CC_CALLBACK_1(GameLayer::onMouseMove, this);
  m_mouseListener->onMouseUp = CC_CALLBACK_1(GameLayer::onMouseUp, this);
  m_mouseListener->onMouseDown = CC_CALLBACK_1(GameLayer::onMouseDown, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(m_mouseListener, this);*/

  // test
   // add "HelloWorld" splash screen"
  /*auto sprite = Sprite::createWithSpriteFrameName(Constants::WHITE_ROOK_PNG);
 
  // position the sprite on the center of the screen
  sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x + 150, visibleSize.height / 2 + origin.y));

  // add the sprite as a child to this layer
  this->addChild(sprite, 1000);*/

  /*Figure* testFigure = createFigureFileName(1, true);
  m_board->addFigure(testFigure, Size(4,5), 1001);

  Figure* cloneFigure = testFigure->cloneFigure();
  m_board->addFigure(cloneFigure, Size(4, 6), 1001);

  m_board->removeFigure(testFigure);

  cloneFigure->setPosition(m_grid->getPointByCell(3, 3));*/

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

TouchAndDragLayer* GameLayer::createTouchAndDragLayer(GameLayer* gameLayer, Grid* grid)
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

PromotionLayer* GameLayer::createPromotionLayer()
{
  PromotionLayer* pPromotionLayer = new(std::nothrow) PromotionLayer();
  if (pPromotionLayer && pPromotionLayer->init())
  {
    pPromotionLayer->autorelease();
    return pPromotionLayer;
  }
  else
  {
    delete pPromotionLayer;
    pPromotionLayer = nullptr;
    return nullptr;
  }
}

HudLayer* GameLayer::createHudLayer()
{
  HudLayer* pHudLayer = new(std::nothrow) HudLayer();
  if (pHudLayer && pHudLayer->init())
  {
    pHudLayer->autorelease();
    return pHudLayer;
  }
  else
  {
    delete pHudLayer;
    pHudLayer = nullptr;
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

int GameLayer::applyPromotion(int typeFigure)
{
  m_promotionLayer->hide();
  m_lastPromotedFigure = typeFigure;

  return typeFigure;
}

void GameLayer::movePromotion(Size& present, Size& future, Promotion& promotion, int typePromotionFigure)
{
  std::string to_record;

  Position ppresent;
  ppresent.iRow = present.width;
  ppresent.iColumn = present.height;
  
  Position pfuture;
  pfuture.iRow = future.width;
  pfuture.iColumn = future.height;

  std::string presentStr = m_figuresMoveLogic->parseMoveCellIntToString(ppresent);
  std::string futureStr = m_figuresMoveLogic->parseMoveCellIntToString(pfuture);

  to_record += presentStr;
  to_record += futureStr;

  /*cout << "Promote to (Q, R, N, B): ";
    std::string piece;
    getline(cin, piece);

    if (piece.length() > 1)
    {
      createNextMessage("You should type only one character (Q, R, N or B)\n");
      return;
    }

    char chPromoted = toupper(piece[0]);

    if (chPromoted != 'Q' && chPromoted != 'R' && chPromoted != 'N' && chPromoted != 'B')
    {
      createNextMessage("Invalid character.\n");
      return;
    }

    S_promotion.chBefore = current_game->getPieceAtPosition(present.iRow, present.iColumn);

    if (Chess::WHITE_PLAYER == current_game->getCurrentTurn())
    {
      S_promotion.chAfter = toupper(chPromoted);
    }
    else
    {
      S_promotion.chAfter = tolower(chPromoted);
    }

    to_record += '=';
    to_record += toupper(chPromoted); // always log with a capital letter
    */

  Figure* figure = m_figuresMoveLogic->getFigureAtPosition(present.width, present.height);
  //promotion.figureBefore = figure->cloneFigure();
  promotion.figureBefore = figure;
  promotion.typeBefore = static_cast<int>(figure->getType());

  Figure* promotedFigure{ nullptr };

  if(static_cast<int>(Player::WHITE_PLAYER) == m_figuresMoveLogic->getCurrentTurn())
  {
    //S_promotion.chAfter = toupper(chPromoted);
    promotedFigure = createFigureFileName(typePromotionFigure, true);
    //promotion.figureAfter = promotedFigure->cloneFigure();
    promotion.figureAfter = promotedFigure;
    promotion.typeAfter = static_cast<int>(promotedFigure->getType());
    promotion.isWhite = true;
  }
  else
  {
    //S_promotion.chAfter = tolower(chPromoted);
    promotedFigure = createFigureFileName(typePromotionFigure, false);
    //promotion.figureAfter = promotedFigure->cloneFigure();
    promotion.figureAfter = promotedFigure;
    promotion.typeAfter = static_cast<int>(promotedFigure->getType());
    promotion.isWhite = false;
  }

  /*removeFigureBoard(Size(present.width, present.height));
  m_board->removeFigure(figure);

  Figure* promotionFigureAfter = createFigureFileName(promotion.typeAfter, promotion.isWhite);
  //setFigureToNewPos(promotionFigureAfter, Size(present.width, present.height));
  setFigureToNewPos(promotionFigureAfter, Size(future.width, future.height));
  m_board->addFigure(promotionFigureAfter, Size(future.width, future.height), static_cast<int>(ZOrderGame::FIGURES));*/

  // ---------------------------------------------------
  // Log the move: do it prior to making the move
  // because we need the getCurrentTurn()
  // ---------------------------------------------------
  m_figuresMoveLogic->logMove(to_record);

  // ---------------------------------------------------
  // Make the move
  // ---------------------------------------------------
  // Is that move allowed?
  EnPassant  S_enPassant = { 0 };
  Castling   S_castling = { 0 };
  //makeTheMove(Size(present.iRow, present.iColumn), Size(future.iRow, future.iColumn), &S_enPassant, &S_castling, &S_promotion);
  makeTheMove(present, future, &S_enPassant, &S_castling, &promotion);

  // ---------------------------------------------------------------
  // Check if this move we just did put the oponent's king in check
  // Keep in mind that player turn has already changed
  // ---------------------------------------------------------------
  if (true == m_figuresMoveLogic->playerKingInCheck())
  {
    if (true == m_figuresMoveLogic->isCheckMate())
    {
      if (static_cast<int>(Player::WHITE_PLAYER) == m_figuresMoveLogic->getCurrentTurn())
      {
        //appendToNextMessage("Checkmate! Black wins the game!\n");
      }
      else
      {
        //appendToNextMessage("Checkmate! White wins the game!\n");
      }
    }
    else
    {
      // Add to the string with '+=' because it's possible that
      // there is already one message (e.g., piece captured)
      if (static_cast<int>(Player::WHITE_PLAYER) == m_figuresMoveLogic->getCurrentTurn())
      {
        //appendToNextMessage("White king is in check!\n");
      }
      else
      {
        //appendToNextMessage("Black king is in check!\n");
      }
    }
  }
}

void GameLayer::moveFigureToPos(Figure* figure, const Size& pos)
{
  Vec2 prevPosFigure = m_grid->getPointByCell(int(pos.height), int(pos.width));
  figure->setPosition(prevPosFigure);
}

void GameLayer::setFigureToNewPos(Figure* figure, const cocos2d::Size& newPos)
{
  m_dataChess.figures[newPos.width][newPos.height] = figure;
}

void GameLayer::moveFigure(const Size& move_from, const Size& move_to)
{
  std::string to_record;

  // Get user input for the piece they want to move
  /*cout << "Choose piece to be moved. (example: A1 or b2): ";

  std::string move_from;
  getline(cin, move_from);

  if (move_from.length() > 2)
  {
    createNextMessage("You should type only two characters (column and row)\n");
    return;
  }*/

  Position present;
  present.iColumn = move_from.height;
  present.iRow = move_from.width;

  std::string presentStr = m_figuresMoveLogic->parseMoveCellIntToString(present);

  //Put in the string to be logged
  to_record += presentStr;
  //to_record += "-";

  // ---------------------------------------------------
  // Did the user pick a valid piece?
  // Must check if:
  // - It's inside the board (A1-H8)
  // - There is a piece in the square
  // - The piece is consistent with the player's turn
  // ---------------------------------------------------
  /*present.iColumn = toupper(present.iColumn);

  if (present.iColumn < 'A' || present.iColumn > 'H')
  {
    createNextMessage("Invalid column.\n");
    return;
  }

  if (present.iRow < '0' || present.iRow > '8')
  {
    createNextMessage("Invalid row.\n");
    return;
  }

  // Put in the string to be logged
  to_record += present.iColumn;
  to_record += present.iRow;
  to_record += "-";

  // Convert column from ['A'-'H'] to [0x00-0x07]
  present.iColumn = present.iColumn - 'A';

  // Convert row from ['1'-'8'] to [0x00-0x07]
  present.iRow = present.iRow - '1';*/

  //char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);
  Figure* figure = m_figuresMoveLogic->getFigureAtPosition(present.iRow, present.iColumn);
  //new Code

  //cout << "Piece is " << char(chPiece) << "\n";

  if (Constants::EMPTY_SQUARE == figure)
  {
    //createNextMessage("You picked an EMPTY square.\n");
    return;
  }

  if (static_cast<int>(Player::WHITE_PLAYER) == m_figuresMoveLogic->getCurrentTurn())
  {
    if (false == figure->isWhite())
    {
      //createNextMessage("It is WHITE's turn and you picked a BLACK piece\n");
      setBackFigureToPrevPos(figure, move_from);
      return;
    }
  }
  else
  {
    if (false != figure->isWhite())
    {
      //createNextMessage("It is BLACK's turn and you picked a WHITE piece\n");
      setBackFigureToPrevPos(figure, move_from);
      return;
    }
  }

  // ---------------------------------------------------
  // Get user input for the square to move to
  // ---------------------------------------------------
  /*cout << "Move to: ";
  std::string move_to;
  getline(cin, move_to);

  if (move_to.length() > 2)
  {
    createNextMessage("You should type only two characters (column and row)\n");
    return;
  }*/

  // ---------------------------------------------------
  // Did the user pick a valid house to move?
  // Must check if:
  // - It's inside the board (A1-H8)
  // - The move is valid
  // ---------------------------------------------------
  Position future;
  future.iColumn = move_to.height;
  future.iRow = move_to.width;

  std::string futureStr = m_figuresMoveLogic->parseMoveCellIntToString(future);

  //Put in the string to be logged
  to_record += futureStr;

  //future.iColumn = toupper(future.iColumn);

  /*if (future.iColumn < 'A' || future.iColumn > 'H')
  {
    createNextMessage("Invalid column.\n");
    return;
  }

  if (future.iRow < '0' || future.iRow > '8')
  {
    createNextMessage("Invalid row.\n");
    return;
  }*/

  // Put in the string to be logged
  /*to_record += future.iColumn;
  to_record += future.iRow;

  // Convert columns from ['A'-'H'] to [0x00-0x07]
  future.iColumn = future.iColumn - 'A';

  // Convert row from ['1'-'8'] to [0x00-0x07]
  future.iRow = future.iRow - '1';*/

  // Check if it is not the exact same square
  if (future.iRow == present.iRow && future.iColumn == present.iColumn)
  {
    //createNextMessage("[Invalid] You picked the same square!\n");
    return;
  }

  // Is that move allowed?
  EnPassant  S_enPassant = { 0 };
  Castling   S_castling = { 0 };
  Promotion  S_promotion = { 0 };

  if (false == m_figuresMoveLogic->isMoveValid(figure, present, future, &S_enPassant, &S_castling, &S_promotion))
  {
    //createNextMessage("[Invalid] Piece can not move to that square!\n");
    //setBackFigureToPrevPos(figure, move_from);
    return;
  }

  // ---------------------------------------------------
  // Promotion: user most choose a piece to
  // replace the pawn
  // ---------------------------------------------------
  if (S_promotion.bApplied == true)
  {
    bool isWhite = (static_cast<int>(Player::WHITE_PLAYER) == m_figuresMoveLogic->getCurrentTurn());

    // Set callBack
    //auto lfHidePromotion = [this, to_record, present, future, &S_promotion](int typeFigure)
    auto lfHidePromotion = [this, present, future, &S_promotion](int typeFigure)
    {
      Size sPresent(present.iRow, present.iColumn);
      Size sFuture(future.iRow, future.iColumn);
      this->movePromotion(sPresent, sFuture, S_promotion, typeFigure);
    };

    m_promotionLayer->callBackHide(lfHidePromotion);

    m_promotionLayer->show(isWhite);

    return;
    //movePromotion(to_record, Size(present.iRow, present.iColumn), S_promotion, m_lastPromotedFigure);

    /*cout << "Promote to (Q, R, N, B): ";
    std::string piece;
    getline(cin, piece);

    if (piece.length() > 1)
    {
      createNextMessage("You should type only one character (Q, R, N or B)\n");
      return;
    }

    char chPromoted = toupper(piece[0]);

    if (chPromoted != 'Q' && chPromoted != 'R' && chPromoted != 'N' && chPromoted != 'B')
    {
      createNextMessage("Invalid character.\n");
      return;
    }

    S_promotion.chBefore = current_game->getPieceAtPosition(present.iRow, present.iColumn);

    if (Chess::WHITE_PLAYER == current_game->getCurrentTurn())
    {
      S_promotion.chAfter = toupper(chPromoted);
    }
    else
    {
      S_promotion.chAfter = tolower(chPromoted);
    }

    to_record += '=';
    to_record += toupper(chPromoted); // always log with a capital letter
    */
  }

  // ---------------------------------------------------
  // Log the move: do it prior to making the move
  // because we need the getCurrentTurn()
  // ---------------------------------------------------
  m_figuresMoveLogic->logMove(to_record);

  // ---------------------------------------------------
  // Make the move
  // ---------------------------------------------------

  makeTheMove(Size(present.iRow, present.iColumn), Size(future.iRow, future.iColumn), &S_enPassant, &S_castling, &S_promotion);

  // ---------------------------------------------------------------
  // Check if this move we just did put the oponent's king in check
  // Keep in mind that player turn has already changed
  // ---------------------------------------------------------------
  if (true == m_figuresMoveLogic->playerKingInCheck())
  {
    if (true == m_figuresMoveLogic->isCheckMate())
    {
      if (static_cast<int>(Player::WHITE_PLAYER) == m_figuresMoveLogic->getCurrentTurn())
      {
        //appendToNextMessage("Checkmate! Black wins the game!\n");
      }
      else
      {
        //appendToNextMessage("Checkmate! White wins the game!\n");
      }
    }
    else
    {
      // Add to the string with '+=' because it's possible that
      // there is already one message (e.g., piece captured)
      if (static_cast<int>(Player::WHITE_PLAYER) == m_figuresMoveLogic->getCurrentTurn())
      {
        //appendToNextMessage("White king is in check!\n");
      }
      else
      {
        //appendToNextMessage("Black king is in check!\n");
      }
    }
  }

  return;
}

void GameLayer::makeTheMove(const Size& present, const Size& future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion)
{
  //char chPiece = current_game->getPieceAtPosition(present.iRow, present.iColumn);
  Figure* figure = m_figuresMoveLogic->getFigureAtPosition(present.width, present.height);

  // -----------------------
  // Captured a piece?
  // -----------------------
  if (m_figuresMoveLogic->isSquareOccupied(future.width, future.height))
  {
    //char chAuxPiece = current_game->getPieceAtPosition(future.iRow, future.iColumn);
    Figure* auxFigures = m_figuresMoveLogic->getFigureAtPosition(future.width, future.height);

    //if (Chess::getPieceColor(chPiece) != Chess::getPieceColor(chAuxPiece))
    if(figure && figure->isWhite() != auxFigures->isWhite())
    {
      //createNextMessage(Chess::describePiece(chAuxPiece) + " captured!\n");
    }
    else
    {
      //std::cout << "Error. We should not be making this move\n";
      throw("Error. We should not be making this move");
    }
  }
  else if (true == S_enPassant->bApplied)
  {
    bool stop = true;
    //createNextMessage("Pawn captured by \"en passant\" move!\n");
  }

  if ((true == S_castling->bApplied))
  {
    bool stop = true;
    //createNextMessage("Castling applied!\n");
  }

  //current_game->movePiece(present, future, S_enPassant, S_castling, S_promotion);
  Position ppresent;
  ppresent.iRow = present.width;
  ppresent.iColumn = present.height;

  Position pfuture;
  pfuture.iRow = future.width;
  pfuture.iColumn = future.height;

  m_figuresMoveLogic->moveFigure(ppresent, pfuture, S_enPassant, S_castling, S_promotion);
}

DataChess& GameLayer::getDataChess()
{
  return m_dataChess;
}

Board* GameLayer::getBoard()
{
  return m_board;
}

void GameLayer::updateBoard(int typeFigure, const Size& prevPos, const Size& newPos)
{
  /*m_dataChess.figures[newPos.width][newPos.height] = figure;
  m_dataChess.figures[prevPos.width][prevPos.height] = 0;*/

  m_dataChess.board[newPos.width][newPos.height] = typeFigure;
  m_dataChess.board[prevPos.width][prevPos.height] = 0;
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

  bool isMoveValid = m_figuresMoveLogic->isMoveValid(figure, present, future, &S_enPassant, &S_castling, &S_promotion);

  return isMoveValid;
}

void GameLayer::undoMove(void)
{
  if (false == m_figuresMoveLogic->undoIsPossible())
  {
    //createNextMessage("Undo is not possible now!\n");
    return;
  }

  m_figuresMoveLogic->undoLastMove();
  //createNextMessage("Last move was undone\n");
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


