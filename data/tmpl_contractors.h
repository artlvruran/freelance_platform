//
// Created by kirill on 01.04.24.
//
#pragma once

#include <cppcms/view.h>
#include "../src/contractor.h"
#include "tmpl_master.h"

namespace Data {

struct contractorsPage {
  std::vector<Contractor> contractors;
  contractorsPage(){};
  ~contractorsPage(){};
};

struct Contractors : public Master {
  Data::contractorsPage contractors_page;
};
}

