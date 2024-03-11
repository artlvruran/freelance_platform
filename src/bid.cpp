//
// Created by kirill on 11.03.24.
//
#include "sqlite3.h"
#include "bid.h"
#include "constants.h"
#include <iostream>
#include <boost/format.hpp>

void Bid::advance(bid_event e) {
  auto new_status = status->on_event(e);
  if (new_status == nullptr) {
    throw std::runtime_error("event incorrect");
  }
  status = std::move(new_status);

  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open(db_source, &db);
  std::string st = (e == bid_event::approve ? "approved" : "rejected");

  std::string request =
      (boost::format("UPDATE bids"
                     "SET state = %d"
                     "WHERE id = %d") % st % bid_id).str();
  rc = sqlite3_exec(db, request.c_str(), 0, 0, 0);
  sqlite3_close(db);
}