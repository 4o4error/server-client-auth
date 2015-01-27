#include <iostream>
#include <string>

class Hash{

private:
  int key;
public:
  Hash();
  std::string Encrypt(std::string const& s);
  std::string Decrypt(std::string const& s);

};

