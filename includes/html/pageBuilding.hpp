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
std::vector<unsigned char> buildErrorPage(http::Status error);
std::vector<unsigned char> buildAutoindexPage(const ft::filesystem::path& path);
std::vector<unsigned char> buildSimplePage(std::string content);

}; /* namespace html */

#endif /* PAGE_BUILDING_HPP */