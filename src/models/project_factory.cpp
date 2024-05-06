//
// Created by kirill on 26.02.24.
//
#include "project_factory.h"

Task ProjectFactory::create_task(const std::string& name) const {
  return {name};
}

LongTermJob ProjectFactory::create_job(const std::string& name) const {
  return {name};
}

Contest ProjectFactory::create_contest(const std::string& name) const {
  return {name};
}