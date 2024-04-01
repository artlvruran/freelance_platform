//
// Created by kirill on 01.04.24.
//

#ifndef FREELANCEPLATFORM_DATA_TMPL_USER_H_
#define FREELANCEPLATFORM_DATA_TMPL_USER_H_

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
};
}


#endif //FREELANCEPLATFORM_DATA_TMPL_USER_H_
