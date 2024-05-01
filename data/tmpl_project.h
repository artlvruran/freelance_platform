//
// Created by kirill on 31.03.24.
//


#pragma once

#include <cppcms/view.h>
#include "../src/project.h"
#include "../src/bid.h"
#include "tmpl_master.h"
namespace Data {

struct singleProjectPage {
  Project project;
  Employee employee;
  bool has_right = false;
  bool is_employee = false;
  bool is_bid_created = false;
  std::vector<std::pair<Bid, Employee>> bids;

  singleProjectPage(){};
  ~singleProjectPage(){};
};

struct SingleProject : public Master {
  Data::singleProjectPage project_page;
};
}

