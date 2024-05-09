//
// Created by kirill on 09.05.24.
//
#pragma once
#include <iostream>
#include "constants.h"
#include "db_pool.h"

class DataBase {
  std::string name;
 public:
  explicit DataBase(const std::string& name) : name(name) {};

  auto operator<<(const std::string& request) {
    std::string src = "dbname=";
    src += name;
    soci::session sql("sqlite3", src);

    return sql << request;
  }

  auto prepare(const std::string& request) {
    std::string src = "dbname=";
    src += name;
    soci::session sql("sqlite3", src);

    return sql.prepare << request;
  }
};
