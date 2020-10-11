#include "PromptLayer.h"
#include "PromptLogicHelper.h"
#include "PromptElement.h"
#include "Constants.h"
#include "LogicData.h"

USING_NS_CC;

PromptLayer::PromptLayer()
{

}

PromptLayer::PromptLayer(float cellSize, int rows, int columns)
  : Grid(cellSize, rows, columns)
{

}

PromptLayer::~PromptLayer()
{

}

bool PromptLayer::init()
{
  Grid::init();

  if (!CCLayer::init()) {

    return false;
  }

  // Create prompts
  PromptElement* presentPromptRect = PromptElement::createPromptElement(TypePrompt::PROMPT_RECT);
  this->addChild(presentPromptRect, 1);
  presentPromptRect->setPosition(getPointByCell(5, 1));
  m_presentPromptRect = presentPromptRect;
  presentPromptRect->hide();

  PromptElement* futurePromptRect = PromptElement::createPromptElement(TypePrompt::PROMPT_RECT);
  this->addChild(futurePromptRect, 1);
  futurePromptRect->setPosition(getPointByCell(5, 5));
  m_futurePromptRect = futurePromptRect;
  futurePromptRect->hide();

  for (unsigned int i = 0; i < m_circles.size(); i++)
  {
    PromptElement* promptCircle = PromptElement::createPromptElement(TypePrompt::PROMPT_CIRCLE);
    this->addChild(promptCircle, 1);
    promptCircle->hide();
    m_circles[i] = promptCircle;
  }

  // Create promptLogicHelper(get possible moves)
  PromptLogicHelper* promptLogicHelper = PromptLogicHelper::createPromptLogicHelper();
  m_promptLogicHelper = promptLogicHelper;
  this->addChild(promptLogicHelper, 1);

  return true;
}

void PromptLayer::showRectPrompts(bool isToGreenRect)
{
  m_futurePromptRect->setColor(ColorPrompt::GREEN);

  if (!isToGreenRect)
  {
    m_futurePromptRect->setColor(ColorPrompt::RED);
  }

  m_presentPromptRect->show();
  m_futurePromptRect->show();
}

void PromptLayer::setPositionRects(const cocos2d::Size & from, const cocos2d::Size & to)
{
  m_presentPromptRect->setPosition(getPointByCell(from.height, from.width));
  m_futurePromptRect->setPosition(getPointByCell(to.height, to.width));
}

void PromptLayer::hideRectPrompts()
{
  m_presentPromptRect->hide();
  m_futurePromptRect->hide();
}

void PromptLayer::showCirclePrompts(const std::vector<cocos2d::Size>& valideMoves)
{
  for (auto toShowCell: valideMoves)
  {
    for (auto circle : m_circles)
    {
      Size circleInBox = circle->getCellPos();

      if(toShowCell.width == circleInBox.width && toShowCell.height == circleInBox.height)
        circle->show();
    }
  }    
}

void PromptLayer::hideCirclePrompts()
{
  for (auto el : m_circles)
    el->hide();
}

void PromptLayer::setPositionCircles(const std::vector<cocos2d::Size>& valideMoves)
{
  for (unsigned int i = 0; i < valideMoves.size(); i++)
  {
    Size cell = valideMoves[i];
    PromptElement* circlePrompt = m_circles[i];
    circlePrompt->setPosition(getPointByCell(cell.height, cell.width));
    circlePrompt->setCellPos(Size(cell.width, cell.height));
  }
}

void PromptLayer::getValideMoves(int typePiece, const cocos2d::Size& presentCell, std::vector<cocos2d::Size>& valideMoves)
{
  // No check is move valide
  std::vector<cocos2d::Size> possibleMoves;

  m_promptLogicHelper->getPossibleMoves(typePiece, presentCell, possibleMoves);

  for (auto el : possibleMoves)
  {
    bool isMoveValide{ false };

    if(m_isMoveValideCallBack)
      isMoveValide = m_isMoveValideCallBack(presentCell, el);

    if (isMoveValide)
      valideMoves.push_back(el);
  }
}

void PromptLayer::callBackIsMoveValide(const std::function<bool(const cocos2d::Size& presentCell, const cocos2d::Size& futureCell)> isMoveValide)
{
  m_isMoveValideCallBack = isMoveValide;
}

