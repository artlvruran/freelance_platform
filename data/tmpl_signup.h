//
// Created by kirill on 30.03.24.
//

#pragma once

#include <cppcms/view.h>
#include <cppcms/form.h>
#include "tmpl_master.h"

namespace Data {
  struct infoForm : public cppcms::form {
    cppcms::widgets::text username;
    cppcms::widgets::password password;
    cppcms::widgets::select role;
    cppcms::widgets::submit submit;

    infoForm() {
      username.message("Your nickname");
      password.message("Your password");
      role.message("Your role");
      submit.value("Submit");

      add(username);
      add(password);
      add(role);
      add(submit);

      username.non_empty();
      password.non_empty();
      role.add("employee");
      role.add("contractor");
      role.non_empty();
    }
  };

  struct signupForm : public cppcms::form {
    cppcms::widgets::text username;
    cppcms::widgets::password password;
    cppcms::widgets::select role;
    cppcms::widgets::submit submit;
    cppcms::widgets::email email;
    cppcms::widgets::text fullname;

    signupForm(){
      username.message("Your nickname");
      fullname.message("Your full name");
      email.message("Your email");
      password.message("Your password");
      role.message("Your role");
      submit.value("Submit");

      add(username);
      add(fullname);
      add(email);
      add(password);
      add(role);
      add(submit);

      username.non_empty();
      password.non_empty();
      role.add("employee");
      role.add("contractor");
      role.non_empty();
      fullname.non_empty();
      email.non_empty();
    }

  };

  struct Signup : public Master {
    Data::signupForm info;
  };

  struct Login : public Master {
    Data::infoForm info;
  };
}
