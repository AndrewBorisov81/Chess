#include "PromptLogicHelper.h"

USING_NS_CC;

PromptLogicHelper::PromptLogicHelper()
{

}

PromptLogicHelper::~PromptLogicHelper()
{

}

bool PromptLogicHelper::init()
{
  if (!Node::init())
  {
    return false;
  }

  return true;
}
