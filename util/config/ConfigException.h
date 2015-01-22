#ifndef CONFIG_EXCEPTION_H_
#define CONFIG_EXCEPTION_H_

#include <exception>

namespace config{
  class ConfigException : public std::exception{
  protected:
    const char* exception_;
    ConfigException(const char* exception);
    const char* what() const override;
  };
}

#endif  // CONFIG_EXCEPTION_H_