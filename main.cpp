#include <cppcms/applications_pool.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/application.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include <cppcms/application.h>
#include <cppcms/http_file.h>
#include "data/tmpl_master.h"
#include "data/tmpl_signup.h"
#include "data/tmpl_projects.h"
#include "data/tmpl_project.h"
#include "data/tmpl_add_project.h"
#include "data/tmpl_contractors.h"
#include "data/tmpl_user.h"
#include "data/tmpl_profile_edit.h"
#include "data/tmpl_notifications.h"
#include "data/tmpl_employees.h"
#include "data/tmpl_main.h"
#include "data/tmpl_send_message.h"
#include "src/employee.h"
#include "src/contractor.h"
#include "src/constants.h"
#include "src/message.h"
#include <fstream>

template<typename T>
void add_menu(T& object, cppcms::application& app) {
  if (app.session().is_set("username")) {
    object.current_username = app.session()["username"];
  }
  object.menuList.push_back(std::pair<std::string,std::string>("/","Main"));
  object.menuList.push_back(std::pair<std::string,std::string>("/projects","Projects"));
  object.menuList.push_back(std::pair<std::string,std::string>("/users/contractors","Contractors"));
  object.menuList.push_back(std::pair<std::string,std::string>("/users/employees","Creatives"));
  object.menuList.push_back(std::pair<std::string,std::string>("/signup","Sign up"));
  object.menuList.push_back(std::pair<std::string,std::string>("/login","Log in"));
  object.menuList.push_back(std::pair<std::string,std::string>("/logout","Log out"));

  if (app.session().is_set("username")) {
    object.menuList.push_back(std::pair<std::string,std::string>("/notifications","notifications"));
    std::vector<std::pair<int, std::string>> notifications;

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);

    int user_id;
    sql << "select id from users where username=:username", soci::use(app.session()["username"]), soci::into(user_id);

    soci::rowset<soci::row> nots = (sql.prepare << "select * from notifications where user_id=:user_id" , soci::use(user_id));
    for (auto &it : nots) {
      if (!it.get<int>("is_read")) {
        notifications.emplace_back(it.get<int>("id"), it.get<std::string>("description"));
      }
    }

    object.notifications = notifications;
  }

  if (app.session().is_set("role") && app.session()["role"] == "contractor") {
    object.menuList.push_back(std::pair<std::string, std::string>("/projects/add_project", "Add Project"));
  }
}

User get_user(cppcms::application& app) {
  if (app.session().is_set("username")) {
    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    User current_user;
    sql << "select * from users where username=:username",
        soci::use(app.session()["username"]), soci::into(current_user);
    return current_user;
  } else {
    throw std::runtime_error("current user is not set");
  }
  return {};
}

Employee get_employee(cppcms::application& app) {
  if (app.session().is_set("username")) {
    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    Employee current_user;
    sql << "select * from users where username=:username",
        soci::use(app.session()["username"]), soci::into(current_user);
    return current_user;
  } else {
    throw std::runtime_error("current user is not set");
  }
  return {};
}

User get_user_by_id(cppcms::application& app, const std::string& id) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  User user;
  sql << "select * from users where id=:id", soci::use(id), soci::into(user);
  return user;
}

Employee get_employee_by_id(cppcms::application& app, const std::string& id) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);
  Employee user;
  sql << "select * from users where id=:id", soci::use(id), soci::into(user);
  return user;
}

double calculate_rating(const User& user) {
  std::string src = "dbname=";
  src += db_source;
  soci::session sql("sqlite3", src);

  soci::rowset<int> rates = (sql.prepare << "select rate from rates where target_id=:target_id",
      soci::use(user.id));
  long long sum = 0;
  int sz = 0;

  for (int& rate : rates) {
    sum += rate;
    ++sz;
  }
  return double(sum) / sz;
}

class Chat : public cppcms::application {
 public:
  Chat(cppcms::service &srv) : cppcms::application(srv) {
    dispatcher().assign("/(.+)/(.+)", &Chat::chat, this, 1, 2);
    mapper().assign("/{1}/{2}");
  }

  virtual void chat(std::string from_id, std::string to_id) {
    Data::ChatView view;
    add_menu(view, *this);
    if (!session().is_set("username")) {
      response().set_redirect_header("/");
      return;
    }
    auto current_user = get_user(*this);
    if (current_user.id != std::stoi(from_id)) {
      response().set_redirect_header("/");
      return;
    }

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);

    if (request().request_method() == "POST") {
      view.chat_form.load(context());
      if (view.chat_form.validate()) {
        auto message = view.chat_form.message.value();
        sql << "insert into messages (sender_id, address_id, text) values(:sender_id, :address_id, :text)",
            soci::use(current_user.id), soci::use(to_id), soci::use(message);
      }
    }

    soci::rowset<Message> my_messages = (sql.prepare << "select * from messages where sender_id=:sender_id and address_id=:address_id",
        soci::use(from_id), soci::use(to_id));
    soci::rowset<Message> their_messages = (sql.prepare << "select * from messages where sender_id=:sender_id and address_id=:address_id",
        soci::use(to_id), soci::use(from_id));

    for (auto& my_message : my_messages) {
      view.messages.emplace_back(0, my_message);
    }
    for (auto& their_message : their_messages) {
      view.messages.emplace_back(1, their_message);
    }

    auto comparator = [](const std::pair<bool, Message>& first, const std::pair<bool, Message>& second) {
      return first.second.id < second.second.id;
    };

    std::sort(view.messages.begin(), view.messages.end(), comparator);
    render("ChatView", view);
  }
};

class Users : public cppcms::application {
 public:
  Users(cppcms::service &srv) : cppcms::application(srv) {
    dispatcher().assign("/contractors", &Users::contractors, this);
    mapper().assign("/contractors");

    dispatcher().assign("/employees", &Users::employees, this);
    mapper().assign("/employees");

    dispatcher().assign("/(.+)/rate/(.+)", &Users::rate, this, 1, 2);
    mapper().assign("/{1}/rate/{2}");

    dispatcher().assign("/(.+)/subscribe", &Users::subscribe, this, 1);
    mapper().assign("/{1}");

    dispatcher().assign("/(.+)/unsubscribe", &Users::unsubscribe, this, 1);
    mapper().assign("/{1}");

    dispatcher().assign("/(.+)/edit", &Users::edit, this, 1);
    mapper().assign("/{1}");

    dispatcher().assign("/(.+)", &Users::user, this, 1);
    mapper().assign("/{1}");
  }

  virtual void contractors() {
    Data::Contractors cntr;

    add_menu(cntr, *this);

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    soci::rowset<Contractor> contractors = (sql.prepare << "select * from users where role='contractor'");
    std::vector<Contractor> all_contractors;
    for (auto it = contractors.begin(); it != contractors.end(); ++it) {
      all_contractors.push_back(*it);
    }
    cntr.contractors_page.contractors = all_contractors;
    render("Contractors", cntr);
  }

  virtual void employees() {
    Data::Employees empl;

    add_menu(empl, *this);

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    soci::rowset<Employee> employees = (sql.prepare << "select * from users where role='employee'");
    std::vector<Employee> all_employees;
    for (auto it = employees.begin(); it != employees.end(); ++it) {
      all_employees.push_back(*it);
    }
    empl.employees_page.employees = all_employees;
    render("Employees", empl);
  }

  virtual void user(std::string id) {
    Data::SingleUser usr;

    add_menu(usr, *this);

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);

    User user = get_user_by_id(*this, id);

    usr.user_page.user = user;

    if (session().is_set("username") && user.user_role == role::contractor) {
      User current_user = get_user(*this);

      int cnt;
      sql << "select count(*) from subscriptions where contractor_id=:contractor_id and user_id=:user_id",
          soci::use(id), soci::use(current_user.id), soci::into(cnt);

      if (cnt != 0) {
        usr.user_page.is_subscribed = true;
      } else {
        usr.user_page.is_subscribed = false;
      }
    }

    if (session().is_set("username")) {
      User current_user = get_user(*this);
      usr.my_rate = 0;
      sql << "select rate from rates where rater_id=:rater_id and target_id=:target_id",
          soci::use(current_user.id), soci::use(id), soci::into(usr.my_rate);

      usr.my_id = current_user.id;
    }

    if (session().is_set("username") && session()["username"] == usr.user_page.user.username) {
      usr.is_me = true;
    } else {
      usr.is_me = false;
    }


    render("SingleUser", usr);
  }

  virtual void subscribe(std::string id) {
    if (session().is_set("username")) {
      std::string src = "dbname=";
      src += db_source;
      soci::session sql("sqlite3", src);
      Contractor contractor;
      sql << "select * from users where id=:id and role='contractor'", soci::use(id), soci::into(contractor);

      User user;
      sql << "select * from users where username=:username",
          soci::use(session()["username"]), soci::into(user);

      contractor.register_observer(user);
      response().set_redirect_header("/users/" + std::to_string(contractor.id));
      return;
    }
    response().set_redirect_header("/");
  }

  virtual void unsubscribe(std::string id) {
    if (session().is_set("username")) {
      std::string src = "dbname=";
      src += db_source;
      soci::session sql("sqlite3", src);
      Contractor contractor;
      sql << "select * from users where id=:id and role='contractor'", soci::use(id), soci::into(contractor);

      User user;
      sql << "select * from users where username=:username",
          soci::use(session()["username"]), soci::into(user);

      contractor.remove_observer(user);
      response().set_redirect_header("/users/" + std::to_string(contractor.id));
      return;
    }
    response().set_redirect_header("/");
  }

  virtual void edit(std::string id) {
    Data::ProfileEdit edt;

    add_menu(edt, *this);

    if (request().request_method() == "POST") {

      edt.edit_form.load(context());
      auto current_user = get_user(*this);
      std::string src = "dbname=";
      src += db_source;
      soci::session sql("sqlite3", src);
      std::string avatar_name;
      std::string cover_name;

      if (edt.edit_form.avatar_file.validate()) {
        avatar_name = std::to_string(std::hash<std::string>()(session()["username"]));
        if (edt.edit_form.avatar_file.value()->mime() == "image/png") {
          avatar_name += ".png";
        } else {
          avatar_name += ".jpg";
        }
        edt.edit_form.avatar_file.value()->save_to("./media/images/" + avatar_name);
        current_user.avatar = avatar_name;

        sql << "update users "
               "set avatar = :avatar "
               "where id = :id", soci::use(avatar_name), soci::use(current_user.id);
      }

      if (edt.edit_form.cover_file.validate()) {
        cover_name = std::to_string(std::hash<std::string>()(session()["username"] + "cover"));
        if (edt.edit_form.cover_file.value()->mime() == "image/png") {
          cover_name += ".png";
        } else {
          cover_name += ".jpg";
        }
        edt.edit_form.cover_file.value()->save_to("./media/images/" + cover_name);
        current_user.cover = cover_name;
        sql << "update users "
               "set cover = :cover "
               "where id = :id", soci::use(cover_name), soci::use(current_user.id);

      }
    }


    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    User user;
    sql << "select * from users where id=:id", soci::use(id), soci::into(user);

    if (session().is_set("username") && session()["username"] == user.username) {
      edt.is_me = true;
    } else {
      edt.is_me = false;
    }

    render("ProfileEdit", edt);
  }

  virtual void rate(std::string id, std::string grade) {
    auto target_user = get_user_by_id(*this, id);
    if (request().request_method() == "POST" && session().is_set("username")) {
      auto current_user = get_user(*this);

      std::string src = "dbname=";
      src += db_source;
      soci::session sql("sqlite3", src);

      int cnt;
      sql << "select count(*) from rates where rater_id=:rater_id and target_id=:target_id",
          soci::use(current_user.id), soci::use(id), soci::into(cnt);

      if (cnt == 0) {
        sql << "insert into rates (rater_id, target_id, rate) values(:rater_id, :target_id, :rate)",
                soci::use(current_user.id), soci::use(id), soci::use(std::stod(grade));
      } else {
        sql << "update rates "
               "set rate=:rate "
               "where rater_id=:rater_id and target_id=:target_id", soci::use(grade), soci::use(current_user.id), soci::use(id);
      }

      target_user.rate = calculate_rating(target_user);
      sql << "update users "
             "set rate=:rate "
             "where id=:id ", soci::use(target_user);
    }
    response().set_redirect_header("/users/" + id);
  }
};

class Projects : public cppcms::application {
 public:
  Projects(cppcms::service &srv) : cppcms::application(srv) {
    dispatcher().assign("/bid_on/(.+)", &Projects::bid_on, this, 1);
    mapper().assign("/bid_on/{1}");

    dispatcher().assign("/add_project", &Projects::add_project, this);
    mapper().assign("/add_project");

    dispatcher().assign("/(.+)/advance", &Projects::advance, this, 1);
    mapper().assign("/{1}/advance");

    dispatcher().assign("/(.+)/(.+)", &Projects::consider, this, 1, 2);
    mapper().assign("/{1}/{2}");

    dispatcher().assign("/(.+)", &Projects::project, this, 1);
    mapper().assign("/{1}");

    dispatcher().assign("", &Projects::projects, this);
    mapper().assign("");

    dispatcher().assign("/", &Projects::projects, this);
    mapper().assign("/");
  }

  virtual void projects() {
    Data::Projects mn;

    add_menu(mn, *this);

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    soci::rowset<Project> projects = (sql.prepare << "select * from projects");
    std::vector<std::pair<Project, Contractor>> all_projects;
    for (auto it = projects.begin(); it != projects.end(); ++it) {
      Contractor contractor;
      sql << "select * from users where id = :contractor_id and role='contractor'",
             soci::use(it->contractor_id), soci::into(contractor);
      all_projects.push_back({*it, contractor});
    }
    mn.projects_page.projects = all_projects;

    render("Projects", mn);
  }

  virtual void project(std::string id) {
    Data::SingleProject pr;

    add_menu(pr, *this);

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    Project project;
    sql << "select * from projects where id=:id", soci::use(id), soci::into(project);

    pr.project_page.project = project;
    if (session().is_set("role")) {
      if (session()["role"] == "employee") {
        pr.project_page.is_employee = true;
        soci::indicator ind;
        Bid bid;
        sql << "select * from bids where project_id=:project_id and employee_id=("
               "select id from users where role='employee' and username=:username"
               ")",
            soci::use(id), soci::use(session()["username"]), soci::into(bid, ind);
        if (sql.got_data() && ind == soci::i_ok) {
          pr.project_page.is_bid_created = true;
          pr.project_page.bids.emplace_back(bid, get_employee(*this));
        }
      } else {
        pr.project_page.is_employee = false;

        int contractor_id;
        sql << "select contractor_id from projects where id=:id", soci::use(id), soci::into(contractor_id);

        Contractor contractor;
        sql << "select * from users where role='contractor' and username=:username",
            soci::use(session()["username"]), soci::into(contractor);

        if (contractor_id == contractor.id) {
          pr.project_page.has_right = true;
          soci::rowset<Bid> rs = (sql.prepare << "select * from bids where project_id = :id", soci::use(id));
          for (auto& bid : rs) {
            pr.project_page.bids.emplace_back(bid, get_employee_by_id(*this, std::to_string(bid.employee_id)));
          }
        } else {
          pr.project_page.has_right = false;
        }
      }
    }
    render("SingleProject", pr);
  }

  virtual void bid_on(std::string id) {
    if (session().is_set("role")) {
      if (session()["role"] == "employee") {
        std::string src = "dbname=";
        src += db_source;
        soci::session sql("sqlite3", src);
        Project project;
        sql << "select * from projects where id=:id", soci::use(id), soci::into(project);

        Employee employee;
        sql << "select * from users where role='employee' and username=:username",
            soci::use(session()["username"]), soci::into(employee);

        employee.create_bid(project.id);

        response().set_redirect_header("/projects/" + id);
        return;
      } else {
        response().set_redirect_header("/projects/" + id);
        return;
      }
    }
  }

  virtual void consider(std::string bid_id, std::string status) {
    if (session().is_set("role")) {
      if (session()["role"] == "employee") {
        response().set_redirect_header("/projects/");
        return;
      } else {
        int contractor_id;

        std::string src = "dbname=";
        src += db_source;
        soci::session sql("sqlite3", src);
        sql << "select contractor_id from projects where id=("
               "select project_id from bids where id=:bid_id"
               ")", soci::use(bid_id), soci::into(contractor_id);

        Contractor contractor;
        sql << "select * from users where role='contractor' and username=:username",
            soci::use(session()["username"]), soci::into(contractor);
        if (contractor_id == contractor.id) {
          Bid bid;
          sql << "select * from bids where id=:bid_id", soci::use(bid_id), soci::into(bid);
          contractor.consider_bid(bid, (status == "approve" ? bid_event::approve : bid_event::reject));
          int project_id;
          sql << "select project_id from bids where id=:bid_id",
              soci::use(bid_id), soci::into(project_id);
          response().set_redirect_header("/projects/" + std::to_string(project_id));
          return;
        }
        response().set_redirect_header("/");
        return;
      }
    }
    response().set_redirect_header("/");
  }

  virtual void add_project() {
    Data::AddProject addpr;
    add_menu(addpr, *this);
    if (request().request_method() == "POST") {
      addpr.add_project_form.load(context());
      if (addpr.add_project_form.validate()) {
        if (session().is_set("role") && session()["role"] == "contractor") {
          Contractor contractor;
          Project project;
          project.name = addpr.add_project_form.name.value();
          project.description = addpr.add_project_form.description.value();
          project.wage = addpr.add_project_form.wage.value();
          project.location = addpr.add_project_form.location.value();
          project.state = std::make_unique<Preparing>();

          std::string src = "dbname=";
          src += db_source;
          soci::session sql("sqlite3", src);
          sql << "select * from users where role='contractor' and username=:username",
              soci::use(session()["username"]), soci::into(contractor);

          contractor.add_project(project);

          response().set_redirect_header("/projects");
          return;
        }
        response().set_redirect_header("/");
        return;
      }
    }

    render("AddProject", addpr);
  }

  virtual void advance(std::string id) {
    if (session().is_set("role")) {
      if (session()["role"] == "contractor") {
        std::string src = "dbname=";
        src += db_source;
        soci::session sql("sqlite3", src);
        Project project;
        sql << "select * from projects where id=:id", soci::use(id), soci::into(project);

        Contractor contractor;
        sql << "select * from users where role='contractor' and username=:username",
            soci::use(session()["username"]), soci::into(contractor);

        int contractor_id;
        sql << "select contractor_id from projects where id=:id", soci::use(id), soci::into(contractor_id);

        if (contractor_id == contractor.id) {
          if (project.state->integer() == 0) {
            contractor.start_project_hiring(project);
          } else if (project.state->integer() == 1) {
            contractor.end_project_hiring(project);
          } else {
            contractor.end_project(project);
          }
          response().set_redirect_header("/projects/" + id);
          return;
        }
        response().set_redirect_header("/");
      } else {
        response().set_redirect_header("/");
      }
    }
    response().set_redirect_header("/");
  }
};

class WebSite : public cppcms::application {
 public:
  WebSite(cppcms::service& s) : cppcms::application(s) {
    attach(new Users(s),
           "users", "/users/{1}", "/users(/(.*))?", 1);

    attach(new Projects(s),
           "projects", "/projects/{1}", "/projects(/(.*))?", 1);

    attach(new Chat(s),
           "chat", "/chat/{1}", "/chat(/(.*))?", 1);

    dispatcher().assign("/signup", &WebSite::signup, this);
    mapper().assign("signup");

    dispatcher().assign("/login", &WebSite::login, this);
    mapper().assign("login");

    dispatcher().assign("/logout", &WebSite::log_out, this);
    mapper().assign("logout");

    dispatcher().assign("/notifications", &WebSite::notifications, this);
    mapper().assign("/notifications");

    dispatcher().assign("/", &WebSite::master, this);
    mapper().assign("/");
  };

  virtual void main(std::string path) {
    cppcms::application::main(path);
  }

  virtual void master() {
    Data::Main tmpl;
    add_menu(tmpl, *this);
    render("Main", tmpl);
  }

  virtual void signup() {
    Data::Signup sgn;
    add_menu(sgn, *this);

    if (request().request_method() == "POST") {
      sgn.info.load(context());
      if (sgn.info.validate()) {
        if (sgn.info.role.selected_id() == "0") {
          Employee employee;
          employee.username = sgn.info.username.value();
          employee.email = sgn.info.email.value();
          employee.password = sgn.info.password.value();
          employee.fullname = sgn.info.fullname.value();
          employee.sign_up();
        } else {
          Contractor contractor;
          contractor.username = sgn.info.username.value();
          contractor.email = sgn.info.email.value();
          contractor.password = sgn.info.password.value();
          contractor.fullname = sgn.info.fullname.value();
          contractor.sign_up();
        }
        session().set("username", sgn.info.username.value());
        session().set("email", sgn.info.email.value());
        session().set("password", sgn.info.password.value());
        session().set("role", (sgn.info.role.selected_id() == "0" ? "employee" : "contractor"));
        response().set_redirect_header("/");
        return;
      }
    }

    render("Signup", sgn);
  }

  virtual void login() {
    Data::Login lgn;
    add_menu(lgn, *this);

    if (request().request_method() == "POST") {
      lgn.info.load(context());
      if (lgn.info.validate()) {
        if (lgn.info.role.selected_id() == "0") {
          Employee employee;
          employee.username = lgn.info.username.value();
          employee.password = lgn.info.password.value();
          if (employee.log_in()) {
            session().set("username", lgn.info.username.value());
            session().set("password", lgn.info.password.value());
            session().set("role", (lgn.info.role.selected_id() == "0" ? "employee" : "contractor"));

            response().set_redirect_header("/");
            return;
          }
        } else {
          Contractor contractor;
          contractor.username = lgn.info.username.value();
          contractor.password = lgn.info.password.value();
          if (contractor.log_in()) {
            session().set("username", lgn.info.username.value());
            session().set("password", lgn.info.password.value());
            session().set("role", (lgn.info.role.selected_id() == "0" ? "employee" : "contractor"));
            response().set_redirect_header("/");
            return;
          }
        }
      }
    }

    render("Login", lgn);
  }



  virtual void log_out() {
    session().erase("username");
    session().erase("email");
    session().erase("password");
    session().erase("role");
    response().set_redirect_header("/");
  }



  virtual void notifications() {
    if (!session().is_set("username")) {
      response().set_redirect_header("/");
      return;
    }

    Data::Notifications ntf;
    add_menu(ntf, *this);


    if (request().request_method() == "POST") {
      std::string paramlist = request().query_string();
      int notification_id = std::stoi(paramlist.substr(11, paramlist.size() - 10));

      std::string src = "dbname=";
      src += db_source;
      soci::session sql("sqlite3", src);

      int user_id;
      sql << "select id from users where username=:username", soci::use(session()["username"]), soci::into(user_id);

      int not_user_id;
      sql << "select user_id from notifications where id=:id", soci::use(notification_id), soci::into(not_user_id);

      if (user_id == not_user_id) {
        User::read(notification_id);
      }
      response().set_redirect_header("/");
      return;
    }


    render("Notifications", ntf);
  }
};


int main(int argc, char** argv) {
  try {
    cppcms::service srv(argc, argv);
    srv.applications_pool().mount(cppcms::applications_factory<WebSite>());
    srv.run();
  } catch (std::exception const& e) {
    std::cerr << "Failed: " << e.what() << std::endl;
    std::cerr << booster::trace(e) << std::endl;
    return 1;
  }
  return 0;
}