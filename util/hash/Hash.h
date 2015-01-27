#include <iostream>
#include <string>
#include "../config/util_build_opts.h"
namespace config{
  class UTIL_EXPORT_SYMBOL Hash{

  public:
    std::string Encrypt(std::string const& s);
    std::string Decrypt(std::string const s);

  };
}

