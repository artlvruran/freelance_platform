//
// Created by kirill on 26.02.24.
//

#include "project.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>


void Project::advance(event e) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open(constants::db_source, &db);
  int number = (e == event::start ? 0 : (e == event::hired ? 1 : 2));

  std::string find_request = (boost::format ("SELECT id from projects"
                                             "WHERE name = %s") % name).str();

  rc = sqlite3_prepare_v2(db, find_request.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Not found");
  }
  int id = sqlite3_column_int(stmt, 0);

  std::string request =
      (boost::format("UPDATE projects"
                     "SET state = %d"
                     "WHERE id = %d") % number % id).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  sqlite3_close(db);
  status = status->on_event(e);
}

/*
  std::string name;
  std::string details;
  std::string timeline;
  double budget;
  bool completed = false;
  std::vector<Employee> bids;
  std::vector<Employee> participants;

*/