//
// Created by kirill on 31.03.24.
//

#pragma once
#include <cppcms/view.h>
#include <cppcms/form.h>
#include "tmpl_master.h"
#include "custom_fields.h"

using namespace cppcms;

namespace Data {
struct addTaskForm : public cppcms::form {
  cppcms::widgets::text name;
  cppcms::widgets::text description;
  cppcms::widgets::text location;
  number<double> wage;
  cppcms::widgets::submit submit;

  addTaskForm() {
    name.message("Project Name");
    description.message("Project description");
    location.message("Location");
    wage.message("Wage");
    wage.low(1);
    wage.name("wage");
    wage.attributes_string("class=\"euro-input\"");
    submit.value("Submit");

    add(name);
    add(description);
    add(location);
    add(wage);
    add(submit);

    name.non_empty();
  }
};

struct addLongForm : public cppcms::form {
  cppcms::widgets::text name;
  cppcms::widgets::text description;
  cppcms::widgets::text location;
  number<double> wage;
  cppcms::widgets::submit submit;
  cppcms::widgets::text specialization;
  cppcms::widgets::radio format;

  addLongForm() {
    name.message("Project Name");
    description.message("Project description");
    location.message("Location");
    wage.message("Wage");
    wage.low(1);
    wage.name("wage pre month");
    wage.attributes_string("class=\"euro-input\"");
    specialization.message("specialization");
    format.message("job format");
    format.add("remote");
    format.add("office");
    submit.value("Submit");

    add(name);
    add(description);
    add(location);
    add(wage);
    add(specialization);
    add(format);
    add(submit);

    name.non_empty();
  }
};

struct addContestForm : public cppcms::form {
  cppcms::widgets::text name;
  cppcms::widgets::text description;
  cppcms::widgets::text location;
  number<double> wage;
  cppcms::widgets::submit submit;
  number_time<std::string> start_at;
  number_time<std::string> end_at;


  addContestForm() {
    name.message("Project Name");
    description.message("Project description");
    location.message("Location");
    wage.message("Prize");
    wage.low(1);
    wage.name("wage");
    wage.attributes_string("class=\"euro-input\"");
    start_at.message("Starts at");
    end_at.message("Ends at");
    submit.value("Submit");

    add(name);
    add(description);
    add(location);
    add(wage);
    add(start_at);
    add(end_at);
    add(submit);

    name.non_empty();
  }
};

struct AddTask : public Master {
  Data::addTaskForm add_project_form;
};
struct AddLong : public Master {
  Data::addLongForm add_project_form;
};
struct AddContest : public Master {
  Data::addContestForm add_project_form;
};
struct AddProject : public Master {

};
}

