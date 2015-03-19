#include "util\config\ConfigException.h"

namespace config{
  ConfigException::ConfigException(const char* exception): exception_(exception){}
  const char* ConfigException::what() const throw(){
    return exception_;
  }
}