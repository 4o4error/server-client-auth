#ifndef ICONFIGCLASS_H_
#define ICONFIGCLASS_H_
#include <map>
#include <string>
#include "config_reader_build_opts.h"
namespace config{
  class CONFIG_READER_EXPORT_SYMBOL IConfigClass{
  public:
    virtual void setProprietaries(std::map<std::string, std::string>& ProprietariesMap) = 0;
  };
}
#endif // ICONFIGCLASS_H_