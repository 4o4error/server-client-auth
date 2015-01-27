#ifndef CONFIG_EXCEPTION_H_
#define CONFIG_EXCEPTION_H_

#include <exception>
#include "util_build_opts.h"
namespace config{
  class UTIL_EXPORT_SYMBOL ConfigException : public std::exception{
  protected:
    const char* exception_;
    ConfigException(const char* exception);
    const char* what() const override;
  };
}

#endif  // CONFIG_EXCEPTION_H_