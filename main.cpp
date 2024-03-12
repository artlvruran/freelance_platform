
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
  Employee Hans("hans", "hans@mail.com", "h218dhei9ss");
  Hans.id = 3;
  Hans.create_bid(1);
}
