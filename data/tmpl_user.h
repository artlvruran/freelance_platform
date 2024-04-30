//
// Created by kirill on 01.04.24.
//

#pragma once

#include <cppcms/view.h>
#include "../src/project.h"
#include "../src/bid.h"
#include "tmpl_master.h"
namespace Data {

struct singleUserPage {
  User user;
  bool is_subscribed = false;

  singleUserPage(){};
  ~singleUserPage(){};


};

struct SingleUser : public Master {
  Data::singleUserPage user_page;
  bool is_me = false;
  double my_rate = 0;
};
}
