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
      (boost::format("SELECT project_id FROM bids WHERE id == %d") % bid.id).str();
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
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "insert into projects (name, contractor_id, state)"
         "values(:name, :contractor_id, :state)", soci::use(project);
  sql << "select id from projects "
         "where name == :name", soci::use(project.name), soci::into(project.id);
  project.advance(event::start);
}

void Contractor::fire_worker(const Project& project, const Employee& employee) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "delete from bids "
         "where employee_id == :id and project_id == :id1", soci::use(employee.id), soci::use(project.id);
}

void end_project(Project& project) {
  project.advance(event::completed);
}

void end_project_hiring(Project& project) {
  project.advance(event::hired);
}