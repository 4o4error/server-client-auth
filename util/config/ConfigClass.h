#ifndef CONFIGCLASS_H_
#define CONFIGCLASS_H_

#include "IConfigClass.h"
#include <iostream>

#include "util_build_opts.h"

namespace config{

  class UTIL_EXPORT_SYMBOL ConfigClass : public IConfigClass{
  private:
    class Impl;
    Impl* Impl_;
  public:
    ConfigClass();
    ~ConfigClass();
    ConfigClass(ConfigClass&& other);
    ConfigClass(const ConfigClass& other);
    ConfigClass& operator=(ConfigClass& other);

    const std::string& getHost() const;
    const std::string& getPort() const;
    const std::string& getId() const;

    void setProprietaries(std::map<std::string, std::string>& ProprietariesMap) override;

  };
}
#endif // CONFIGCLASS_H_