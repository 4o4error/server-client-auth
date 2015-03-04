
#include "sqlOp.h"
#include <iostream>

class Singleton
{
private:
  static bool instanceFlag;
  
  Singleton()
  {
    
  }
public:

  void method();
  ~Singleton()
  {
    instanceFlag = false;
  }
  static SqlOp* Singleton::getInstance()
  {
    static SqlOp single;
    return &single;
  
  }
};
