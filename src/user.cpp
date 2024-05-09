//
// Created by kirill on 12.03.24.
//
#include "user.h"
#include <soci/soci.h>
#include "constants.h"
#include "database.h"
#include "chrono"

void User::notify(int contractor_id) {
  DataBase db(db_source);
  db << "insert into notifications (contractor_id, user_id, is_read)"
         "  values(:contractor_id, :user_id, false)", soci::use(contractor_id), soci::use(id);
}
