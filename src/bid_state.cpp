//
// Created by kirill on 08.03.24.
//
#include "bid_state.h"
#include <string>


std::unique_ptr<BidState> BidStateConsidering::on_event(bid_event e) {
  if (e == bid_event::approve) {
    return std::make_unique<BidStateApproved>();
  }
  return std::make_unique<BidStateRejected>();
}

std::string BidStateConsidering::str() const {
  return "considering";
}

std::unique_ptr<BidState> BidStateApproved::on_event(bid_event e) {
  if (e == bid_event::reject) {
    return std::make_unique<BidStateRejected>();
  }
  return nullptr;
}

std::string BidStateApproved::str() const {
  return "approved";
}

std::unique_ptr<BidState> BidStateRejected::on_event(bid_event e) {
  return nullptr;
}
std::string BidStateRejected::str() const {
  return "rejected";
}
