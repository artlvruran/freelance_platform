//
// Created by kirill on 08.03.24.
//
#pragma once
#include <iostream>
#include <memory>

enum event {
  start,
  hired,
  completed
};

struct State {
  virtual std::unique_ptr<State> on_event(event e) = 0;
  virtual int integer() const = 0;
};

struct NotStarted : State {
  std::unique_ptr<State> on_event(event e) override;
  int integer() const override;
};

struct Preparing : State {
  std::unique_ptr<State> on_event(event e) override;
  int integer() const override;
};

struct Processing : State {
  std::unique_ptr<State> on_event(event e) override;
  int integer() const override;
};

struct Completed : State {
  std::unique_ptr<State> on_event(event e) override;
  int integer() const override;
};
