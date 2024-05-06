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
  static void from_base(values const& values, indicator ind, Message &message) {
    if (ind == i_null) return;
    try {
      message.id = values.get<int>("id", 0);
      message.sender_id = values.get<int>("sender_id", 0);
      message.address_id = values.get<int>("address_id", 0);
      message.text = values.get<std::string>("text", {});

    } catch (std::exception const& exception) { std::cerr << exception.what() << std::endl; }
  }

  static void to_base(const Message &message, values &values, indicator &ind) {
    try {
      values.set("id", message.id);
      values.set("sender_id", message.sender_id);
      values.set("address_id", message.address_id);
      values.set("text", message.text);

      ind = i_ok;
      return;
    } catch (std::exception const& exception) { std::cerr << exception.what() << std::endl; }
    ind = i_null;
  }
};
}
