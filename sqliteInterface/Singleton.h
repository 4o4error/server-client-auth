
#include "sqlOp.h"
#include <iostream>

class Singleton
{
private:
   Singleton()
  {
    
  }
public:
  // always provide constructors/ destructors 
  Singleton(Singleton& const other) = delete;
  Singleton& operator=(Singleton& const other) = delete;
  static SqlOp& Singleton::getInstance()
  {
    static SqlOp single;
    return single;
  }
};
