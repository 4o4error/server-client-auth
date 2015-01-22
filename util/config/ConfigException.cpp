#include "ConfigException.h"

namespace config{
  ConfigException::ConfigException(const char* exception): exception_(exception){}
  const char* ConfigException::what() const {
    return exception_;
  }
}