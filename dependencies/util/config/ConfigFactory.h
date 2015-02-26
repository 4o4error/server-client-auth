#include "ConfigReader.h"

namespace config{
  class ConfigReaderFactory{
  private:
    ConfigReaderFactory() = delete;
    ConfigReaderFactory(ConfigReaderFactory& other) = delete;
  public:
    static ConfigReader& getInstace(){
      static ConfigReader INSTANCE;
      return INSTANCE;
    }
    \
  };
  
}