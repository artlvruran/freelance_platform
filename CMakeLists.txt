cmake_minimum_required(VERSION 3.5.1)
project(FreelancePlatform)

set(CMAKE_CXX_STANDARD 17)
execute_process(COMMAND ./make_templates.sh -i . -o ./build)
add_executable(FreelancePlatform main.cpp
        src/models/user.h
        src/models/project.h
        src/models/employee.h
        src/models/contractor.h
        src/models/project_factory.h
        src/models/contractor.cpp
        src/models/employee.cpp
        src/models/project.cpp
        src/models/project_factory.cpp
        src/models/constants.h
        src/models/state.h
        src/models/state.cpp
        src/models/constants.cpp
        src/models/bid.h
        src/models/bid_state.h
        src/models/bid_state.cpp
        src/models/bid.cpp
        src/models/db_pool.h
        src/models/user.cpp
        data/tmpl_master.h
        data/tmpl_signup.h
        data/tmpl_projects.h
        data/tmpl_project.h
        data/tmpl_add_project.h
        data/tmpl_contractors.h
        data/tmpl_user.h
        src/database.h)
add_compile_options(-l sqlite3)
add_compile_options(-l soci_sqlite3)
add_compile_options(-l soci_core)
add_compile_options(-l cppcms)
add_compile_options(-l booster)
target_link_libraries(FreelancePlatform sqlite3 soci_core soci_sqlite3 cppcms booster)
