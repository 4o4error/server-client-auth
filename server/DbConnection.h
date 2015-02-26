#ifndef DBCONNECTION_H_
#define DBCONNECTION_H_
#include <string>

namespace dbconn{

  class DbConnection
  {
    static int count;
  public:
    DbConnection();
    ~DbConnection();
    const std::string generateLicense();
  private:

  };
}


#endif