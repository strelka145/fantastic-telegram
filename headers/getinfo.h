#ifndef GET_INFO
#define GET_INFO
#include <string>
#include <vector>
#include "headers/include/rapidjson/document.h"
struct get_info
{
    std::string url;
    std::string doi;
    std::string abstract;
    std::string title;
    int year;
    int month;
    int day;
    std::string journal;
    std::string publisher;
    std::string first;
    //std::string citation;
    std::vector<std::string> author_given_name;
    std::vector<std::string> author_family_name;
    std::vector<std::string> reference;

    void get_doi(std::string input_url);
    void get_url();
    void get_abstract();
    void get_title();
    void get_date();
    void get_journal();
    void get_publisher();
    void get_author();
    void get_reference();
    void get_from_doi(std::string input_doi);
    std::string get_citation();

    private:
      Document paper_info;
      std::string html;


};

struct option_arg {
  const char *name;
  int has_arg;
  int *flag;
  int val;
  std::string description;
};

#endif
