#include "Figure.h"
#include "Constants.h"

#include <string>

USING_NS_CC;
using namespace cocos2d::ui;

Figure::Figure() :
  cocos2d::ui::ImageView()
{
}

Figure::Figure(TypeFigure type, ColourFigure colour, const std::string& fileName) 
  :
  m_type(type),
  m_colour(colour),
  cocos2d::ui::ImageView()
{

}


/*Figure::Figure(TypeFigure type, bool isWhite, const std::string & fileName)
  : cocos2d::ui::ImageView()
{
}*/

Figure::~Figure()
{
}

Figure* Figure::createFigure(int type, bool isWhite, const std::string& fileName)
{
  TypeFigure typeFigure{ TypeFigure::PAWN };
  ColourFigure colourFigure{ ColourFigure::WHITE };
  //std::string fileNameFigure = Constants::WHITE_PAWN_PNG;

  if (!isWhite) colourFigure = ColourFigure::BLACK;

  switch (type)
  {
  case 1:
    typeFigure = TypeFigure::ROOK;
    //fileNameFigure = (colour == static_cast<int>(ColourFigure::WHITE)) ? Constants::WHITE_ROOK_PNG : Constants::BLACK_ROOK_PNG;
    break;

  case 2:
    typeFigure = TypeFigure::HORSE;
    //fileNameFigure = (colour == static_cast<int>(ColourFigure::WHITE)) ? Constants::WHITE_HORSE_PNG : Constants::BLACK_HORSE_PNG;
    break;

  case 3:
    typeFigure = TypeFigure::KNIGHT;
    //fileNameFigure = (colour == static_cast<int>(ColourFigure::WHITE)) ? Constants::WHITE_KNIGHT_PNG : Constants::WHITE_KNIGHT_PNG;
    break;

  case 4:
    typeFigure = TypeFigure::QUEEN;
    //fileNameFigure = (colour == static_cast<int>(ColourFigure::WHITE)) ? Constants::WHITE_QUEEN_PNG : Constants::WHITE_QUEEN_PNG;
    break;

  case 5:
    typeFigure = TypeFigure::KING;
    //fileNameFigure = (colour == static_cast<int>(ColourFigure::WHITE)) ? Constants::WHITE_KING_PNG : Constants::WHITE_KING_PNG;
    break;
  }

  //Figure* pFigure = new(std::nothrow) Figure(typeFigure, colourFigure, fileNameFigure);
  Figure* pFigure = new(std::nothrow) Figure(typeFigure, colourFigure, "Hello!");

  if (pFigure && pFigure->init(fileName, cocos2d::ui::ImageView::TextureResType::PLIST))
  {
    pFigure->autorelease();
    return pFigure;
  }
  else
  {
    delete pFigure;
    pFigure = nullptr;
    return nullptr;
  }
  return pFigure;
}

ColourFigure Figure::getColour()
{
  return m_colour;
}

TypeFigure Figure::getType()
{
  return m_type;
}


