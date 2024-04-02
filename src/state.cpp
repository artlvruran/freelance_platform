//
// Created by kirill on 08.03.24.
//
#include "state.h"

std::unique_ptr<State> NotStarted::on_event(event e) {
  if (e == event::start) {
    return std::make_unique<Preparing>();
  }
  return nullptr;
}

int NotStarted::integer() const {
  return 0;
}

std::unique_ptr<State> Preparing::on_event(event e) {
  if (e == event::hired) {
    return std::make_unique<Processing>();
  }
  return nullptr;
}

int Preparing::integer() const {
  return 1;
}

std::unique_ptr<State> Processing::on_event(event e) {
  if (e == event::completed) {
    return std::make_unique<Completed>();
  }
  return nullptr;
}

int Processing::integer() const {
  return 2;
}

std::unique_ptr<State> Completed::on_event(event e) {
  if (e == event::completed) {
    return std::make_unique<Completed>();
  }
  return nullptr;
}

int Completed::integer() const {
  return 3;
}