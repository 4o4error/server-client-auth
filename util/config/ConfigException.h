#ifndef CONFIG_EXCEPTION_H_
#define CONFIG_EXCEPTION_H_

#include <exception>
#include "sca_platformtype.h"
namespace config{
  class SCA_EXPORT_SYMBOL ConfigException : public std::exception{
  protected:
    const char* exception_;
    ConfigException(const char* exception);
  public:
    ConfigException(){}
    const char* what() const throw() override;
  };
}

#endif  // CONFIG_EXCEPTION_H_