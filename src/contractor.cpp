//
// Created by kirill on 26.02.24.
//

#include "contractor.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>
#include <string>

void Contractor::sign_up() const {
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(constants::db_source, &db);
  std::string request =
      (boost::format("INSERT INTO contractors ('username', 'email', 'password') VALUES ('%s', '%s', '%s')") % username % email % password).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  sqlite3_close(db);
}

void Contractor::approve_bid(const Project &project) {
}


void Contractor::add_project(const Project &project) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open(constants::db_source, &db);
  std::string find_request = (boost::format ("SELECT id from contractors"
                                             "WHERE username = %s") % username).str();

  rc = sqlite3_prepare_v2(db, find_request.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Not found");
  }
  int id = sqlite3_column_int(stmt, 0);

  std::string request =
      (boost::format("INSERT INTO projects ('name', 'description', 'contractor_id', 'completed) VALUES ('%s', '%s', '%d', %b)") % project.name % project.description % id % false).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  sqlite3_close(db);
}
void approve_bid(const Project& project);
void reject_bid(const Project& project);
void fire_worker(const Project& project);
void end_project(const Project& project);