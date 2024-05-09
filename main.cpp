#include "src/employee.h"
#include "src/contractor.h"
#include "src/project.h"
#include "src/constants.h"
#include "src/database.h"
#include <string>


int main() {
  Contractor john;
  Employee hans;
  auto src = db_source;
  DataBase db(src);
  db << "select * from users where username == :username", soci::into(john), soci::use(std::string("john"));
  db << "select * from users where username == :username", soci::into(hans), soci::use(std::string("hans"));
  john.register_observer(hans);
  Project pr;
  pr.name = "tt";
  john.add_project(pr);
  john.notify_observers("hello");
}
