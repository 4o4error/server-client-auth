#include "util\config\ConfigReader.h"
#include "util\config\ConfigClass.h"
#include "util\hash\Hash.h"
#include "gtest\gtest.h"
#include <iostream>

using namespace config;

void main(){

  Hash hash;
  std::string password = "Areyou92!@#$%";
  std::string cript = hash.Encrypt(password);
  std::cout << password << std::endl;
  std::cout << cript << std::endl;
  std::cout << hash.Decrypt(cript) << std::endl;

}


/*
class GoodConfigTest : public ::testing::Test{
  ConfigReader cr;
  ConfigClass c;
protected:
	virtual void SetUp(){
		c = cr.readIniFile<ConfigClass>("C:\\Users\\alexandra.vintila\\Documents\\GitHub\\Repo\\server-client-auth\\tests\\readconfig_test\\test.conf");
	}
	//virtual void TearDown(){}
	ConfigClass& getConfigClass(){
		return c;
	}
};
class BadConfigTest : public ::testing::Test {
  ConfigReader cr;
  ConfigClass c;
protected:
  virtual void SetUp(){
     c = cr.readIniFile<ConfigClass>("C:\\Users\\alexandra.vintila\\Documents\\GitHub\\Repo\\server-client-auth\\tests\\readconfig_test\\testincorect.conf");
  }
  //virtual void TearDown(){}
  ConfigClass& getConfigClass(){
    return c;
  }
};

class MalformedConfigTest : public ::testing::Test{
  ConfigReader cr;
  ConfigClass c;
protected:
  virtual void SetUp(){
    try{
      c = cr.readIniFile<ConfigClass>("C:\\Users\\alexandra.vintila\\Documents\\GitHub\\Repo\\server - client - auth\\tests\\readconfig_test\\testmalformed.conf");
    }
    catch (std::exception& e){
      throw e;
    }
  }
  //virtual void TearDown(){}
  ConfigClass& getConfigClass(){
    return c;
  }
};

TEST_F(GoodConfigTest, TestHostCorrect){
  ConfigClass c = getConfigClass();
	EXPECT_EQ(0, c.getHost().compare("localhost"));
}
TEST_F(GoodConfigTest, TestPortCorrect){
  ConfigClass c = getConfigClass();
	EXPECT_EQ(0, c.getPort().compare("8080"));
}
TEST_F(GoodConfigTest, TestIDCorrect){
  ConfigClass c = getConfigClass();
	EXPECT_EQ(0, c.getId().compare("vlad"));
}
TEST_F(BadConfigTest, TestHostIncorrect){
  ConfigClass c = getConfigClass();
  EXPECT_EQ(1, c.getHost().compare("localhost"));
}
TEST_F(BadConfigTest, TestPortIncorrect){
  ConfigClass c = getConfigClass();
  EXPECT_EQ(1, c.getPort().compare("8080"));
}
TEST_F(BadConfigTest, TestIDIncorrect){
  ConfigClass c = getConfigClass();
  EXPECT_EQ(1, c.getId().compare("vlad"));
}

TEST_F(MalformedConfigTest, TestHostMalformed){
  
    ConfigClass c = getConfigClass();
    EXPECT_EQ(1, c.getHost().compare("localhost"));

}
TEST_F(MalformedConfigTest, TestPortMalformed){
  ConfigClass c = getConfigClass();
  EXPECT_EQ(1, c.getHost().compare("localhost"));
}
TEST_F(MalformedConfigTest, TestIDMalformed){
  ConfigClass c = getConfigClass();
  EXPECT_EQ(1, c.getHost().compare("localhost"));
}
int main(int argc, char **argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

*/