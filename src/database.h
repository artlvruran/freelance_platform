//
// Created by kirill on 09.05.24.
//
#pragma once
#include <iostream>

class DataBase {
  soci::session sql;
 public:
  explicit DataBase(const std::string& name) : sql("sqlite3", "dbname=" + name) {};

  auto operator<<(const std::string& request) {
    return sql << request;
  }

  auto prepare(const std::string& request) {
    return sql.prepare << request;
  }
};
