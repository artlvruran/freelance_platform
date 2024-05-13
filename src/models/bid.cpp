//
// Created by kirill on 11.03.24.
//
#include "bid.h"
#include "constants.h"
#include "../database.h"
#include <iostream>
#include <boost/format.hpp>

void Bid::advance(bid_event e) {
  auto new_status = state->on_event(e);
  if (new_status == nullptr) {
    throw std::runtime_error("event incorrect");
  }
  state = std::move(new_status);

  DataBase db(db_source);
  db << "update bids "
         "set state = :state "
         "where id = :id", soci::use(*this);
}