//
// Created by kirill on 12.03.24.
//
#include "user.h"
#include <soci/soci.h>
#include "constants.h"

void User::notify(int contractor_id) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);

  sql << "insert into notifications (contractor_id, user_id, is_read)"
         "  values(:contractor_id, :user_id, false)", soci::use(contractor_id), soci::use(id);
}
//TODO: test