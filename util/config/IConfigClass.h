#ifndef ICONFIGCLASS_H_
#define ICONFIGCLASS_H_
#include <map>
#include <string>

namespace config{
  class IConfigClass{
  public:
    virtual void setProprietaries(std::map<std::string, std::string>& ProprietariesMap) = 0;
  };
}
#endif // ICONFIGCLASS_H_