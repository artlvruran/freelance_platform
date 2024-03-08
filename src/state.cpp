//
// Created by kirill on 08.03.24.
//
#pragma once
#include "state.h"

std::unique_ptr<State> Preparing::on_event(event e) {
  if (e == event::hired) {
    return std::make_unique<Processing>();
  }
  return nullptr;
}

std::unique_ptr<State> Processing::on_event(event e) {
  if (e == event::completed) {
    return std::make_unique<Completed>();
  }
  return nullptr;
}

std::unique_ptr<State> Completed::on_event(event e) {
  if (e == event::completed) {
    return std::make_unique<Completed>();
  }
  return nullptr;
}