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

/*Piece::Piece(const Piece& Piece)
{
  this->init(Piece._textureFile, cocos2d::ui::ImageView::TextureResType::PLIST);
  this->m_type = Piece.m_type;
  this->m_isWhite = Piece.m_isWhite;
  this->autorelease();
}*/

Piece::Piece(const Piece& Piece)
{
  this->init(Piece.m_fileName, cocos2d::ui::ImageView::TextureResType::PLIST);
  this->m_type = Piece.m_type;
  this->m_isWhite = Piece.m_isWhite;
  //this->autorelease();
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

Piece* Piece::clonePiece()
{
  return new Piece(*this);
}

TypePiece Piece::getType()
{
  return m_type;
}


