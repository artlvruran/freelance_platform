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
 private:
};

namespace soci {
template<> struct type_conversion<Bid> {
  typedef values base_type;

  static void from_base(values const& v, indicator ind, Bid& p) {
    if (ind == i_null) return;
    try {
      p.id = v.get<int>("id", 0);
      p.project_id = v.get<int>("project_id", 0);
      p.employee_id = v.get<int>("employee_id", 0);

      auto st = v.get<std::string>("state", {});

      if (st == "considering") {
        p.state = std::make_unique<BidStateConsidering>();
      } else if (st == "approved") {
        p.state = std::make_unique<BidStateApproved>();
      } else {
        p.state = std::make_unique<BidStateRejected>();
      }

    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
  }

  static void to_base(const Bid& p, values& v, indicator& ind) {
    try {
      v.set("id", p.id);
      v.set("project_id", p.project_id);
      v.set("employee_id", p.employee_id);
      v.set("state", p.state->str());

      ind = i_ok;
      return;
    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
    ind = i_null;
  }
};
}
