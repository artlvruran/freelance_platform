#include <cppcms/applications_pool.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/application.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include <cppcms/application.h>
#include "data/tmpl_master.h"
#include "data/tmpl_signup.h"
#include "data/tmpl_projects.h"
#include "data/tmpl_project.h"
#include "data/tmpl_add_project.h"
#include "data/tmpl_contractors.h"
#include "data/tmpl_user.h"
#include "src/employee.h"
#include "src/contractor.h"
#include "src/constants.h"

class WebSite : public cppcms::application {
 public:
  WebSite(cppcms::service& s) : cppcms::application(s) {
    dispatcher().assign("/signup", &WebSite::signup, this);
    mapper().assign("signup");

    dispatcher().assign("/login", &WebSite::login, this);
    mapper().assign("login");

    dispatcher().assign("/logout", &WebSite::log_out, this);
    mapper().assign("logout");

    dispatcher().assign("/users/contractors", &WebSite::contractors, this);
    mapper().assign("/users/contractors");

    dispatcher().assign("/users/(.+)/subscribe", &WebSite::subscribe, this, 1);
    mapper().assign("/users/{1}");

    dispatcher().assign("/users/(.+)", &WebSite::user, this, 1);
    mapper().assign("/users/{1}");


    dispatcher().assign("/projects/bid_on/(.+)", &WebSite::bid_on, this, 1);
    mapper().assign("/projects/bid_on/{1}");

    dispatcher().assign("/projects/add_project", &WebSite::add_project, this);
    mapper().assign("/projects/add_project");

    dispatcher().assign("/projects/(.+)/advance", &WebSite::advance, this, 1);
    mapper().assign("/projects/{1}/advance");

    dispatcher().assign("/projects/(.+)/(.+)", &WebSite::consider, this, 1, 2);
    mapper().assign("/projects/bid_on/{1}/{2}");

    dispatcher().assign("/projects/(.+)", &WebSite::project, this, 1);
    mapper().assign("/projects/{1}");

    dispatcher().assign("/projects", &WebSite::projects, this);
    mapper().assign("/projects");

    dispatcher().assign("/", &WebSite::master, this);
    mapper().assign("/");
  };

  virtual void main(std::string path) {
    cppcms::application::main(path);
  }

  template<typename T>
  void add_menu(T& object) {
    object.page.description = "description";
    object.page.keywords = "keywords";
    if (session().is_set("username")) {
      object.page.current_username = session()["username"];
    }
    object.page.menuList.push_back(std::pair<std::string,std::string>("/","Main"));
    object.page.menuList.push_back(std::pair<std::string,std::string>("/projects","Projects"));
    object.page.menuList.push_back(std::pair<std::string,std::string>("/users/contractors","Contractors"));
    object.page.menuList.push_back(std::pair<std::string,std::string>("/signup","Sign up"));
    object.page.menuList.push_back(std::pair<std::string,std::string>("/login","Log in"));
    object.page.menuList.push_back(std::pair<std::string,std::string>("/logout","Log out"));
    if (session().is_set("role") && session()["role"] == "contractor") {
      object.page.menuList.push_back(std::pair<std::string, std::string>("/projects/add_project", "Add Project"));
    }
  }

  virtual void master() {
    Data::Master tmpl;
    add_menu(tmpl);


    if (session().is_set("username")) {
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
    }

    if (session().is_set("username")) {
      std::vector<std::pair<int, std::string>> notifications;

      std::string src = "dbname=";
      src += db_source;
      soci::session sql("sqlite3", src);

      int user_id;
      sql << "select id from users where username=:username", soci::use(session()["username"]), soci::into(user_id);

      soci::rowset<soci::row> nots = (sql.prepare << "select * from notifications where user_id=:user_id" , soci::use(user_id));
      for (auto &it : nots) {
        if (!it.get<int>("is_read")) {
          notifications.push_back({it.get<int>("id"), it.get<std::string>("description")});
        }
      }

      tmpl.page.notifications = notifications;

    }

    render("Master",tmpl);
  }

  virtual void signup() {
    Data::Signup sgn;
    add_menu(sgn);

    if (request().request_method() == "POST") {
      sgn.info.load(context());
      if (sgn.info.validate()) {
        if (sgn.info.role.selected_id() == "0") {
          Employee employee;
          employee.username = sgn.info.username.value();
          employee.email = sgn.info.email.value();
          employee.password = sgn.info.password.value();
          employee.sign_up();
        } else {
          Contractor contractor;
          contractor.username = sgn.info.username.value();
          contractor.email = sgn.info.email.value();
          contractor.password = sgn.info.password.value();
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
    Data::Signup sgn;
    add_menu(sgn);

    if (request().request_method() == "POST") {
      sgn.info.load(context());
      if (sgn.info.validate()) {
        if (sgn.info.role.selected_id() == "0") {
          Employee employee;
          employee.username = sgn.info.username.value();
          employee.email = sgn.info.email.value();
          employee.password = sgn.info.password.value();
          if (employee.log_in()) {
            session().set("username", sgn.info.username.value());
            session().set("email", sgn.info.email.value());
            session().set("password", sgn.info.password.value());
            session().set("role", (sgn.info.role.selected_id() == "0" ? "employee" : "contractor"));

            response().set_redirect_header("/");
            return;
          }
        } else {
          Contractor contractor;
          contractor.username = sgn.info.username.value();
          contractor.email = sgn.info.email.value();
          contractor.password = sgn.info.password.value();
          if (contractor.log_in()) {
            session().set("username", sgn.info.username.value());
            session().set("email", sgn.info.email.value());
            session().set("password", sgn.info.password.value());
            session().set("role", (sgn.info.role.selected_id() == "0" ? "employee" : "contractor"));
            response().set_redirect_header("/");
            return;
          }
        }
      }
    }

    render("Signup", sgn);
  }

  virtual void projects() {
    Data::Projects mn;

    add_menu(mn);

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    soci::rowset<Project> projects = (sql.prepare << "select * from projects");
    std::vector<Project> all_projects;
    for (auto it = projects.begin(); it != projects.end(); ++it) {
      all_projects.push_back(*it);
    }
    mn.projects_page.projects = all_projects;
    render("Projects", mn);
  }

  virtual void project(std::string id) {
    Data::SingleProject pr;

    add_menu(pr);

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
          pr.project_page.bids.push_back(bid);
        }
      } else {
        pr.project_page.is_employee = false;

        int contractor_id;
        sql << "select contractor_id from projects where id=:id", soci::use(id), soci::into(contractor_id);

        Contractor contractor;
        sql << "select * from users where role='contractor' and username=:username",
              soci::use(session()["username"]), soci::into(contractor);

        if (contractor_id == contractor.id) {
          soci::rowset<Bid> rs = (sql.prepare << "select * from bids where project_id = :id", soci::use(id));
          for (auto& bid : rs) {
            pr.project_page.bids.push_back(bid);
          }
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

  virtual void log_out() {
    session().erase("username");
    session().erase("email");
    session().erase("password");
    session().erase("role");
    response().set_redirect_header("/");
  }

  virtual void consider(std::string bid_id, std::string status) {
    if (session().is_set("role")) {
      if (session()["role"] == "employee") {
        response().set_redirect_header("/projects");
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
    add_menu(addpr);

    if (request().request_method() == "POST") {
      addpr.add_project_form.load(context());
      if (addpr.add_project_form.validate()) {
        if (session().is_set("role") && session()["role"] == "contractor") {
          Contractor contractor;
          Project project;
          project.name = addpr.add_project_form.name.value();

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
            contractor.add_project(project);
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

  virtual void contractors() {
    Data::Contractors cntr;

    add_menu(cntr);

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

  virtual void user(std::string id) {
    Data::SingleUser usr;

    add_menu(usr);

    std::string src = "dbname=";
    src += db_source;
    soci::session sql("sqlite3", src);
    User user;
    sql << "select * from users where id=:id", soci::use(id), soci::into(user);

    usr.user_page.user = user;

    if (session().is_set("username") && user.user_role == role::contractor) {
      User current_user;
      sql << "select * from users where username=:username",
          soci::use(session()["username"]), soci::into(current_user);

      int cnt;
      sql << "select count(*) from subscriptions where contractor_id=:contractor_id and user_id=:user_id",
          soci::use(id), soci::use(current_user.id), soci::into(cnt);

      if (cnt != 0) {
        usr.user_page.is_subscribed = true;
      } else {
        usr.user_page.is_subscribed = false;
      }
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