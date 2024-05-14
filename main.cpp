#include <cppcms/applications_pool.h>
#include <cppcms/application.h>
#include <cppcms/service.h>

#include "data/tmpl_master.h"
#include "data/tmpl_signup.h"
#include "data/tmpl_projects.h"
#include "data/tmpl_add_project.h"
#include "data/tmpl_notifications.h"
#include "data/tmpl_main.h"

#include "src/models/employee.h"
#include "src/models/contractor.h"
#include "src/models/constants.h"

#include "src/apps/chat_app.h"
#include "src/apps/users_app.h"
#include "src/apps/projects_app.h"

#include "src/utils/utils.h"


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

      DataBase db(db_source);

      int user_id;
      db << "select id from users where username=:username", soci::use(session()["username"]), soci::into(user_id);

      int not_user_id;
      db << "select user_id from notifications where id=:id", soci::use(notification_id), soci::into(not_user_id);

      if (user_id == not_user_id) {
        User::read(notification_id);
      }
      response().set_redirect_header("/notifications");
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