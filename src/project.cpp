//
// Created by kirill on 26.02.24.
//

#include "project.h"
#include "sqlite3.h"
#include "constants.h"
#include "database.h"
#include <boost/format.hpp>


void Project::advance(event e) {
  auto new_status = state->on_event(e);
  if (new_status == nullptr) {
    throw std::runtime_error("event incorrect");
  }
  state = std::move(new_status);

  DataBase db(db_source);
  db << "update projects "
         "set state = :state "
         "where id = :id", soci::use(*this);
}