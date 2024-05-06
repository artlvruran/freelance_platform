//
// Created by kirill on 26.02.24.
//
#include "employee.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>

void Employee::sign_up() {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "insert into users (username, email, password, role) values(:username, :email, :password, 'employee')", soci::use(*this);
}

bool Employee::log_in() {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);

  int cnt;
  sql << "select count(*) from users where username == :username and password == :password and role == 'employee'", soci::use(*this), soci::into(cnt);
  return cnt == 1;
}

Bid Employee::create_bid(int project_id) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);

  sql << "insert into bids (project_id, employee_id, state) "
         "values(:project_id, :employee_id, 'considering')", soci::use(project_id), soci::use(id);
  int bid_id;
  sql << "select count(*) from bids ", soci::into(bid_id);
  Bid bid{bid_id, id, project_id};
  return bid;
}