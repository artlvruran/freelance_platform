#include <cppcms/applications_pool.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/application.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include <cppcms/application.h>
#include "data/tmpl_master.h"
#include "data/tmpl_signup.h"
#include "src/employee.h"
#include "src/contractor.h"

class WebSite : public cppcms::application {
 public:
  WebSite(cppcms::service& s) : cppcms::application(s) {
    dispatcher().assign("/signup", &WebSite::signup, this, 1);
    mapper().assign("signup");

    dispatcher().assign("/login", &WebSite::login, this, 1);
    mapper().assign("login");

    dispatcher().assign("(/?)", &WebSite::master, this, 1);
    mapper().assign("/");
  };

  virtual void main(std::string path) {
    cppcms::application::main(path);
  }

  virtual void master(std::string path) {
    Data::Master tmpl;
    tmpl.page.title = path;
    tmpl.page.description = "description";
    tmpl.page.keywords = "keywords";
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/","Main"));
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/signup","Sign Up"));
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/login","Log in"));
    render("Master",tmpl);
  }

  virtual void signup(std::string path) {
    Data::Signup sgn;
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
        master(path);
        return;
      }
    }

    render("Signup", sgn);
  }

  virtual void login(std::string path) {
    Data::Signup sgn;
    if (request().request_method() == "POST") {
      sgn.info.load(context());
      if (sgn.info.validate()) {
        if (sgn.info.role.selected_id() == "0") {
          Employee employee;
          employee.username = sgn.info.username.value();
          employee.email = sgn.info.email.value();
          employee.password = sgn.info.password.value();
          employee.sign_up();
          if (employee.log_in()) {
            session().set("username", sgn.info.username.value());
            session().set("email", sgn.info.email.value());
            session().set("password", sgn.info.password.value());
            session().set("role", (sgn.info.role.selected_id() == "0" ? "employee" : "contractor"));
            master(path);
            return;
          }
        } else {
          Contractor contractor;
          contractor.username = sgn.info.username.value();
          contractor.email = sgn.info.email.value();
          contractor.password = sgn.info.password.value();
          contractor.sign_up();
          if (contractor.log_in()) {
            session().set("username", sgn.info.username.value());
            session().set("email", sgn.info.email.value());
            session().set("password", sgn.info.password.value());
            session().set("role", (sgn.info.role.selected_id() == "0" ? "employee" : "contractor"));
            master(path);
            return;
          }
        }
      }
    }

    render("Signup", sgn);
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