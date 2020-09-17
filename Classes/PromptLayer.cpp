#include "PromptLayer.h"
#include "Constants.h"
#include "PromptElement.h"

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
  }

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

void PromptLayer::showCirclePrompts()
{

}

void PromptLayer::hideCirclePrompts()
{

}

