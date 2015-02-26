#include "DbConnection.h"
#include <algorithm>
namespace dbconn{
  std::string random_string(size_t length)
  {
    auto randchar = []() -> char
    {
      const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
      const size_t max_index = (sizeof(charset)-1);
      return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
  }
  const std::string DbConnection::generateLicense(){
   
    return random_string(10);
  }
  DbConnection::DbConnection()
  {
    count = 0;
  }

  DbConnection::~DbConnection()
  {
  }
  int DbConnection::count;
}