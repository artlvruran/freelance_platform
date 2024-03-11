//
// Created by kirill on 08.03.24.
//

#ifndef FREELANCEPLATFORM_SRC_BID_H_
#define FREELANCEPLATFORM_SRC_BID_H_

#include "bid_state.h"

struct Bid {
  int bid_id;
  int project_id;
  int employee_id;
  std::unique_ptr<BidState> status = std::make_unique<BidStateConsidering>();
  void advance(bid_event e);
};

#endif //FREELANCEPLATFORM_SRC_BID_H_
