#include <cppcms/applications_pool.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/application.h>
#include <cppcms/url_mapper.h>
#include <cppcms/service.h>
#include "data/tmpl_master.h"

class WebSite : public cppcms::application {
 public:
  WebSite(cppcms::service& s) : cppcms::application(s) {};

  virtual void main(std::string path) {
    Data::Master tmpl;
    tmpl.page.title = path;
    tmpl.page.description = "description";
    tmpl.page.keywords = "keywords";
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/","MAIN"));
    tmpl.page.menuList.insert(std::pair<std::string,std::string>("/else","ELSE"));
    render("Master",tmpl);
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