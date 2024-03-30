#include <cppcms/applications_pool.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/application.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include "data/tmpl_master.h"
#include "data/tmpl_signup.h"

class WebSite : public cppcms::application {
 public:
  WebSite(cppcms::service& s) : cppcms::application(s) {
    dispatcher().assign("/signup", &WebSite::signup, this, 1);
    mapper().assign("signup");

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
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/login","Log in"));\
    render("Master",tmpl);
  }

  virtual void signup(std::string path) {
    Data::Signup sgn;
    if (request().request_method() == "POST") {
      sgn.info.load(context());
      if (sgn.info.validate()) {
        sgn.username = sgn.info.username.value();
        sgn.email = sgn.info.email.value();
        sgn.password = sgn.info.password.value();
        sgn.info.clear();
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