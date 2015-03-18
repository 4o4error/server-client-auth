#include "sqlOp.h"

bool SqlOp::openDatabase(std::string database_name)
{
  // Open Database
  try
  {
    rc = sqlite3_open(database_name.c_str(), &db); 

    if (rc){
      // failed
      fprintf(stderr, "ERROR: Can't open database: %s\n", sqlite3_errmsg(db));
      closeDatabase();
    }
  }
  catch (std::string e)
  {
      std::cout << "An exception occurred. Exception : " << e.c_str() << '\n';
        return false;
   }

}

bool SqlOp::setLicence(std::string table_name, std::string licence, std::string user_name){

  std::string str = "UPDATE " + table_name + " SET usedBy = '"+user_name+"'  WHERE license like '" + licence + "';";
  char * ch = new char[str.length() + 1];
  std::strcpy(ch, str.c_str());
  query(ch);
  str = "UPDATE " + table_name + " SET inUse = '1' WHERE license like '"+licence+"';";
  std::strcpy(ch, str.c_str());
  query(ch);
  delete[] ch;

  return true;
}

bool SqlOp::resetLicence(std::string table_name, std::string licence){
  
  std::string str = "UPDATE " + table_name + " SET usedBy = '' WHERE license like '" + licence + "';";
  char * ch = new char[str.length() + 1];
  std::strcpy(ch, str.c_str());
  query(ch);
  str = "UPDATE " + table_name + " SET inUse = '0' WHERE license like '" + licence + "';";
  std::strcpy(ch, str.c_str());
  query(ch);
  delete[] ch;

  return true;
}



bool SqlOp::createTable(std::string table_name, std::string columns)
  {
  try
  {
    std::string str = "CREATE TABLE " + table_name + "(" + columns + ");";
    char * ch = new char[str.length() + 1];
    std::strcpy(ch, str.c_str());
    query(ch);
    delete[] ch;
    
  }
  catch (std::string e)
  {
    std::cout << "An exception occurred. Exception : " << e.c_str() << '\n';
    return false;
  }
  return true;
  }
bool SqlOp::insertValue(std::string table_name, std::string values)
  {

    try
    {
      std::string str = "INSERT INTO " + table_name + " VALUES(NULL, " + values + " );";
      char * ch = new char[str.length() + 1];
      std::strcpy(ch, str.c_str());
      query(ch);
      delete[] ch;

    }
    catch (std::string e)
    {
      std::cout << "An exception occurred. Exception : " << e.c_str() << '\n';
      return false;
    }
    return true;
  
  }


bool SqlOp::displayTable(std::string table_name)
  {
    // Display MyTable
  std::cout << "Retrieving values  ..." << table_name.c_str() << std::endl;
  std::string query = "SELECT * FROM " + table_name + ";";
    const char *sqlSelect = query.c_str();
    char **results = NULL;
    int rows, columns;
    sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
    if (rc)
    {
      std::cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << std::endl << std::endl;
      sqlite3_free(error);
      return false;
    }
    else
    {
      // Display Table
      for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
      {
        for (int colCtr = 0; colCtr < columns; ++colCtr)
        {
          // Determine Cell Position
          int cellPosition = (rowCtr * columns) + colCtr;

          // Display Cell Value
          std::cout.width(12);
          std::cout.setf(std::ios::left);
          std::cout << results[cellPosition] << " ";
        }

        // End Line
        std::cout << std::endl;

        // Display Separator For Header
        if (0 == rowCtr)
        {
          for (int colCtr = 0; colCtr < columns; ++colCtr)
          {
            std::cout.width(12);
            std::cout.setf(std::ios::left);
            std::cout << "~~~~~~~~~~~~ ";
          }
          std::cout << std::endl;
        }
      }
    }
    sqlite3_free_table(results);
    return true;
  }
bool SqlOp::closeDatabase()
  {
    // Close Database
  sqlite3_free(error);
  sqlite3_close(db);
  sqlite3_free(db);
  return true;
  }
std::vector<std::string> SqlOp::query(char* query)
{
  sqlite3_stmt *statement;
  std::vector<std::vector<std::string> > results;
  if (sqlite3_prepare_v2(db, query, -1, &statement, 0) == SQLITE_OK)
  {
    int cols = sqlite3_column_count(statement);
    int result = 0;
    while (true)
    {
      result = sqlite3_step(statement);
      if (result == SQLITE_ROW)
      {
        std::vector<std::string> values;
        for (int col = 0; col < cols; col++)
        {
          values.push_back((char*)sqlite3_column_text(statement, col));
        }
        results.push_back(values);
      }
      else
      {
        break;
      }
    }
    sqlite3_step(statement);
    sqlite3_finalize(statement);
  }

  return results[0];
}

std::string  SqlOp::getUnusedLicences(std::string user_name){

  std::vector<std::string> results = query("SELECT * FROM licenses WHERE inUse LIKE '0' LIMIT 1 ");
  if (results.empty()){
    std::cout << "there are no unused licences";
    return "-1";
  }
  else{
    setLicence("licenses", results[1].c_str(), user_name);
    return results[1];
  }
  
}

void SqlOp::createTablesAndDatabase(){

  createTable("licenses", " id INTEGER PRIMARY KEY AUTOINCREMENT, license UNIQUE, inUse INTEGER , usedBy");
  insertValue("licenses", "'alex','0','0'");
  insertValue("licenses", "'vlad','0','1'");
  insertValue("licenses", "'adi','0','2'");
}



