#include "GameLayer.h"
#include "Board.h"
#include "Grid.h"
#include "Piece.h"
#include "PieceMoveLogic.h"
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

  // Copy piece_board
  std::copy(&Constants::INITIAL_PIECE_BOARD[0][0], &Constants::INITIAL_PIECE_BOARD[0][0] + Constants::ROWS*Constants::COLUMNS, &m_piece_board[0][0]);

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
      m_dataChess.board[i][j] = Constants::INITIAL_PIECE_BOARD[i][j];
    }
  }

  // Create Board
  Board* board = createBoard(Constants::CELL_SIZE, Constants::ROWS, Constants::COLUMNS);
  this->addChild(board, static_cast<int>(ZOrderGame::BOARD));
  m_board = board;
  float deltaBoardX = (Constants::CELL_SIZE * Constants::COLUMNS / 2);
  float deltaBoardY = (Constants::CELL_SIZE * Constants::ROWS / 2);
  board->setPosition(Vec2((visibleSize.width / 2 + origin.x) - deltaBoardX, (visibleSize.height / 2 + origin.y) - deltaBoardY));

  // Create Piece
  m_pieces = createPieces(Constants::INITIAL_PIECE_BOARD, Constants::ROWS, Constants::COLUMNS);
  m_dataChess.pieces = m_pieces;
  // Load piece
  board->loadAllPieces(m_pieces, static_cast<int>(ZOrderGame::PIECE));

  // Create Logic(PieceMoveLogic)
  PieceMoveLogic* pPieceMoveLogic = createPieceMoveLogic(this);
  m_pieceMoveLogic = pPieceMoveLogic;
  this->addChild(pPieceMoveLogic, 1);
  pPieceMoveLogic->loadBoard(Constants::INITIAL_PIECE_BOARD);

  pPieceMoveLogic->callBackAddPiece([this](int type, bool isWhite, const Size& futureCell) { this->m_board->addPieceN(type, isWhite, futureCell, static_cast<int>(ZOrderGame::PIECE));});
  pPieceMoveLogic->callBackDeletePiece ([this](const Size& presentCell) { this->m_board->removePieceN(presentCell);});
  pPieceMoveLogic->callBackMovePiece([this](const Size& presentCell, const Size& futureCell) {this->m_board->movePieceFromToN(presentCell, futureCell);});
  pPieceMoveLogic->callBackUpdatePieceCell([this](const Size& presentCell, const Size& futureCell) {this->m_board->updatePieceCellN(presentCell, futureCell); });

  // Create Promotion Layer
  PromotionLayer* pPromotionLayer = createPromotionLayer();
  m_promotionLayer = pPromotionLayer;
  this->addChild(pPromotionLayer, static_cast<int>(ZOrderGame::PROMOTION));
  pPromotionLayer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

  // Set callBack
  auto lfClickPiece = [this](int typePiece)
  {
    this->applyPromotion(typePiece);
  };

  pPromotionLayer->callBackClickPiece(lfClickPiece);

  // Create TouchAndDragLayer
  TouchAndDragLayer* touchAndDragLayer = createTouchAndDragLayer(board);
  this->addChild(touchAndDragLayer, static_cast<int>(ZOrderGame::TOUCH_AND_DRAG));
  m_touchAndDragLayer = touchAndDragLayer;
  touchAndDragLayer->setPosition(board->getPosition());

  // Set callBacks to touchAndDragLayer;
  auto lfUpdatePieceBoard = [this](Piece* piece, Size& prevPos, Size& newPos)->void
  {
    //movePiece(prevPos, newPos);
    bool isMoveValid = this->checkPieceMove(piece, prevPos, newPos);

    if (isMoveValid)
    {
      movePiece(prevPos, newPos);
      //piece->setCell(newPos);
      //updateBoardChess(piece, prevPos, newPos);
    }
    else
    {
      setBackPieceToPrevPos(piece, prevPos);
    } 
  };
  touchAndDragLayer->callBackUpdateBoardPiece(lfUpdatePieceBoard);

  auto lfGetPieceFromCell = [this](Size& cellIJ)->Piece*
  {
    Piece* pieceIJ = m_board->getPieceFromCell(cellIJ.width, cellIJ.height);
    return pieceIJ;
  };
  touchAndDragLayer->callBackGetPieceFromCell(lfGetPieceFromCell);

  // Create HudLayer
  HudLayer* pHudLayer = createHudLayer();
  this->addChild(pHudLayer, static_cast<int>(ZOrderGame::HUD));
  m_hudLayer = pHudLayer;
  auto lfUndoMove = [this]() {
    this->undoMove();
  };
  pHudLayer->callBackUndoLastMove(lfUndoMove);

  // Create Test Piece
  //createTestPiece();

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

  /*Piece* testPiece = createPieceFileName(1, true);
  m_board->addPiece(testPiece, Size(4,5), 1001);

  Piece* clonePiece = testPiece->clonePiece();
  m_board->addPiece(clonePiece, Size(4, 6), 1001);

  m_board->removePiece(testPiece);

  clonePiece->setPosition(m_grid->getPointByCell(3, 3));*/

  return true;
}

void GameLayer::update(float delta) {
  
}

Board* GameLayer::createBoard(float cellSize, int rows, int columns)
{
  Board* pBoard = new(std::nothrow) Board(cellSize, rows, columns);
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

TouchAndDragLayer* GameLayer::createTouchAndDragLayer(Grid* grid)
{
  TouchAndDragLayer* pTouchAndDrag = new(std::nothrow) TouchAndDragLayer(grid);
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

PieceMoveLogic* GameLayer::createPieceMoveLogic(GameLayer* gameLayer)
{
  PieceMoveLogic* pPieceMoveLogic = new(std::nothrow) PieceMoveLogic(gameLayer);
  if (pPieceMoveLogic && pPieceMoveLogic->init())
  {
    pPieceMoveLogic->autorelease();
    return pPieceMoveLogic;
  }
  else
  {
    delete pPieceMoveLogic;
    pPieceMoveLogic = nullptr;
    return nullptr;
  }
}

void GameLayer::setBackPieceToPrevPos(Piece* piece, const Size& prevPos)
{
  /*Piece* boardPiece = m_board->getPieceFromCell(prevPos.width, prevPos.height);
  if(boardPiece)
    boardPiece->setCell(prevPos);*/

  Vec2 prevPosPiece = m_board->getPointByCell(int(prevPos.height), int(prevPos.width));
  if (piece)
  {
    piece->setPosition(prevPosPiece);
    piece->setCell(prevPos);
  }  
}

int GameLayer::applyPromotion(int typePiece)
{
  m_promotionLayer->hide();
  m_lastPromotedPiece = typePiece;

  return typePiece;
}

void GameLayer::movePromotion(Size& present, Size& future, Promotion& promotion, int typePromotionPiece)
{
  std::string to_record;

  Position ppresent;
  ppresent.iRow = present.width;
  ppresent.iColumn = present.height;
  
  Position pfuture;
  pfuture.iRow = future.width;
  pfuture.iColumn = future.height;

  std::string presentStr = m_pieceMoveLogic->parseMoveCellIntToString(ppresent);
  std::string futureStr = m_pieceMoveLogic->parseMoveCellIntToString(pfuture);

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

  //Piece* piece = m_pieceMoveLogic->getPieceAtPosition(present.width, present.height);
  int iPiece = m_pieceMoveLogic->getPieceAtPositionI(present.width, present.height);
  //promotion.pieceBefore = piece;
  //promotion.typeBefore = iPiece;

  //Piece* promotedPiece{ nullptr };

  int kColor{ 1 };
  if(static_cast<int>(Player::WHITE_PLAYER) == m_pieceMoveLogic->getCurrentTurn())
  {
    // to delete
    //promotion.pieceAfter = promotedPiece;
    kColor = 1;
    promotion.typeAfter = kColor * typePromotionPiece;
    // to delete
    promotion.isWhite = true;
  }
  else
  {
    // to delete
    //promotion.pieceAfter = promotedPiece;
    kColor = -1;
    promotion.typeAfter = kColor * typePromotionPiece;
    // to delete
    promotion.isWhite = false;
  }

  // ---------------------------------------------------
  // Log the move: do it prior to making the move
  // because we need the getCurrentTurn()
  // ---------------------------------------------------
  m_pieceMoveLogic->logMove(to_record);

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
  if (true == m_pieceMoveLogic->playerKingInCheck())
  {
    if (true == m_pieceMoveLogic->isCheckMate())
    {
      if (static_cast<int>(Player::WHITE_PLAYER) == m_pieceMoveLogic->getCurrentTurn())
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
      if (static_cast<int>(Player::WHITE_PLAYER) == m_pieceMoveLogic->getCurrentTurn())
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

void GameLayer::movePieceToPos(Piece* piece, const Size& futureCell)
{
  //Vec2 prevPosPiece = m_grid->getPointByCell(int(futureCell.height), int(futureCell.width));
  Vec2 prevPosPiece = m_board->getPointByCell(int(futureCell.height), int(futureCell.width));
  piece->setPosition(prevPosPiece);
}

void GameLayer::setPieceToNewPos(Piece* piece, const cocos2d::Size& newPos)
{
  m_dataChess.pieces[newPos.width][newPos.height] = piece;
  piece->setCell(Size(newPos.width,newPos.height));
}

void GameLayer::movePiece(const Size& move_from, const Size& move_to)
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

  std::string presentStr = m_pieceMoveLogic->parseMoveCellIntToString(present);

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
  Piece* piece = m_pieceMoveLogic->getPieceAtPosition(present.iRow, present.iColumn);
  int iPiece = m_pieceMoveLogic->getPieceAtPositionI(present.iRow, present.iColumn);
  //new Code

  //cout << "Piece is " << char(chPiece) << "\n";

  if (Constants::EMPTY_SQUAREI == iPiece)
  {
    //createNextMessage("You picked an EMPTY square.\n");
    return;
  }

  if (static_cast<int>(Player::WHITE_PLAYER) == m_pieceMoveLogic->getCurrentTurn())
  {
    if (Piece::isBlack(iPiece))
    {
      //createNextMessage("It is WHITE's turn and you picked a BLACK piece\n");
      setBackPieceToPrevPos(piece, move_from);
      return;
    }
  }
  else
  {
    if (Piece::isWhite(iPiece))
    {
      //createNextMessage("It is BLACK's turn and you picked a WHITE piece\n");
      setBackPieceToPrevPos(piece, move_from);
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

  std::string futureStr = m_pieceMoveLogic->parseMoveCellIntToString(future);

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

  if (false == m_pieceMoveLogic->isMoveValid(piece, present, future, &S_enPassant, &S_castling, &S_promotion))
  {
    //createNextMessage("[Invalid] Piece can not move to that square!\n");
    //setBackPieceToPrevPos(Piece, move_from);
    return;
  }

  // ---------------------------------------------------
  // Promotion: user most choose a piece to
  // replace the pawn
  // ---------------------------------------------------
  if (S_promotion.bApplied == true)
  {
    bool isWhite = (static_cast<int>(Player::WHITE_PLAYER) == m_pieceMoveLogic->getCurrentTurn());

    // Set callBack
    //auto lfHidePromotion = [this, to_record, present, future, &S_promotion](int typePiece)
    auto lfHidePromotion = [this, present, future, &S_promotion](int typePiece)
    {
      Size sPresent(present.iRow, present.iColumn);
      Size sFuture(future.iRow, future.iColumn);

      m_board->removePieceN(sFuture);
      m_board->addPieceN(typePiece, m_pieceMoveLogic->getOppositCurrentTurn(), sFuture, static_cast<int>(ZOrderGame::PIECE));

      this->movePromotion(sPresent, sFuture, S_promotion, typePiece);
    };

    m_promotionLayer->callBackHide(lfHidePromotion);

    m_promotionLayer->show(isWhite);

    return;
    //movePromotion(to_record, Size(present.iRow, present.iColumn), S_promotion, m_lastPromotedPiece);

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
  m_pieceMoveLogic->logMove(to_record);

  // ---------------------------------------------------
  // Make the move
  // ---------------------------------------------------

  makeTheMove(Size(present.iRow, present.iColumn), Size(future.iRow, future.iColumn), &S_enPassant, &S_castling, &S_promotion);

  // ---------------------------------------------------------------
  // Check if this move we just did put the oponent's king in check
  // Keep in mind that player turn has already changed
  // ---------------------------------------------------------------
  if (true == m_pieceMoveLogic->playerKingInCheck())
  {
    if (true == m_pieceMoveLogic->isCheckMate())
    {
      if (static_cast<int>(Player::WHITE_PLAYER) == m_pieceMoveLogic->getCurrentTurn())
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
      if (static_cast<int>(Player::WHITE_PLAYER) == m_pieceMoveLogic->getCurrentTurn())
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
  Piece* piece = m_pieceMoveLogic->getPieceAtPosition(present.width, present.height);
  int iPiece = m_pieceMoveLogic->getPieceAtPositionI(present.width, present.height);

  // -----------------------
  // Captured a piece?
  // -----------------------
  if (m_pieceMoveLogic->isSquareOccupied(future.width, future.height))
  {
    //char chAuxPiece = current_game->getPieceAtPosition(future.iRow, future.iColumn);
    int iAuxPiece = m_pieceMoveLogic->getPieceAtPositionI(future.width, future.height);

    //Piece* auxPiece = m_pieceMoveLogic->getPieceAtPosition(future.width, future.height);

    //if (Chess::getPieceColor(chPiece) != Chess::getPieceColor(chAuxPiece))
    //if(piece && piece->isWhite() != auxPiece->isWhite())
    if(Piece::getColor(iPiece) != Piece::getColor(iAuxPiece))
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
    //createNextMessage("Pawn captured by \"en passant\" move!\n");
  }

  if ((true == S_castling->bApplied))
  {
    //createNextMessage("Castling applied!\n");
  }

  //current_game->movePiece(present, future, S_enPassant, S_castling, S_promotion);
  Position ppresent;
  ppresent.iRow = present.width;
  ppresent.iColumn = present.height;

  Position pfuture;
  pfuture.iRow = future.width;
  pfuture.iColumn = future.height;

  m_pieceMoveLogic->movePiece(ppresent, pfuture, S_enPassant, S_castling, S_promotion);
}

DataChess& GameLayer::getDataChess()
{
  return m_dataChess;
}

Board* GameLayer::getBoard()
{
  return m_board;
}

void GameLayer::updateBoard(int typePiece, const Size& presentCell, const Size& futureCell)
{
  m_dataChess.board[futureCell.width][futureCell.height] = typePiece;
  m_dataChess.board[presentCell.width][futureCell.height] = 0;
}

void GameLayer::removePieceBoard(const Size& presentCell)
{
  m_dataChess.pieces[presentCell.width][presentCell.height] = 0;
}

void GameLayer::movePieceBoard(int typePiece, const cocos2d::Size & presentCell, const cocos2d::Size & futureCell)
{
  m_dataChess.board[futureCell.width][futureCell.height] = typePiece;
  m_dataChess.board[presentCell.width][futureCell.height] = 0;
}

void GameLayer::addPieceBoard(int typePiece, cocos2d::Size & futureCell)
{
  m_dataChess.board[futureCell.width][futureCell.height] = typePiece;
}

TouchAndDragLayer* GameLayer::getTouchAndDragLayer()
{
  return m_touchAndDragLayer;
}

const std::vector<std::vector<Piece*>>& GameLayer::getPiece()
{
  return m_pieces;
}

Piece* GameLayer::createPieceFileName(int type, bool isWhite)
{
  std::string fileName(Constants::WHITE_PAWN_PNG);
  TypePiece typePiece{TypePiece::PAWN};

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
      fileName =(isWhite) ? Constants::WHITE_KING_PNG : Constants::BLACK_KING_PNG;
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

std::vector<std::vector<Piece*>> GameLayer::createPieces(const int piece_board[8][8], int rows, int columns)
{
  std::vector<std::vector<Piece*>> pieces;
  Piece* pPiece{ nullptr };

  for (int i = 0; i < rows; i++)
  {
    std::vector<Piece*> row;
    for (int j = 0; j < columns; j++)
    {
      int piece = piece_board[i][j];

      int type{ abs(piece) };
      bool isWhite = (piece < 0) ? false : true;

      if (piece > 0 || piece < 0)
      {
        pPiece = createPieceFileName(type, isWhite);
        row.push_back(pPiece);
      }
      else
      {
        row.push_back(0);
      }
    }
    pieces.push_back(row);
  }
  return pieces;
}

void GameLayer::createTestPiece()
{
  Grid* grid = m_grid;

  // Create piece
 /*Piece* piece1 = Piece::createPiece(TypePiece::ROOK, Constants::BLACK_ROOK_PNG);
 grid->addChild(piece1, static_cast<int>(ZOrderGame::Piece));
 Piece1->setPosition(grid->getPointByCell(0, 0));
 });*/

 // Create piece
  //Piece* piece2 = Piece::createPiece(TypePiece::HORSE, ColourPiece::WHITE, Constants::BLACK_HORSE_PNG);
  Piece* piece2 = Piece::createPiece(2, true, Constants::BLACK_KING_PNG);
  grid->addChild(piece2, static_cast<int>(ZOrderGame::PIECE));
  piece2->setPosition(grid->getPointByCell(1, 0));
  piece2->setTouchEnabled(true);
  /*Piece2->addClickEventListener([=](Ref*) {
    //Piece2->setRotation(45);
    //m_currentPiece = Piece2;
    if (m_grid->getCurrentPiece() == nullptr) {
      m_grid->setCurrentPiece(Piece2);
    }
  });*/

  // Create Piece
  /*Piece* Piece3 = Piece::createPiece(TypePiece::OFFICER, Constants::BLACK_OFFICER_PNG);
  grid->addChild(Piece3, static_cast<int>(ZOrderGame::Piece));
  Piece3->setPosition(grid->getPointByCell(2, 0));

  // Create Piece
  Piece* Piece4 = Piece::createPiece(TypePiece::QUEEN, Constants::BLACK_QUEEN_PNG);
  grid->addChild(Piece4, static_cast<int>(ZOrderGame::Piece));
  Piece4->setPosition(grid->getPointByCell(3, 0));

  // Create Piece
  Piece* Piece5 = Piece::createPiece(TypePiece::KING, Constants::BLACK_KING_PNG);
  grid->addChild(Piece5, static_cast<int>(ZOrderGame::Piece));
  Piece5->setPosition(grid->getPointByCell(4, 0));

  // Create Piece
  Piece* Piece6 = Piece::createPiece(TypePiece::PAWN, Constants::BLACK_PAWN_PNG);
  grid->addChild(Piece6, static_cast<int>(ZOrderGame::Piece));
  Piece6->setPosition(grid->getPointByCell(4, 1));
  Piece6->activateClickEvent();*/

  /*DrawNode *drawnode = DrawNode::create();
  drawnode->drawCircle(Vec2(0, 0), 20, 360, 20, true, 1, 1, Color4F::MAGENTA);
  Piece1->addChild(drawnode);*/

}

bool GameLayer::checkPieceMove(Piece* piece, const cocos2d::Size& prevCellIJ, const cocos2d::Size& curCellIJ)
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

  bool isMoveValid = m_pieceMoveLogic->isMoveValid(piece, present, future, &S_enPassant, &S_castling, &S_promotion);

  return isMoveValid;
}

void GameLayer::undoMove(void)
{
  if (false == m_pieceMoveLogic->undoIsPossible())
  {
    //createNextMessage("Undo is not possible now!\n");
    return;
  }

  m_pieceMoveLogic->undoLastMove();
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
  //m_currentPiece = nullptr;
  m_delta.x = 0;
  m_delta.y = 0;
}

void GameLayer::onMouseMove(Event* event)
{
  //m_delta = e->getDelta();

  /*if (m_currentPiece)
  {
    EventMouse* e = (EventMouse*)event;
    Vec2 curFig(m_currentPiece->getPosition());
    m_currentPiece->setPosition(m_currentPiece->getPosition() + e->getDelta());
  }
}*/


