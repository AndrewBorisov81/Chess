#pragma once

#include <string>

#include "cocos2d.h"

class Connector: public cocos2d::Node
{
public:

  Connector();
  ~Connector();

  virtual bool init();

  static Connector* createConnector();

  void Connector::ConnectToEngine(char* path);

  std::string getNextMove(std::string position);

  void Connector::CloseConnection();

private:

};
