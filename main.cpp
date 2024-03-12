
#pragma once
#include <iostream>
#include <sqlite3.h>
#include "src/employee.h"
#include "src/contractor.h"
#include "src/project.h"
#include "src/constants.h"
#include <iostream>
#include <memory>
#include <string>


int main() {
  Contractor john;
  Employee hans;
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "select * from users where username == :username", soci::into(john), soci::use(std::string("john"));
  sql << "select * from users where username == :username", soci::into(hans), soci::use(std::string("hans"));

  john.remove_observer(hans);
}
