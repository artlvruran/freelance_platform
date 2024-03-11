//
// Created by kirill on 08.03.24.
//

#ifndef FREELANCEPLATFORM_SRC_BID_H_
#define FREELANCEPLATFORM_SRC_BID_H_

#include "bid_state.h"
#include "db_pool.h"

class Bid {
 public:
  int id;

  int project_id;

  int employee_id;
  std::unique_ptr<BidState> status = std::make_unique<BidStateConsidering>();
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
    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
  }

  static void to_base(const Bid& p, values& v, indicator& ind) {
    try {
      v.set("id", p.id);
      v.set("project_id", p.project_id);
      v.set("employee_id", p.employee_id);

      ind = i_ok;
      return;
    } catch (std::exception const & e) { std::cerr << e.what() << std::endl; }
    ind = i_null;
  }
};
}

#endif //FREELANCEPLATFORM_SRC_BID_H_
