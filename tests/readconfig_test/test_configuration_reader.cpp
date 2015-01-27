#include "util\config\ConfigReader.h"
#include "util\config\ConfigClass.h"
#include "util\config\ConfigException.h"
#include "gtest\gtest.h"
#include <iostream>
#include <util\config\ConfigFactory.h>
using namespace config;

class GoodConfigTest : public ::testing::Test {
protected:
  
  ConfigReader cr;
  ConfigClass c;
  // You can define per-test set-up and tear-down logic as usual.
  virtual void SetUp() {
    cr = ConfigReaderFactory::getInstace();
    c = cr.readIniFile<ConfigClass>("../../../tests/readconfig_test/test.conf");
  }
};
class BadConfigTest : public ::testing::Test {
protected:
  ConfigReader cr;
  ConfigClass c;
  // You can define per-test set-up and tear-down logic as usual.
  virtual void SetUp() {
    cr = ConfigReaderFactory::getInstace();
    c = cr.readIniFile<ConfigClass>("../../../tests/readconfig_test/testincorect.conf");
  }
};
class MalformedConfigTest : public ::testing::Test {
protected:
  ConfigReader cr;
};


  TEST_F(GoodConfigTest, TestHostCorrect){
    EXPECT_EQ(0, c.getHost().compare("localhost"));
  }
  TEST_F(GoodConfigTest, TestPortCorrect){
    EXPECT_EQ(0, c.getPort().compare("8080"));
  }
  TEST_F(GoodConfigTest, TestIDCorrect){
    EXPECT_EQ(0, c.getId().compare("vlad"));
  }
  TEST_F(BadConfigTest, TestHostIncorrect){
    EXPECT_EQ(1, c.getHost().compare("localhost"));
  }
  TEST_F(BadConfigTest, TestPortIncorrect){
    EXPECT_EQ(1, c.getPort().compare("8080"));
  }
  TEST_F(BadConfigTest, TestIDIncorrect){
    EXPECT_EQ(1, c.getId().compare("vlad"));
  }

  

int main(int argc, char **argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

