//
// Created by kirill on 26.02.24.
//

#pragma once
#include "project_factory.h"

Task ProjectFactory::create_task(std::string& name,
                                  std::string& description,
                                  std::string& timeline,
                                  double budget) const {
  return {name, description, timeline, budget};
}

LongTermJob ProjectFactory::create_job(std::string& name,
                                 std::string& description,
                                 std::string& timeline,
                                 double budget) const {
  return {name, description, timeline, budget};
}

Contest ProjectFactory::create_contest(std::string& name,
                                     std::string& description,
                                     std::string& timeline,
                                     double budget) const {
  return {name, description, timeline, budget};
}