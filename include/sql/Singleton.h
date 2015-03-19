
#include "sqlOp.h"
#include <iostream>

class Singleton
{
private:
   Singleton()
  {
    
  }
public:
  // copying may cause unnintended 
  // instance copies
  Singleton(const Singleton& other) = delete;
  Singleton& operator=(const Singleton& other) = delete;
  static SqlOp& Singleton::getInstance()
  {
    static SqlOp single;
    return single;
  }
};
