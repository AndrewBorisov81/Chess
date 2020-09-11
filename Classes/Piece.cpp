#include "Piece.h"
#include "Constants.h"

#include <string>

USING_NS_CC;
using namespace cocos2d::ui;

Piece::Piece() :
  cocos2d::ui::ImageView()
{
}

Piece::Piece(int type, bool isWhite)
  :
  m_isWhite(isWhite),
  cocos2d::ui::ImageView()
{

}

Piece::Piece(int type, bool isWhite, const std::string& fileName)
  :
  m_isWhite(isWhite),
  m_fileName(fileName),
  cocos2d::ui::ImageView()
{

}

Piece::~Piece()
{
}


bool Piece::isWhite(int iPiece)
{
  bool isWhitePiece = (iPiece < 0) ? false : true;
  return isWhitePiece;
}

bool Piece::isBlack(int iPiece)
{
  bool isBlackPiece = (iPiece > 0) ? false : true;
  return isBlackPiece;
}

PieceColor Piece::getColor(int iPiece)
{
  PieceColor pieceColor = (iPiece > 0) ? PieceColor::WHITE_PIECE : PieceColor::BLACK_PIECE;

  return pieceColor;
}

TypePiece Piece::getTypeP(int iPiece)
{
  TypePiece typePiece{ TypePiece::PAWN };

  switch (iPiece)
  {
    case 1:
      typePiece = TypePiece::ROOK;
      break;
    
    case 2:
      typePiece = TypePiece::KNIGHT;
      break;

    case 3:
      typePiece = TypePiece::BISHOP;
      break;

    case 4:
      typePiece = TypePiece::QUEEN;
      break;

    case 5:
      typePiece = TypePiece::KING;
      break;

    case 6:
      typePiece = TypePiece::PAWN;
      break;
  }
  return typePiece;
}

Piece* Piece::createPiece(int type, bool isWhite, std::string fileName)
{
  Piece* pPiece = new(std::nothrow) Piece(type, isWhite, fileName);

  if (pPiece && pPiece->init(fileName, cocos2d::ui::ImageView::TextureResType::PLIST))
  {
    pPiece->autorelease();
    return pPiece;
  }
  else
  {
    delete pPiece;
    pPiece = nullptr;
    return nullptr;
  }
  return pPiece;
}

bool Piece::isWhite()
{
  return m_isWhite;
}

void Piece::setType(TypePiece type)
{
  m_type = type;
}

void Piece::setIsWhite(bool isWhite)
{
  m_isWhite = isWhite;
}

void Piece::setCell(const Size& cellRowColumn)
{
  m_cell = cellRowColumn;
}

Size Piece::getCell()
{
  return m_cell;
}

cocos2d::Rect Piece::getRect()
{
  auto s = this->getContentSize();
  return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}

TypePiece Piece::getType()
{
  return m_type;
}


