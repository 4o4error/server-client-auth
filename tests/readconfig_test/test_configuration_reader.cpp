#include "util\config\ConfigReader.h"
#include "util\config\ConfigClass.h"
#include "gtest\gtest.h"
#include <iostream>

using namespace config;

class GoodConfigTest : public ::testing::Test{
  ConfigReader cr;
  ConfigClass c;
protected:
	virtual void SetUp(){
		c = cr.readIniFile<ConfigClass>("C:\\Users\\Krull\\Documents\\Visual Studio 2013\\Projects\\ConfigFileReader\\test.conf");
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
     c = cr.readIniFile<ConfigClass>("C:\\Users\\Krull\\Documents\\Visual Studio 2013\\Projects\\ConfigFileReader\\testincorect.conf");
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
      c = cr.readIniFile<ConfigClass>("C:\\Users\\Krull\\Documents\\Visual Studio 2013\\Projects\\ConfigFileReader\\testmalformed.conf");
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

