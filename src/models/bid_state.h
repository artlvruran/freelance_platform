//
// Created by kirill on 08.03.24.
//

#pragma once

#include <memory>

enum bid_event {
  approve,
  reject
};

struct BidState {
  virtual std::unique_ptr<BidState> on_event(bid_event e) = 0;
  virtual std::string str() const = 0;
};

struct BidStateConsidering : BidState {
  using BidState::BidState;
  std::unique_ptr<BidState> on_event(bid_event e) override;
  std::string str() const override;
};

struct BidStateApproved : BidState {
  using BidState::BidState;
  std::unique_ptr<BidState> on_event(bid_event e) override;
  std::string str() const override;
};

struct BidStateRejected : BidState {
  using BidState::BidState;
  std::unique_ptr<BidState> on_event(bid_event e) override;
  std::string str() const override;
};
