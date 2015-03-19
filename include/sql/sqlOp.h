#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <ios>
#include <iostream>
#include <cstring>
#include "sqlite3.h"
#include <mutex>
#include <vector>

//build opts
#include "sca_platformtype.h"
class SCA_EXPORT_SYMBOL SqlOp{
  friend class Singleton;
private:
  int unique;
  int rc;
  char *error;
  sqlite3 *db;
public:
  SqlOp(){};
  ~SqlOp(){ };
public:
  std::mutex thr_mutex;
  std::vector<std::string> query(std::string query);
  bool openDatabase(std::string databse_name);
  bool createTable(std::string table_name, std::string columns);
  bool insertValue(std::string table_name, std::string values);
  bool setLicence(std::string table_name, std::string licence, std::string user_name);
  bool resetLicence(std::string table_name, std::string licence);
  bool displayTable(std::string table_name);
  std::string getUnusedLicences(std::string  user_name);
  void createTablesAndDatabase();
  bool closeDatabase();
};

