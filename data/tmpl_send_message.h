//
// Created by kirill on 30.04.24.
//

#pragma once
#include <cppcms/view.h>
#include <cppcms/form.h>
#include "tmpl_master.h"
#include "../src/message.h"

using namespace cppcms;

namespace Data {
struct sendMessageForm : public cppcms::form {
  cppcms::widgets::text message;
  cppcms::widgets::submit submit;

  sendMessageForm() {
    message.attributes_string("placeholder=\"Message...\"");

    add(message);
    add(submit);

    message.non_empty();
  }
};

struct ChatView : public Master {
  Data::sendMessageForm chat_form;
  std::vector<std::pair<bool, Message>> messages;
};
}

