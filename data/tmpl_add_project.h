//
// Created by kirill on 31.03.24.
//

#pragma once
#include <cppcms/view.h>
#include <cppcms/form.h>
#include "tmpl_master.h"

using namespace cppcms;

template<typename T>
class number: public cppcms::widgets::base_html_input {
 public:
  number() :
      base_html_input("number"),
      check_low_(false),
      check_high_(false),
      non_empty_(false)
  {
  }

  ///
  /// Inform the validator that this widget should contain some value.
  ///
  void non_empty()
  {
    non_empty_=true;
  }


  ///
  /// Get numeric value that was loaded from the POST or
  /// GET data.
  ///
  /// \note if the value was not set (empty field for example)
  /// then this function will throw. So it is good idea to
  /// check if \ref set() returns true before using this
  /// function.
  ///
  T value()
  {
    if(!set())
      throw cppcms_error("Value not loaded");
    return value_;
  }

  ///
  /// Set the value of the widget.
  ///
  void value(T v)
  {
    set(true);
    value_=v;
  }

  ///
  /// Set the minimum valid value.
  ///
  void low(T a)
  {
    min_=a;
    check_low_=true;
    non_empty();
  }

  ///
  /// Set the maximum valid value.
  ///
  void high(T b)
  {
    max_=b;
    check_high_=true;
    non_empty();
  }

  ///
  /// Same as low(a); high(b);
  ///
  void range(T a,T b)
  {
    low(a);
    high(b);
  }

  ///
  /// Render the first part of the widget.
  ///
  virtual void render_value(form_context &context)
  {
    if(set())
      context.out()<<"value=\""<<value_<<"\" ";
    else
      context.out()<<"value=\""<<util::escape(loaded_string_)<<"\" ";
  }

  virtual void clear()
  {
    base_html_input::clear();
    loaded_string_.clear();
  }

  ///
  /// Load the widget data.
  ///
  virtual void load(http::context &context)
  {
    pre_load(context);

    loaded_string_.clear();

    set(false);
    valid(true);

    http::request::form_type::const_iterator p;
    http::request::form_type const &request=context.request().post_or_get();
    p=request.find(name());
    if(p==request.end()) {
      return;
    }
    else {
      loaded_string_=p->second;
      if(loaded_string_.empty())
        return;

      std::istringstream ss(loaded_string_);
      ss.imbue(context.locale());
      ss>>value_;
      if(ss.fail() || !ss.eof())
        valid(false);
      else
        set(true);
    }
  }

  ///
  /// Validate the widget.
  ///
  virtual bool validate()
  {
    if(!valid())
      return false;
    if(!set()) {
      if(non_empty_) {
        valid(false);
        return false;
      }
      return true;
    }
    if(check_low_ && value_ <min_) {
      valid(false);
      return false;
    }
    if(check_high_ && value_ > max_) {
      valid(false);
      return false;
    }
    return true;
  }

 private:

  T min_,max_,value_;

  bool check_low_;
  bool check_high_;
  bool non_empty_;
  std::string loaded_string_;
};


namespace Data {
struct addProjectForm : public cppcms::form {
  cppcms::widgets::text name;
  cppcms::widgets::text description;
  cppcms::widgets::text location;
  number<double> wage;
  cppcms::widgets::submit submit;

  addProjectForm() {
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

struct AddProject : public Master {
  Data::addProjectForm add_project_form;
};
}

