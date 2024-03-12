
#pragma once
#include <iostream>
#include <sqlite3.h>
#include "src/employee.h"
#include "src/contractor.h"
#include "src/project.h"
#include "src/constants.h"
#include <iostream>
#include <memory>


int main() {
  Project pr("kk");
  Contractor John("john", "r", "r");
  John.sign_up("john", "r", "r");
  John.id = 1;
  pr.contractor_id = 1;
  John.add_project(pr);
  pr.advance(event::hired);
}
