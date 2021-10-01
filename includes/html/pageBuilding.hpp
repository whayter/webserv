#ifndef PAGE_BUILDING_HPP
#define PAGE_BUILDING_HPP

#include "HtmlBuilder.hpp"
#include "ReturnDirective.hpp"
#include "Status.hpp"
#include "utility.hpp"

namespace html {

Builder link(std::string rel, std::string type, std::string href);
Builder a(std::string content, std::string href);
std::vector<unsigned char> buildRedirectionPage(const ReturnDirective &rdir);
std::string buildErrorPage(http::Status error);
std::string buildAutoindexPage(const ft::filesystem::path& path);

}; /* namespace html */

#endif /* PAGE_BUILDING_HPP */