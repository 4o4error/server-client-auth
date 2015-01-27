#include "ConfigClass.h"

namespace config{
  class ConfigClass::Impl{
    friend ConfigClass;
    std::string host_ = "";
    std::string port_ = "";
    std::string id_ = "";
    bool useDefaultValues_ = false;
  };

  ConfigClass::ConfigClass() : Impl_(new Impl){ }
  ConfigClass::~ConfigClass() { delete Impl_; }
  const std::string ConfigClass::getHost() const{ return Impl_->host_; }
  const std::string ConfigClass::getPort() const { return Impl_->port_; }
  const std::string ConfigClass::getId() const{ return Impl_->id_; }

  void  ConfigClass::setProprietaries(std::map<std::string, std::string>& ProprietariesMap) {
    try{
      Impl_->host_ = ProprietariesMap.at("host");
      Impl_->port_ = ProprietariesMap.at("port");
      Impl_->id_ = ProprietariesMap.at("id");
    }
    catch (std::exception& e){
      std::cout << e.what() << std::endl;
    }
  }
  ConfigClass::ConfigClass(ConfigClass&& other){
    if (this != &other){
      Impl_ = new Impl;
      Impl_->host_ = other.getHost();
      Impl_->port_ = other.getPort();
      Impl_->id_ = other.getId();
    }
  }

  ConfigClass::ConfigClass(const ConfigClass& other){
    if (this != &other){
      Impl_ = new Impl;
      Impl_->host_ = other.getHost();
      Impl_->port_ = other.getPort();
      Impl_->id_ = other.getId();
    }
  }
  ConfigClass&  ConfigClass::operator=(ConfigClass& other){
    if (this != &other){
      Impl_ = new Impl;
      Impl_->host_ = other.getHost();
      Impl_->port_ = other.getPort();
      Impl_->id_ = other.getId();
    }
    return *this;
  }

}