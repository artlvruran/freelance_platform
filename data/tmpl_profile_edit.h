//
// Created by kirill on 27.04.24.
//

#pragma once

#include <cppcms/view.h>
#include <cppcms/form.h>
#include "tmpl_master.h"

namespace Data {
  struct profileEditForm : public cppcms::form {
    cppcms::widgets::file avatar_file;
    cppcms::widgets::file cover_file;
    cppcms::widgets::submit submit;

    profileEditForm() {
      avatar_file.message("Choose avatar");
      avatar_file.filename(booster::regex(".*\\.(jpg|jpeg|png)"));
      avatar_file.mime(booster::regex("image/(png|jpeg)"));
      avatar_file.add_valid_magic("\x89\x50\x4e\x47\x0D\x0A\x1A\x0A");
      avatar_file.add_valid_magic("\xff\xd8");

      add(avatar_file);

      cover_file.message("Choose cover");
      cover_file.filename(booster::regex(".*\\.(jpg|jpeg|png)"));
      cover_file.mime(booster::regex("image/(png|jpeg)"));
      cover_file.add_valid_magic("\x89\x50\x4e\x47\x0D\x0A\x1A\x0A");
      cover_file.add_valid_magic("\xff\xd8");

      add(cover_file);

      submit.value("Change");
      add(submit);
    }
  };

  struct ProfileEdit : public Master {
    bool is_me = false;
    Data::profileEditForm edit_form;
  };
}