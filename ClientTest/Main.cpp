#include "SslClientAuthDll.h"

int main(){ 
  try{
    client& c = client::Instance();
    c.initRemoteHostData("localhost", "4242", "cacert.pem");
    c.get_license();
    std::cout << c.license() << std::endl;
    system("PAUSE");
  }
  catch (std::exception& e){
    std::cout << e.what() << std::endl;
  }
  return 0;
}


