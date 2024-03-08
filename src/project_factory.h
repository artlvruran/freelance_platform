//
// Created by kirill on 26.02.24.
//

#ifndef FREELANCEPLATFORM_SRC_PROJECT_FACTORY_H_
#define FREELANCEPLATFORM_SRC_PROJECT_FACTORY_H_

#pragma once
#include "project.h"

class ProjectFactory {
  virtual Task create_task(std::string& name,
                            std::string& description,
                            std::string& timeline,
                            double budget) const = 0;
  virtual LongTermJob create_job(std::string& name,
                                  std::string& description,
                                  std::string& timeline,
                                  double budget) const = 0;
  virtual Contest create_contest(std::string& name,
                                  std::string& description,
                                  std::string& timeline,
                                  double budget) const = 0;
};

#endif //FREELANCEPLATFORM_SRC_PROJECT_FACTORY_H_
