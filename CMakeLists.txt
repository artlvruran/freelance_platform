cmake_minimum_required(VERSION 3.5.1)
project(FreelancePlatform)

set(CMAKE_CXX_STANDARD 17)
execute_process(COMMAND ./make_templates.sh -i . -o ./build)
add_executable(FreelancePlatform main.cpp
        src/user.h
        src/project.h
        src/employee.h
        src/contractor.h
        src/project_factory.h
        src/contractor.cpp
        src/employee.cpp
        src/project.cpp
        src/project_factory.cpp
        src/constants.h
        src/state.h
        src/state.cpp
        src/constants.cpp
        src/bid.h
        src/bid_state.h
        src/bid_state.cpp
        src/bid.cpp
        src/db_pool.h
        src/user.cpp
        data/tmpl_master.h
        data/tmpl_signup.h
        data/tmpl_projects.h
        data/tmpl_project.h
        data/tmpl_add_project.h
        data/tmpl_contractors.h
        data/tmpl_user.h)
add_compile_options(-l sqlite3)
add_compile_options(-l soci_sqlite3)
add_compile_options(-l soci_core)
add_compile_options(-l cppcms)
add_compile_options(-l booster)
target_link_libraries(FreelancePlatform sqlite3 soci_core soci_sqlite3 cppcms booster)
