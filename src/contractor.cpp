//
// Created by kirill on 26.02.24.
//
#include "contractor.h"
#include "sqlite3.h"
#include "constants.h"
#include <boost/format.hpp>
#include <string>

void Contractor::sign_up() {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "insert into users (username, email, password, fullname, role) values(:username, :email, :password, :fullname, 'contractor')", soci::use(*this);
  int idd;
  sql << "select id from users "
         "  where username == :username", soci::into(idd), soci::use(username);
  id = idd;
}

bool Contractor::log_in() {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);

  int cnt;
  sql << "select count(*) from users where username = :username and password = :password and role = 'contractor'", soci::use(*this), soci::into(cnt);
  return cnt == 1;
}

void Contractor::consider_bid(Bid& bid, bid_event e) {

  int contractor_id;

  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "select contractor_id from projects"
         "  where id == ("
         "               select project_id from bids"
         "                  where id == :id"
         ")", soci::into(contractor_id), soci::use(bid.id);

  if (contractor_id != id) {
    throw std::runtime_error("Error in responsibility of contractor");
  }

  bid.advance(e);
}

void Contractor::add_project(Project &project) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << (boost::format("insert into projects (name, contractor_id, state, location, wage, description)"
         "values(:name, %d, :state, :location, :wage, :description)") % id).str(), soci::use(project);
  sql << "select id from projects "
         "where name == :name", soci::use(project.name), soci::into(project.id);
  project.advance(event::start);
  notify_observers("Project " + project.name + " has been created.");
  sql.close();
}

void Contractor::fire_worker(const Project& project, const Employee& employee) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "delete from bids "
         "where employee_id == :id and project_id == :id1", soci::use(employee.id), soci::use(project.id);
}

void Contractor::end_project(Project& project) {
  project.advance(event::completed);
  notify_observers("Project " + project.name + " has been ended.");
}

void Contractor::start_project_hiring(Project& project) {
  project.advance(event::start);
  notify_observers("Project " + project.name + " has started its hiring.");
}


void Contractor::end_project_hiring(Project& project) {
  project.advance(event::hired);
  notify_observers("Project " + project.name + " has finished its hiring.");
}

void Contractor::register_observer(const User &user) const {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "insert into subscriptions (contractor_id, user_id)"
         "values(:id, :user_id)", soci::use(id), soci::use(user.id);
}

void Contractor::remove_observer(const User &user) const {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  sql << "delete from subscriptions "
         "where contractor_id == :id and user_id == :user_id", soci::use(id), soci::use(user.id);
  sql.close();
}

void Contractor::notify_observers(std::string description) const {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  soci::rowset<int> rs = (sql.prepare << "select user_id from subscriptions where contractor_id = :id", soci::use(id));
  for (auto it = rs.begin(); it != rs.end(); ++it) {
    int user_id = *it;
    sql << "insert into notifications (contractor_id, user_id, is_read, description)"
           "  values(:contractor_id, :user_id, false, :description)", soci::use(id), soci::use(user_id), soci::use(description);
  }
}