
#include "Hash.h"
namespace config{
  /*
  // Receive as parameter a string that is hashed 
  // Return string containing hashed result
  */
  std::string Hash::Encrypt(std::string const& strTarget)
{
    int len = strTarget.length();
    char a;
    std::string strFinal(strTarget);

    for (int i = 0; i <= (len - 1); i++)
    {
      a = strTarget.at(i);
      int b = (int)a; //get the ASCII value of 'a'
      b += 2; //Mulitply the ASCII value by 2
      if (b > 254) { b = 254; }
      a = (char)b; //Set the new ASCII value back into the char
      strFinal.insert(i, 1, a); //Insert the new Character back into the string
    }
    std::string strEncrypted(strFinal, 0, len);
    return strEncrypted;
  }


  /*
  // Receive as parameter a string containing hashed value 
  // Return string containing unhashed result
  */
  std::string Hash::Decrypt(std::string const strTarget)
{
  int len = strTarget.length();
  char a;
  std::string strFinal(strTarget);

  for (int i = 0; i <= (len - 1); i++)
  {
    a = strTarget.at(i);
    int b = (int)a;
    b -= 2;

    a = (char)b;
    strFinal.insert(i, 1, a);
  }
  std::string strDecrypted(strFinal, 0, len);
  return strDecrypted;
}
 
}