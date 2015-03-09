#include "gtest\gtest.h"
#include "client\SslClientAuthDll.h"
#include <thread>
#include <vector>
#include <map>
void simulateWork(){
  std::thread t([](){
    while (true){
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  });
  t.join();
}


class TestMultipleThreads : public ::testing::Test{
protected:
  std::vector<std::thread> threads;
  std::vector<auth::client*> clients;
  std::map<std::string, int> duplicates;
  /* virtual void SetUpTestCase()
  {
  c = auth::client::Instance();
  c->initRemoteHostData("localhost", "4242", "cacert.pem");
  }*/
  virtual void SetUp()
  {

    for (int i = 0; i < 10; i++){
      threads.push_back(std::thread([this](){
        auth::client* c;
        c = auth::client::Instance();
        c->initRemoteHostData("localhost", "4242", "cacert.pem");
        c->get_license();
        clients.push_back(c);
        std::string license = c->license();
        if (license.compare("-1") == 0){
          // donno what to test here yet
        }
        else
        {
          if (duplicates.find(license) == duplicates.end())
          {
            duplicates.insert(make_pair(license, 1));
          }
          else{
            auto it = duplicates.find(license);
            it->second += 1;
          }
        }
      }));
    }

  }

};

class TestEndurance : public ::testing::Test{
protected:
  std::vector<std::thread> threads;
  std::vector<auth::client*> clients;
 

};

TEST(TEST_EACH_ONE){
  std::vector<std::string> vec = { "alex", "vlad", "adi", "-1" };
  std::vector<auth::client*> clients;
  std::thread t([this, &vec, &clients](){
    for (int i = 0; i < 4; i++)
    {
      auth::client* c;
      c = auth::client::Instance();
      c->initRemoteHostData("localhost", "4242", "cacert.pem");
      c->get_license();
      clients.push_back(c);
      std::string license = c->license();
      EXPECT_EQ(license, vec[i]);
      std::cout << "license is " << license << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

  });
  t.join();
  for each(auto var in clients){
    var->destroy();
  }
  for each(auto var in clients){
    delete var;
  }

  //wait for sesssions to be destroyed before next test
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}


 

TEST_F(TestMultipleThreads, EachLicenseIsUsedOnce){
 
  for (auto it = std::begin(threads); it != std::end(threads); ++it){
    it->join();
  }
  for each (auto var in duplicates)
  {
    std::cout << var.first << ": " << var.second << std::endl;
    EXPECT_EQ(1, var.second);
  }
  EXPECT_EQ(3, duplicates.size());

  for each(auto var in clients){
    var->destroy();
  }
  for each(auto var in clients){
    delete var;
  }

  //wait for sesssions to be destroyed before next test
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

TEST_F(TestEndurance, Endurance){
  std::thread t([this]()
  {
    while (true)
    {
      for (int i = 0; i < 10; i++){
        threads.push_back(std::thread([this](){
          auth::client* c;
          c = auth::client::Instance();
          c->initRemoteHostData("localhost", "4242", "cacert.pem");
          c->get_license();
          clients.push_back(c);
          std::string license = c->license();
        }));
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
      for each(auto var in clients){
        var->destroy();
      }
      for each(auto var in clients){
        delete var;
      }
      threads.clear();
      clients.clear();

      //wait for sesssions to be destroyed before next test
     
    }
  });
  t.join();
}



int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}