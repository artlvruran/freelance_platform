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

    dispatcher().assign("/projects/bid_on/(.+)", &WebSite::bid_on, this, 1);
    mapper().assign("/projects/bid_on/{1}");

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

  virtual void master() {
    Data::Master tmpl;
    tmpl.page.description = "description";
    tmpl.page.keywords = "keywords";
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/","Main"));
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/signup","Sign Up"));
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/login","Log in"));
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/projects","Projects"));
    render("Master",tmpl);
  }

  virtual void signup() {
    Data::Signup sgn;
    sgn.page.description = "description";
    sgn.page.keywords = "keywords";
    sgn.page.menuList.insert(std::pair<std::string,std::string>("/","Main"));
    sgn.page.menuList.insert(std::pair<std::string,std::string>("/signup","Sign Up"));
    sgn.page.menuList.insert(std::pair<std::string,std::string>("/login","Log in"));
    sgn.page.menuList.insert(std::pair<std::string,std::string>("/projects","Projects"));

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
    sgn.page.description = "description";
    sgn.page.keywords = "keywords";
    sgn.page.menuList.insert(std::pair<std::string,std::string>("/","Main"));
    sgn.page.menuList.insert(std::pair<std::string,std::string>("/signup","Sign Up"));
    sgn.page.menuList.insert(std::pair<std::string,std::string>("/login","Log in"));
    sgn.page.menuList.insert(std::pair<std::string,std::string>("/projects","Projects"));

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

    mn.page.description = "description";
    mn.page.keywords = "keywords";
    mn.page.menuList.insert(std::pair<std::string,std::string>("/","Main"));
    mn.page.menuList.insert(std::pair<std::string,std::string>("/signup","Sign Up"));
    mn.page.menuList.insert(std::pair<std::string,std::string>("/login","Log in"));
    mn.page.menuList.insert(std::pair<std::string,std::string>("/projects","Projects"));

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

    pr.page.description = "description";
    pr.page.keywords = "keywords";
    pr.page.menuList.insert(std::pair<std::string,std::string>("/","Main"));
    pr.page.menuList.insert(std::pair<std::string,std::string>("/signup","Sign Up"));
    pr.page.menuList.insert(std::pair<std::string,std::string>("/login","Log in"));
    pr.page.menuList.insert(std::pair<std::string,std::string>("/projects","Projects"));

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
        int bid_id;
        sql << "select id from bids where project_id=:project_id and employee_id=("
               "select id from users where role='employee' and username=:username"
               ")",
            soci::use(id), soci::use(session()["username"]), soci::into(id, ind);
        if (sql.got_data() && ind == soci::i_ok) {
          pr.project_page.is_bid_created = true;
        }
      } else {
        pr.project_page.is_employee = false;
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