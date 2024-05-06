//
// Created by kirill on 06.05.24.
//

#pragma once
#include <cppcms/view.h>
#include <cppcms/form.h>

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

  void non_empty()
  {
    non_empty_=true;
  }

  T value()
  {
    if(!set())
      throw cppcms_error("Value not loaded");
    return value_;
  }

  void value(T v)
  {
    set(true);
    value_=v;
  }

  void low(T a)
  {
    min_=a;
    check_low_=true;
    non_empty();
  }

  void high(T b)
  {
    max_=b;
    check_high_=true;
    non_empty();
  }

  void range(T a,T b)
  {
    low(a);
    high(b);
  }

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

template<typename T>
class number_time: public cppcms::widgets::base_html_input {
 public:
  number_time() :
      base_html_input("datetime-local"),
      check_low_(false),
      check_high_(false),
      non_empty_(false) {
  }

  void non_empty() {
    non_empty_=true;
  }

  T value() {
    if(!set())
      throw cppcms_error("Value not loaded");
    return value_;
  }

  void value(T v) {
    set(true);
    value_=v;
  }

  void low(T a) {
    min_=a;
    check_low_=true;
    non_empty();
  }

  void high(T b) {
    max_=b;
    check_high_=true;
    non_empty();
  }

  void range(T a,T b) {
    low(a);
    high(b);
  }

  virtual void render_value(form_context &context) {
    if(set())
      context.out()<<"value=\""<<value_<<"\" ";
    else
      context.out()<<"value=\""<<util::escape(loaded_string_)<<"\" ";
  }

  virtual void clear() {
    base_html_input::clear();
    loaded_string_.clear();
  }

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

  virtual bool validate() {
    return true;
  }

 private:

  T min_,max_,value_;

  bool check_low_;
  bool check_high_;
  bool non_empty_;
  std::string loaded_string_;
};
