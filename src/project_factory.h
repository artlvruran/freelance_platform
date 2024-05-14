//
// Created by kirill on 26.02.24.
//

#pragma once
#include "project.h"

class ProjectFactory {
  virtual Task create_task(const std::string& name) const = 0;
  virtual LongTermJob create_job(const std::string& name) const = 0;
  virtual Contest create_contest(const std::string& name) const = 0;
};

