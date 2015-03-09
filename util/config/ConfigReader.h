#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include <map>
#include <sstream>
#include <fstream>
#include "include\sca_platformtype.h"

namespace config{
  template <typename ParseRule>
  class BasicReader{
    class Impl{
      friend BasicReader;
      std::map<std::string, std::string> PropertyMap;
    };
    Impl* Impl_;
  private:
   
    void parse(const std::string& iniFilePath){
      std::ifstream file;
      file.open(iniFilePath); 
      
      if (file.good()){
        std::string id, eq, val, line = "";
        
        while (std::getline(file, line)){
          std::stringstream ss(line);
          ss >> id >> eq >> val;
          if (id[0] == ParseRule::getCommentSymbol()) continue;
          if (id[0] == ParseRule::getCategorySymbol()) continue;
          if (eq.compare(ParseRule::getParseSymbol()) != 0) {
            throw std::exception("wrong ini syntax");  // change to other type of exception
          }
          Impl_->PropertyMap[id] = val;
        }
      }

    }
  public:
    BasicReader() : Impl_(new Impl){}
    ~BasicReader() { delete Impl_; }
    template <typename ConfigClass>
    ConfigClass readIniFile(const std::string& iniFilePath) {
      try{
        parse(iniFilePath);
      }
      catch (std::exception& e){
        throw e; // TODO: change to own type of exception 
      }
      ConfigClass c;
      c.setProprietaries(Impl_->PropertyMap);
      return std::move(c);
    }
  };

  class ParseOnEqual{
  public:
    static const std::string getParseSymbol(){
      return "=";
    }
    static const char getCommentSymbol(){
      return '#';
    }
    static const char getCategorySymbol(){
      return '[';
    }

  };

  using ConfigReader = BasicReader<ParseOnEqual>;

}
#endif //CONFIG_READER_H_