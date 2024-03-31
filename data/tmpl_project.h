//
// Created by kirill on 31.03.24.
//

#ifndef FREELANCEPLATFORM_DATA_TMPL_PROJECT_H_
#define FREELANCEPLATFORM_DATA_TMPL_PROJECT_H_

#include <cppcms/view.h>
#include "../src/project.h"
#include "tmpl_master.h"
namespace Data {

struct singleProjectPage {
  Project project;
  Employee employee;
  bool is_employee = false;
  bool is_bid_created = false;

  singleProjectPage(){};
  ~singleProjectPage(){};
};

struct SingleProject : public Master {
  Data::singleProjectPage project_page;
};
}


#endif //FREELANCEPLATFORM_DATA_TMPL_PROJECT_H_
