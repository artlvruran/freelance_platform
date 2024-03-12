//
// Created by kirill on 26.02.24.
//

#pragma once
#include "project.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>


void Project::advance(event e) {
  auto new_status = state->on_event(e);
  if (new_status == nullptr) {
    throw std::runtime_error("event incorrect");
  }
  state = std::move(new_status);

  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "update projects "
         "set state = :state "
         "where id = :id", soci::use(*this);
  sql.close();
}