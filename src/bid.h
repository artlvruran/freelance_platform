//
// Created by kirill on 08.03.24.
//

#pragma once

#include "bid_state.h"
#include "db_pool.h"

class Bid {
 public:
  int id;

  int project_id;

  int employee_id;
  std::unique_ptr<BidState> state = std::make_unique<BidStateConsidering>();
  void advance(bid_event e);

  Bid() = default;

  Bid(int id, int project_id, int employee_id) : id(id),
                                                 project_id(project_id),
                                                 employee_id(employee_id) {};

  Bid(const Bid& other) : id(other.id),
                          project_id(other.project_id),
                          employee_id(other.employee_id) {
    if (other.state->str() == "considering") {
      state = std::make_unique<BidStateConsidering>();
    } else if (other.state->str() == "approved") {
      state = std::make_unique<BidStateApproved>();
    } else {
      state = std::make_unique<BidStateRejected>();
    }
  }
 private:
};

namespace soci {
template<> struct type_conversion<Bid> {
  typedef values base_type;

  static void from_base(values const& values, indicator ind, Bid& bid) {
    if (ind == i_null) return;
    try {
      bid.id = values.get<int>("id", 0);
      bid.project_id = values.get<int>("project_id", 0);
      bid.employee_id = values.get<int>("employee_id", 0);

      auto st = values.get<std::string>("state", {});

      if (st == "considering") {
        bid.state = std::make_unique<BidStateConsidering>();
      } else if (st == "approved") {
        bid.state = std::make_unique<BidStateApproved>();
      } else {
        bid.state = std::make_unique<BidStateRejected>();
      }

    } catch (std::exception const& exception) {
      std::cerr << exception.what() << std::endl;
    }
  }

  static void to_base(const Bid& bid, values& values, indicator& ind) {
    try {
      values.set("id", bid.id);
      values.set("project_id", bid.project_id);
      values.set("employee_id", bid.employee_id);
      values.set("state", bid.state->str());

      ind = i_ok;
      return;
    } catch (std::exception const& exception) {
      std::cerr << exception.what() << std::endl;
    }
    ind = i_null;
  }
};
}
