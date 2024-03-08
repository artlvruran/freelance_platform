
#pragma once
#include <iostream>
#include <sqlite3.h>
#include "src/employee.h"
#include "src/contractor.h"
#include "src/project.h"
#include "src/constants.h"

int main() {
  Employee employee("test", "password", "test@test.com", "software engineering", "Russia", "Moscow");
  employee.sign_up();
  Contractor contractor("boss", "boss_password", "boss@boss.com", "software engineering", "Russia", "Moscow");
  contractor.sign_up();
  Task task("pr", "desc", "12-27/05/26", 987.89);
  contractor.add_project(task);
}
