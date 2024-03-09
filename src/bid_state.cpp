//
// Created by kirill on 08.03.24.
//
#include "bid_state.h"


std::unique_ptr<BidState> BidStateConsidering::on_event(bid_event e) {
  if (e == bid_event::approve) {
    return std::make_unique<BidStateApproved>();
  }
  return std::make_unique<BidStateRejected>();
}

std::unique_ptr<BidState> BidStateApproved::on_event(bid_event e) {
  if (e == bid_event::reject) {
    return std::make_unique<BidStateRejected>();
  }
  return nullptr;
}

std::unique_ptr<BidState> BidStateRejected::on_event(bid_event e) {
  return nullptr;
}
