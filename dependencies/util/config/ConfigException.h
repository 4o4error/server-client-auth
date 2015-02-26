#ifndef CONFIG_EXCEPTION_H_
#define CONFIG_EXCEPTION_H_

#include <exception>
#include "config_reader_build_opts.h"
namespace config{
  class CONFIG_READER_EXPORT_SYMBOL ConfigException : public std::exception{
  protected:
    const char* exception_;
    ConfigException(const char* exception);
  public:
    ConfigException(){}
    const char* what() const override;
  };
}

#endif  // CONFIG_EXCEPTION_H_