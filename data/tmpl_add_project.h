//
// Created by kirill on 31.03.24.
//

#pragma once
#include <cppcms/view.h>
#include <cppcms/form.h>
#include "tmpl_master.h"

namespace Data {
struct addProjectForm : public cppcms::form {
  cppcms::widgets::text name;
  cppcms::widgets::text description;
  cppcms::widgets::submit submit;

  addProjectForm() {
    name.message("Project Name");
    submit.value("Submit");

    add(name);
    add(submit);

    name.non_empty();
  }
};

struct AddProject : public Master {
  Data::addProjectForm add_project_form;
};
}

