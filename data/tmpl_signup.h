//
// Created by kirill on 30.03.24.
//

#ifndef FREELANCEPLATFORM_DATA_TMPL_SIGNUP_H_
#define FREELANCEPLATFORM_DATA_TMPL_SIGNUP_H_


#include <cppcms/view.h>
#include <cppcms/form.h>
#include "tmpl_master.h"

namespace Data {
  struct infoForm : public cppcms::form {
    cppcms::widgets::text username;                        // титул страницы
    cppcms::widgets::text email;                   // описание страницы
    cppcms::widgets::text password;

    infoForm() {
      username.message("your nickname");
      email.message("your email");
      password.message("your password");

      add(username);
      add(email);
      add(password);
      username.non_empty();
      email.non_empty();
      password.non_empty();
    }
  };

  struct Signup : public Master {
    std::string username, email, password;
    Data::infoForm info;
  };
}

#endif //FREELANCEPLATFORM_DATA_TMPL_SIGNUP_H_
