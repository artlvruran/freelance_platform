//
// Created by kirill on 08.03.24.
//

#ifndef FREELANCEPLATFORM_SRC_BID_STATE_H_
#define FREELANCEPLATFORM_SRC_BID_STATE_H_

#include <memory>

enum bid_event {
  approve,
  reject
};

struct BidState {
  virtual std::unique_ptr<BidState> on_event(bid_event e) = 0;
};

struct BidStateConsidering : BidState {
  using BidState::BidState;
  std::unique_ptr<BidState> on_event(bid_event e) override;
};

struct BidStateApproved : BidState {
  using BidState::BidState;
  std::unique_ptr<BidState> on_event(bid_event e) override;
};

struct BidStateRejected : BidState {
  using BidState::BidState;
  std::unique_ptr<BidState> on_event(bid_event e) override;
};

#endif //FREELANCEPLATFORM_SRC_BID_STATE_H_
