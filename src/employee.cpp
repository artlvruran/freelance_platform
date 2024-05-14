//
// Created by kirill on 26.02.24.
//
#include "employee.h"
#include "sqlite3.h"
#include "constants.h"
#include "database.h"
#include <boost/format.hpp>

void Employee::sign_up() {
  DataBase db(db_source);
  db << "insert into users (username, email, password, role) values(:username, :email, :password, 'employee')", soci::use(*this);
}

bool Employee::log_in() {
  DataBase db(db_source);

  int cnt;
  db << "select count(*) from users where username == :username and password == :password and role == 'employee'", soci::use(*this), soci::into(cnt);
  return cnt == 1;
}

void Employee::create_bid(int project_id) {
  DataBase db(db_source);
  db << "insert into bids (project_id, employee_id, state) "
         "values(:project_id, :employee_id, 'considering')", soci::use(project_id), soci::use(id);
}