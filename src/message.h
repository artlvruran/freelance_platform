//
// Created by kirill on 30.04.24.
//

#pragma once
#include <string>
#include <memory>
#include <utility>
#include "db_pool.h"

class Message {
 public:
  int id;
  int sender_id;
  int address_id;
  std::string text;
};

namespace soci {

template<>
struct type_conversion<Message> {
  typedef values base_type;
  static void from_base(values const &v, indicator ind, Message &p) {
    if (ind == i_null) return;
    try {
      p.id = v.get<int>("id", 0);
      p.sender_id = v.get<int>("sender_id", 0);
      p.address_id = v.get<int>("address_id", 0);
      p.text = v.get<std::string>("text", {});

    } catch (std::exception const &e) { std::cerr << e.what() << std::endl; }
  }

  static void to_base(const Message &p, values &v, indicator &ind) {
    try {
      v.set("id", p.id);
      v.set("sender_id", p.sender_id);
      v.set("address_id", p.address_id);
      v.set("text", p.text);

      ind = i_ok;
      return;
    } catch (std::exception const &e) { std::cerr << e.what() << std::endl; }
    ind = i_null;
  }
};
}
