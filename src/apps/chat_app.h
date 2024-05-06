//
// Created by kirill on 06.05.24.
//
#pragma once

#include <cppcms/applications_pool.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/application.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include <cppcms/application.h>
#include <cppcms/http_file.h>

#include "../../data/tmpl_master.h"
#include "../../data/tmpl_signup.h"
#include "../../data/tmpl_projects.h"
#include "../../data/tmpl_project.h"
#include "../../data/tmpl_add_project.h"
#include "../../data/tmpl_contractors.h"
#include "../../data/tmpl_user.h"
#include "../../data/tmpl_profile_edit.h"
#include "../../data/tmpl_notifications.h"
#include "../../data/tmpl_employees.h"
#include "../../data/tmpl_main.h"
#include "../../data/tmpl_send_message.h"

#include "../utils/utils.h"

class Chat : public cppcms::application {
 public:
  Chat(cppcms::service &srv) : cppcms::application(srv) {
    dispatcher().assign("/(.+)/(.+)", &Chat::chat, this, 1, 2);
    mapper().assign("/{1}/{2}");
  }

  virtual void chat(std::string from_id, std::string to_id) {
    Data::ChatView view;
    add_menu(view, *this);
    if (!session().is_set("username")) {
      response().set_redirect_header("/");
      return;
    }
    auto current_user = get_user(*this);
    if (current_user.id != std::stoi(from_id)) {
      response().set_redirect_header("/");
      return;
    }

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);

    if (request().request_method() == "POST") {
      view.chat_form.load(context());
      if (view.chat_form.validate()) {
        auto message = view.chat_form.message.value();
        sql << "insert into messages (sender_id, address_id, text) values(:sender_id, :address_id, :text)",
            soci::use(current_user.id), soci::use(to_id), soci::use(message);
      }
    }

    soci::rowset<Message> my_messages = (sql.prepare << "select * from messages where sender_id=:sender_id and address_id=:address_id",
        soci::use(from_id), soci::use(to_id));
    soci::rowset<Message> their_messages = (sql.prepare << "select * from messages where sender_id=:sender_id and address_id=:address_id",
        soci::use(to_id), soci::use(from_id));

    for (auto& my_message : my_messages) {
      view.messages.emplace_back(0, my_message);
    }
    for (auto& their_message : their_messages) {
      view.messages.emplace_back(1, their_message);
    }

    auto comparator = [](const std::pair<bool, Message>& first, const std::pair<bool, Message>& second) {
      return first.second.id < second.second.id;
    };

    std::sort(view.messages.begin(), view.messages.end(), comparator);
    render("ChatView", view);
  }
};