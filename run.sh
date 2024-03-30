#!/bin/bash
cmake -S . -B build
cmake --build build
cd build
./FreelancePlatform -c ../config.json
cd ..