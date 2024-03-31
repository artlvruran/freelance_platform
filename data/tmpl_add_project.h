//
// Created by kirill on 31.03.24.
//

#ifndef FREELANCEPLATFORM_DATA_TMPL_ADD_PROJECT_H_
#define FREELANCEPLATFORM_DATA_TMPL_ADD_PROJECT_H_

#include <cppcms/view.h>
#include <cppcms/form.h>
#include "tmpl_master.h"

namespace Data {
struct addProjectForm : public cppcms::form {
  cppcms::widgets::text name;                        // титул страницы
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

#endif //FREELANCEPLATFORM_DATA_TMPL_ADD_PROJECT_H_
