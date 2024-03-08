#include <iostream>
#include <sqlite3.h>
#include "src/employee.h"
#include "src/contractor.h"
#include "src/project.h"

int main() {
  Employee employee("test", "password", "test@test.com", "software engineering", "Russia", "Moscow");
  employee.sign_up();
  Contractor contractor("boss", "boss_password", "boss@boss.com", "software engineering", "Russia", "Moscow");
  contractor.add_project(pr);
}
