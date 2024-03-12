
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
  Contractor ervin("john", "r", "r");
  ervin.id = 2;
  Bid bid;
  bid.id = 1;
  bid.employee_id = 3;
  bid.project_id = 1;
  ervin.consider_bid(bid, bid_event::approve);
}
