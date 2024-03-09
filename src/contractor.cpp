//
// Created by kirill on 26.02.24.
//

#pragma once
#include "contractor.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>
#include <string>

void Contractor::sign_up() const {
  sqlite3 *db;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string request =
      (boost::format("INSERT INTO contractors ('username', 'email', 'password') VALUES ('%s', '%s', '%s')") % username % email % password).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  sqlite3_close(db);
}

void Contractor::consider_bid(Bid& bid, bid_event e) {
  auto new_status = bid.status->on_event(e);
  if (new_status != nullptr) {
    bid.status = std::move(new_status);
  }
// Добавить проверку на то что заказчик курирует именно этот проект
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string status = (e == bid_event::approve ? "approved" : "rejected");

  std::string find_request = (boost::format ("SELECT id from bids"
                                             "WHERE id = %d") % bid.bid_id).str();

  rc = sqlite3_prepare_v2(db, find_request.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Not found");
  }
  int id = sqlite3_column_int(stmt, 0);

  std::string request =
      (boost::format("UPDATE bids"
                     "SET status = %s"
                     "WHERE id = %d") % status.c_str() % id).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  sqlite3_close(db);
}

void Contractor::add_project(const Project &project) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string find_request = (boost::format ("SELECT id from contractors WHERE username = '%s'") % username).str();

  rc = sqlite3_prepare_v2(db, find_request.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Not found");
  }
  int id = sqlite3_column_int(stmt, 0);

  std::string request =
      (boost::format("INSERT INTO projects ('name', 'description', 'contractor_id', 'state') VALUES ('%s', '%s', '%d', '%d')") % project.name % project.description % id % 0).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Error in insertion project in db");
  }
  sqlite3_close(db);
}

void fire_worker(const Project& project, const Employee& employee) {
  //deleting from table bids
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open(db_source, &db);

  std::string find_request = (boost::format ("SELECT id from employees WHERE username = '%s'") % employee.username).str();
  rc = sqlite3_prepare_v2(db, find_request.c_str(), -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Employee not found");
  }
  int employee_id = sqlite3_column_int(stmt, 0);

  std::string request =
      (boost::format("DELETE FROM bids WHERE employee_id = %d AND project_id = %d") % employee_id % project.id).str();
}

void end_project(Project& project) {
  project.advance(event::completed);
}

void end_project_hiring(Project& project) {
  project.advance(event::hired);
}