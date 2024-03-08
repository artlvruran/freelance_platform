//
// Created by kirill on 08.03.24.
//

#ifndef FREELANCEPLATFORM_SRC_STATE_H_
#define FREELANCEPLATFORM_SRC_STATE_H_
#include "project.h"
#include <iostream>
#include <memory>

struct State {
  virtual std::unique_ptr<State> on_event(event e) = 0;
};

struct Preparing : State {
  std::unique_ptr<State> on_event(event e) override;
};

struct Processing : State {
  std::unique_ptr<State> on_event(event e) override;

};

struct Completed : State {
  std::unique_ptr<State> on_event(event e) override;
};


#endif //FREELANCEPLATFORM_SRC_STATE_H_
