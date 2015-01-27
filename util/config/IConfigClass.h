#ifndef ICONFIGCLASS_H_
#define ICONFIGCLASS_H_
#include <map>
#include <string>
#include "util_build_opts.h"
namespace config{
  class UTIL_EXPORT_SYMBOL IConfigClass{
  public:
    virtual void setProprietaries(std::map<std::string, std::string>& ProprietariesMap) = 0;
  };
}
#endif // ICONFIGCLASS_H_