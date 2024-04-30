//
// Created by kirill on 29.04.24.
//


#pragma once

#include <cppcms/view.h>
#include "../src/employee.h"
#include "tmpl_master.h"

namespace Data {

struct employeesPage {
  std::vector<Employee> employees;
  employeesPage(){};
  ~employeesPage(){};
};

struct Employees : public Master {
  Data::employeesPage employees_page;
};
}