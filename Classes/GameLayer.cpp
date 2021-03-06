#include "GameLayer.h"
#include "Board.h"
#include "Grid.h"
#include "Piece.h"
#include "PieceMove.h"
#include "PieceMoveLogic.h"
#include "AILogic.h"
#include "TouchAndDragLayer.h"
#include "PromotionLayer.h"
#include "PromptLayer.h"
#include "HudLayer.h"
#include "LogicData.h"
#include "Constants.h"
#include "Globals.h"

//#include "ConnectorC.hpp"
#include "Connector.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <cmath>
#include <string>
#include <stdlib.h>
#include <vector>
#include <ctime>

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
  if (!Layer::init()) {

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

  // Create Board
  Board* board = createBoard(Constants::CELL_SIZE, Constants::ROWS, Constants::COLUMNS);
  this->addChild(board, static_cast<int>(ZOrderGame::BOARD));
  m_board = board;
  float deltaBoardX = (Constants::CELL_SIZE * Constants::COLUMNS / 2);
  float deltaBoardY = (Constants::CELL_SIZE * Constants::ROWS / 2);
  board->setPosition(Vec2((visibleSize.width / 2 + origin.x) - deltaBoardX, (visibleSize.height / 2 + origin.y) - deltaBoardY));

  // Create Piece
  m_pieces = createPieces(Constants::INITIAL_PIECE_BOARD, Constants::ROWS, Constants::COLUMNS);

  // Load piece
  board->loadAllPieces(m_pieces, static_cast<int>(ZOrderGame::PIECE));

  // Create Logic(PieceMoveLogic)
  PieceMoveLogic* pPieceMoveLogic = createPieceMoveLogic(this);
  m_pieceMoveLogic = pPieceMoveLogic;
  this->addChild(pPieceMoveLogic, 1);
  pPieceMoveLogic->loadBoard(Constants::INITIAL_PIECE_BOARD);

  // CallBacks for Logic
  pPieceMoveLogic->callBackAddPiece([this](int type, bool isWhite, const Size& futureCell) { this->m_board->addPieceN(type, isWhite, futureCell, static_cast<int>(ZOrderGame::PIECE)); });
  pPieceMoveLogic->callBackDeletePiece([this](const Size& presentCell) { this->m_board->removePieceN(presentCell); });
  pPieceMoveLogic->callBackMovePiece([this](const Size& presentCell, const Size& futureCell) { this->m_board->movePieceFromToN(presentCell, futureCell); });
  pPieceMoveLogic->callBackUndoLastMove([this](const Size& presentCell, const Size& futureCell)
  {
    m_promptLayer->hideRectPrompts();
    m_promptLayer->showRectPrompts();
    m_promptLayer->setPositionRects(presentCell, futureCell);
  });

  // Create connector
  // AI
  /*Connector* connector = Connector::createConnector();
  this->addChild(connector, 1);
  m_connector = connector;
  m_connector->ConnectToEngine("D:\\BIBAGAMES\\C++PROJECTS\\COCOS\\PROJECTS\\Chess\\proj.win32\\stockfish.exe");*/


  // Creat PromptPieceLayer
  PromptLayer* promptLayer = createPromptPieceLayer(Constants::CELL_SIZE, Constants::ROWS, Constants::COLUMNS);
  promptLayer->setPosition(board->getPosition());
  m_promptLayer = promptLayer;
  this->addChild(promptLayer, static_cast<int>(ZOrderGame::PROMT));


  // Create Promotion Layer
  PromotionLayer* pPromotionLayer = createPromotionLayer();
  m_promotionLayer = pPromotionLayer;
  this->addChild(pPromotionLayer, static_cast<int>(ZOrderGame::PROMOTION));
  pPromotionLayer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

  // Set callBack
  auto lfClickPiece = [this](int typePiece)
  {
    this->applyPromotion();
  };

  pPromotionLayer->callBackClickPiece(lfClickPiece);


  // Create AILogic
  AILogic* AILogic = createAILogic(Player::BLACK_PLAYER);
  this->addChild(AILogic, 1);
  m_AILogic = AILogic;

  auto lfGetPieceTypeColor = [this](int i, int j)->int
  {
    int piece = m_pieceMoveLogic->getPieceAtPositionI(i, j);

    return piece;
  };

  AILogic->callBackGetPiece(lfGetPieceTypeColor);

  auto lfGetValideMovesAI = [this](int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& possibleMoves)
  {
    if (m_promptLayer)
    {
      // Show circle prompts
      auto lfCallBackIsMoveValide = [this](const Size& prevPos, const Size& newPos)->bool
      {
        PieceMove pieceMove;

        bool isMoveValide = this->checkPieceMove(prevPos, newPos, pieceMove);

        return isMoveValide;
      };

      m_promptLayer->callBackIsMoveValide(lfCallBackIsMoveValide);

      m_promptLayer->getValideMoves(typePiece, presentCell, possibleMoves);
    }
  };

  AILogic->getValideMovesCallBack(lfGetValideMovesAI);

  auto lfGetTypeMoveCallBack = [this](const cocos2d::Size& moveFrom, const cocos2d::Size& moveTo, Player playerTurn, PieceMove& pieceMove)
  {
    this->getTypePieceMove(moveFrom, moveTo, playerTurn, pieceMove);
  };

  AILogic->getTypeMoveCallBack(lfGetTypeMoveCallBack);

  auto lfForwardOneMoveLogic = [this](PieceMove& pieceMove)
  {
    this->forwardOneMoveLogic(pieceMove);
  };

  AILogic->forwardOneMoveLogicCallBack(lfForwardOneMoveLogic);

  auto lfUndoOneMoveLogic = [this](PieceMove& pieceMove)
  {
    this->undoOneMoveLogic(pieceMove);
  };

  AILogic->undoOneMoveLogicCallBack(lfUndoOneMoveLogic);

  // Create TouchAndDragLayer
  TouchAndDragLayer* touchAndDragLayer = createTouchAndDragLayer(Constants::CELL_SIZE, Constants::ROWS, Constants::COLUMNS);
  this->addChild(touchAndDragLayer, static_cast<int>(ZOrderGame::TOUCH_AND_DRAG));
  m_touchAndDragLayer = touchAndDragLayer;
  touchAndDragLayer->setPosition(board->getPosition());

  // Set callBacks to touchAndDragLayer;
  auto lfUpdatePieceBoard = [this](Piece* piece, Size& prevPos, Size& newPos)->void
  {
    PieceMove pieceMove;

    bool isMoveValid = this->checkPieceMove(prevPos, newPos, pieceMove);

    m_promptLayer->hideRectPrompts();
    m_promptLayer->hideCirclePrompts();

    if (isMoveValid)
    {
      movePiece(prevPos, newPos);

      piece->setCell(newPos);

      m_promptLayer->showRectPrompts();
      m_promptLayer->setPositionRects(prevPos, newPos);


      // AI(myCode)
      if (Globals::onePlayer == true && Player::WHITE_PLAYER != static_cast<Player>(m_pieceMoveLogic->getCurrentTurn()))
      {
        PieceMove bestPieceMove;

        m_AILogic->getBestMove(bestPieceMove);

        MoveData bestMoveData;
        bestPieceMove.getMoveData(bestMoveData);

        Position moveFrom = bestMoveData.from;
        Position moveTo = bestMoveData.to;

        movePiece(Size(moveFrom.iRow, moveFrom.iColumn), Size(moveTo.iRow, moveTo.iColumn));
        //m_board->movePieceFromToN(Size(moveFrom.iRow, moveFrom.iColumn), Size(moveTo.iRow, moveTo.iColumn));
      }

      // AI(computer stockfish)
      /*if (Globals::onePlayer == true && Player::WHITE_PLAYER != static_cast<Player>(m_pieceMoveLogic->getCurrentTurn()))
      {
        std::string toComputerMove;

        //m_isFirstMove = false;

        if (m_isFirstMove)
        {
          toComputerMove = m_pieceMoveLogic->getLastMove();
        }
        else
        {
          std::string lastMove = m_pieceMoveLogic->getLastMove();
          std::string penultMove = m_pieceMoveLogic->getPenultMove();
          toComputerMove = lastMove + " " + penultMove;
        }

        std::string computerMove = m_connector->getNextMove(toComputerMove);

        // Parse the line
        Position from;
        Position to;

        m_pieceMoveLogic->parseMoveStringToCell(computerMove, &from, &to);

        bool isMoveValideComp = this->checkPieceMove(Size(from.iRow, from.iColumn), Size(to.iRow, to.iColumn));

        if (isMoveValideComp)
        {
          movePiece(Size(from.iRow, from.iColumn), Size(to.iRow, to.iColumn));
          m_board->movePieceFromToN(Size(from.iRow, from.iColumn), Size(to.iRow, to.iColumn));
        }
      }*/
    }
    else
    {
      if (Constants::EMPTY_SQUAREI == m_pieceMoveLogic->getPieceAtPositionI(newPos.width, newPos.height))
      {
        piece->setCell(newPos);

        // Move back Piece
        m_board->movePieceFromToN(newPos, prevPos);
      }
      else
      {
        // Move back Piece
        m_board->movePieceFromToN(prevPos, prevPos);
      }

      // Show rect prompts
      m_promptLayer->showRectPrompts(false);
      m_promptLayer->setPositionRects(prevPos, newPos);

      // Show circle prompts
      auto lfCallBackIsMoveValide = [this](const Size& prevPos, const Size& newPos)->bool
      {
        PieceMove pieceMove;

        bool isMoveValide = this->checkPieceMove(prevPos, newPos, pieceMove);

        return isMoveValide;
      };

      m_promptLayer->callBackIsMoveValide(lfCallBackIsMoveValide);

      int typePiece = static_cast<int>(piece->getType());

      std::vector<cocos2d::Size> valideMovesPiece;

      m_promptLayer->getValideMoves(typePiece, prevPos, valideMovesPiece);

      m_promptLayer->setPositionCircles(valideMovesPiece);
      m_promptLayer->showCirclePrompts(valideMovesPiece);
    }
  };

  // Touch And Drag layer callback Udate Piece Board
  touchAndDragLayer->callBackHaveMovedPiece(lfUpdatePieceBoard);

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

  /*m_mouseListener = EventListenerMouse::create();
  m_mouseListener->onMouseMove = CC_CALLBACK_1(GameLayer::onMouseMove, this);
  m_mouseListener->onMouseUp = CC_CALLBACK_1(GameLayer::onMouseUp, this);
  m_mouseListener->onMouseDown = CC_CALLBACK_1(GameLayer::onMouseDown, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(m_mouseListener, this);*/
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

TouchAndDragLayer* GameLayer::createTouchAndDragLayer(float cellSize, int rows, int columns)
{
  TouchAndDragLayer* pTouchAndDrag = new(std::nothrow) TouchAndDragLayer(cellSize, rows, columns);
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

PromptLayer* GameLayer::createPromptPieceLayer(float cellSize, int rows, int columns)
{
  PromptLayer* pPromptLayer = new(std::nothrow) PromptLayer(cellSize, rows, columns);
  if (pPromptLayer && pPromptLayer->init())
  {
    pPromptLayer->autorelease();
    return pPromptLayer;
  }
  else
  {
    delete pPromptLayer;
    pPromptLayer = nullptr;
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

AILogic* GameLayer::createAILogic(Player turn)
{
  AILogic* pAILogic = new(std::nothrow) AILogic(turn);
  if (pAILogic && pAILogic->init())
  {
    pAILogic->autorelease();
    return pAILogic;
  }
  else
  {
    delete pAILogic;
    pAILogic = nullptr;
    return nullptr;
  }

  return nullptr;
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

void GameLayer::applyPromotion()
{
  m_promotionLayer->hide();
}

void GameLayer::movePiece(const Size& move_from, const Size& move_to)
{
  std::string to_record;

  Position present;
  present.iColumn = move_from.height;
  present.iRow = move_from.width;

  std::string presentStr = m_pieceMoveLogic->parseMoveCellIntToString(present);

  //Put in the string to be logged
  to_record += presentStr;

  int iPiece = m_pieceMoveLogic->getPieceAtPositionI(present.iRow, present.iColumn);

  //cout << "Piece is " << char(chPiece) << "\n";

  if (Constants::EMPTY_SQUAREI == iPiece)
  {
    //createNextMessage("You picked an EMPTY square.\n");
    return;
  }

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

  if (false == m_pieceMoveLogic->isMoveValid(present, future, &S_enPassant, &S_castling, &S_promotion))
  {
    //createNextMessage("[Invalid] Piece can not move to that square!\n");
    return;
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

  // ---------------------------------------------------
// Promotion: user most choose a piece to
// replace the pawn
// ---------------------------------------------------
  if (S_promotion.bApplied == true)
  {
    bool isWhite = (static_cast<int>(Player::WHITE_PLAYER) == m_pieceMoveLogic->getCurrentTurn());

	if (!Globals::onePlayer)
	{
		// Set callBack
		auto lfHidePromotion = [this, present, future, &S_promotion, isWhite](int typePiece)
		{
			Size sPresent(future.iRow, future.iColumn);
			Size sFuture(future.iRow, future.iColumn);

			m_board->removePieceN(sFuture);
			m_board->addPieceN(typePiece, m_pieceMoveLogic->getCurrentTurn(), sFuture, static_cast<int>(ZOrderGame::PIECE));

			int k = (isWhite) ? -1 : 1;
			m_pieceMoveLogic->updateBoardA(k * typePiece, sFuture);
		};

		m_promotionLayer->callBackHide(lfHidePromotion);

		m_promotionLayer->show(!isWhite);
	}
	else 
	{
		//Get random figure KNIGHT, ROOK, QUEEN, BISHOP
		TypePiece randomPiece { TypePiece::KNIGHT };
		std::array<TypePiece, 4> pieceTypes{ TypePiece::ROOK, TypePiece::KNIGHT, TypePiece::BISHOP, TypePiece::QUEEN };
		int r = rand() % 4;
		int randomTypePiece = static_cast<int>(pieceTypes[r]);

		Size sPresent(future.iRow, future.iColumn);
		Size sFuture(future.iRow, future.iColumn);

		m_board->removePieceN(sFuture);
		m_board->addPieceN(randomTypePiece, m_pieceMoveLogic->getCurrentTurn(), sFuture, static_cast<int>(ZOrderGame::PIECE));

		int k = (isWhite) ? -1 : 1;
		m_pieceMoveLogic->updateBoardA(k * randomTypePiece, sFuture);
	}
    

    return;
  }

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

void GameLayer::promotionPiece(int typePromotion, bool isWhite, const cocos2d::Size& to)
{
  m_board->removePieceN(to);
  m_board->addPieceN(typePromotion, isWhite, to, static_cast<int>(ZOrderGame::PIECE));
}

void GameLayer::makeTheMove(const Size& present, const Size& future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion)
{
  int iPiece = m_pieceMoveLogic->getPieceAtPositionI(present.width, present.height);

  // -----------------------
  // Captured a piece?
  // -----------------------
  if (m_pieceMoveLogic->isSquareOccupied(future.width, future.height))
  {
    int iAuxPiece = m_pieceMoveLogic->getPieceAtPositionI(future.width, future.height);

    if (Piece::getColor(iPiece) != Piece::getColor(iAuxPiece))
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

  Position ppresent;
  ppresent.iRow = present.width;
  ppresent.iColumn = present.height;

  Position pfuture;
  pfuture.iRow = future.width;
  pfuture.iColumn = future.height;

  m_pieceMoveLogic->movePiece(ppresent, pfuture, S_enPassant, S_castling, S_promotion);
}


Board* GameLayer::getBoard()
{
  return m_board;
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

void GameLayer::forwardOneMoveLogic(PieceMove& pieceMove)
{
  MoveData moveData;
  pieceMove.getMoveData(moveData);

  Position moveTo = moveData.to;
  Position moveFrom = moveData.from;

  m_pieceMoveLogic->updateBoardA(moveData.piece, Size(moveTo.iRow, moveTo.iColumn));
  m_pieceMoveLogic->updateBoardA(0, Size(moveFrom.iRow, moveFrom.iColumn));
}

void GameLayer::undoOneMoveLogic(PieceMove& pieceMove)
{
  MoveData moveData;
  pieceMove.getMoveData(moveData);

  Position moveTo = moveData.to;
  Position moveFrom = moveData.from;

  bool isCaptured { false };
  isCaptured = (moveData.captured && abs(moveData.captured) > 0) ? true : false;

  if (isCaptured || pieceMove.isCapturingPromotion())
  {
	  // ?????????????????????????? CheckThis
	  m_pieceMoveLogic->updateBoardA(moveData.captured, Size(moveTo.iRow, moveTo.iColumn));
  }
  else if (pieceMove.isPromotion())
  {
	  m_pieceMoveLogic->updateBoardA(moveData.typeBefore, Size(moveTo.iRow, moveTo.iColumn));
  }
  else if(pieceMove.isSimple())
  {
	  m_pieceMoveLogic->updateBoardA(0, Size(moveTo.iRow, moveTo.iColumn));
  }

  m_pieceMoveLogic->updateBoardA(moveData.piece, Size(moveFrom.iRow, moveFrom.iColumn));
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

bool GameLayer::checkPieceMove(const cocos2d::Size& moveFrom, const cocos2d::Size& moveTo, PieceMove& pieceMove)
{
  EnPassant  S_enPassant = { 0 };
  Castling   S_castling = { 0 };
  Promotion  S_promotion = { 0 };

  Position present { (int)(moveFrom.width), (int)(moveFrom.height) };
  Position future { (int)(moveTo.width), (int)(moveTo.height) };

  bool isMoveValid = m_pieceMoveLogic->isMoveValid(present, future, &S_enPassant, &S_castling, &S_promotion);

  return isMoveValid;
}

void GameLayer::getTypePieceMove(const cocos2d::Size& moveFrom, const cocos2d::Size& moveTo, Player turn, PieceMove& pieceMove)
{
  EnPassant  S_enPassant = { 0 };
  Castling   S_castling = { 0 };
  Promotion  S_promotion = { 0 };

  Position present { (int)(moveFrom.width), (int)(moveFrom.height) };
  Position future { (int)(moveTo.width), (int)(moveTo.height) };

  m_pieceMoveLogic->isMoveValid(present, future, &S_enPassant, &S_castling, &S_promotion);

  int piece = m_pieceMoveLogic->getPieceAtPositionI(moveFrom.width, moveFrom.height);

  int capturedPiece = m_pieceMoveLogic->getPieceAtPositionI(moveTo.width, moveTo.height);

  bool isCapturedApplied{ false };

  if (m_pieceMoveLogic->getCurrentTurn() == static_cast<int>(turn))
  {
    isCapturedApplied = (abs(capturedPiece > 0)) ? true : false;
  }

  if (isCapturedApplied && !S_enPassant.bApplied)
  {
    CaptureT captureMove { present, future, piece, capturedPiece, false };
    pieceMove.details = captureMove;
  }
  else if (S_enPassant.bApplied == true)
  {
    Position enPassant = S_enPassant.PawnCaptured;
    int enPassantCaptured = m_pieceMoveLogic->getPieceAtPositionI(enPassant.iColumn, enPassant.iRow);

    CaptureT captureMove { present, future, piece, enPassantCaptured, true };
    pieceMove.details = captureMove;
  }
  else if (S_castling.bApplied == true)
  {
    CastleT castleMove{ present, future, S_castling.rook_before, S_castling.rook_after, S_castling.bKingSide };

    pieceMove.details = castleMove;
  }
  else if (S_promotion.bApplied == true && !isCapturedApplied) // promotion move
  {
    PromotionT promotionMove { present, future, S_promotion.typeBefore, S_promotion.typeAfter };

    pieceMove.details = promotionMove;
  }
  else if (S_promotion.bApplied == true && isCapturedApplied) // capturedPromotion move
  {
    CapturePromotionT capturePromotionMove{ present, future, capturedPiece, S_promotion.typeBefore, S_promotion.typeAfter };

    pieceMove.details = capturePromotionMove;
  }
  else // simple move
  {
    SimpleT simpleMove { present, future, piece };

    pieceMove.details = simpleMove;
  }
}

void GameLayer::undoMove(void)
{
  if (false == m_pieceMoveLogic->undoIsPossible())
  {
    //createNextMessage("Undo is not possible now!\n");
    return;
  }

  m_pieceMoveLogic->undoLastMove();

  m_promptLayer->hideCirclePrompts();
  //createNextMessage("Last move was undone\n");
}



