//
// Created by kirill on 26.02.24.
//

#pragma once
#include "contractor.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>
#include <string>

void Contractor::sign_up(const std::string& username,
                         const std::string& email,
                         const std::string& password) const {
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string request =
      (boost::format("INSERT INTO contractors ('username', 'email', 'password') VALUES ('%s', '%s', '%s')") % username % email % password).str();
  sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr);
  sqlite3_close(db);
}

bool Contractor::log_in(const std::string& username,
                        const std::string& email,
                        const std::string& password) const {
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string request =
      (boost::format("SELECT id FROM contractors WHERE username == '%s' AND email == '%s' AND password == '%s')") % username % email % password).str();
  rc = sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr);
  sqlite3_close(db);
  return rc == SQLITE_OK;
}

void Contractor::consider_bid(Bid& bid, bid_event e) {
  sqlite3_stmt *stmt;
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(db_source, &db);

  std::string request =
      (boost::format("SELECT project_id FROM bids WHERE id == %d") % bid.bid_id).str();
  rc = sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
  sqlite3_close(db);

  int project_id = sqlite3_column_int(stmt, 0);

  request =
      (boost::format("SELECT contractor_id FROM projects WHERE id == %d") % project_id).str();
  rc = sqlite3_prepare_v2(db, request.c_str(), -1, &stmt, nullptr);
  sqlite3_close(db);

  int contractor_id = sqlite3_column_int(stmt, 0);

  if (contractor_id != id) {
    throw std::runtime_error("Error in responsibility of contractor");
  }

  bid.advance(e);
}

void Contractor::add_project(Project &project) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open(db_source, &db);

  std::string request =
      (boost::format("INSERT INTO projects ('name', 'contractor_id', 'state') VALUES ('%s', '%d', '%d')") % project.name % id % 0).str();
  rc = sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error in insertion project in db");
  }
  sqlite3_close(db);

  project.advance(event::start);
}

void fire_worker(const Project& project, const Employee& employee) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open(db_source, &db);

  int employee_id = employee.id;

  std::string request =
      (boost::format("DELETE FROM bids WHERE employee_id = %d AND project_id = %d") % employee_id % project.id).str();
  rc = sqlite3_exec(db, request.c_str(), nullptr, nullptr, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error in deleting employee");
  }
  sqlite3_close(db);
}

void end_project(Project& project) {
  project.advance(event::completed);
}

void end_project_hiring(Project& project) {
  project.advance(event::hired);
}