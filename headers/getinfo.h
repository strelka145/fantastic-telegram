#ifndef GET_INFO
#define GET_INFO
#include <string>
#include <vector>
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
    std::vector<std::string> author;
    std::vector<std::string> reference;

    void get_doi(std::string input_url);
    void get_from_doi(std::string input_doi);

    private:
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
