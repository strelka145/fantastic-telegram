#include <unistd.h>
#include <iostream>
#include <string>
#include <ext/stdio_filebuf.h>
#include "headers/getinfo.h"
#include <cstdlib>
#include <getopt.h>
#include <iterator>
#include <vector>


#define OPTSTRING "u::i::tymdjpc:wafr"

int main(int argc, char* argv[]){

  struct std::vector<option_arg> option_list= {
      {"url",       2,      NULL,         'u',      "-u [argument] or --url=[argument] to obtain the paper information of the specified URL. (If no argument is specified, the URL obtained from DOI is displayed.)"},
      {"doi",       2,      NULL,         'i',      "-i [argument] or --doi=[argument] to obtain the paper information of the specified DOI. (If no argument is specified, the DOI obtained from URL is displayed.)"},
      {"abstract",  0,      NULL,         'a',      "Display abstract."},
      {"first",     0,      NULL,         'f',      "Display first author."},
      {"author",    0,      NULL,         'w',      "Display all authors."},
      {"title",     0,      NULL,         't',      "Display title."},
      {"year",      0,      NULL,         'y',      "Display the year of publication."},
      {"month",     0,      NULL,         'm',      "Display the month of publication."},
      {"day",       0,      NULL,         'd',      "Display the day of publication."},
      {"journal",   0,      NULL,         'j',      "Display the journal name."},
      {"publisher", 0,      NULL,         'p',      "Display publishers."},
      {"citation",  1,      NULL,         'c',      "Create a citation in the specified format."},
      {"reference", 0,      NULL,         'r',      "List the DOIs of the references. References for which DOIs cannot be obtained will be skipped."},
      {NULL,        0,      NULL,           0,      ""}
  };
  struct std::vector<option> ex_discription;
  for (option_arg item : option_list){ex_discription.push_back({item.name,item.has_arg,item.flag,item.val});}
  struct option *long_options=ex_discription.data();


  int c, option_index;
  get_info paper;
  std::vector<char> output_list;
  while (true){
    option_index = 0;
    if ((c = getopt_long(argc, argv, OPTSTRING, long_options,&option_index)) == -1){
      break;
    }
    output_list.push_back(c);
    switch (c){
      case 'u':
        if (optarg){
          paper.get_doi(optarg);
          paper.get_from_doi(paper.doi);
          output_list.erase(output_list.end() - 1);
        }
        break;

      case 'i':
        if(optarg){
          paper.doi=optarg;
          paper.get_from_doi(paper.doi);
          output_list.erase(output_list.end() - 1);
        }
        break;
      case '?':
        std::cout << "Either -u(--url=) or -i(--doi=) must have a value." << std::endl<< std::endl;
        for (option_arg item_option : option_list){
          if(item_option.name){
            std::cout << "\t-" << (char)item_option.val << "(--"<<item_option.name<<")"<< std::endl;
            std::cout << "\t" << item_option.description<< std::endl<< std::endl;
          }
        }
        break;

    }
  }

  for(char print_option : output_list){
    switch (print_option) {
      case 'u':
        paper.get_url();
        std::cout << paper.url << '\n';
        break;
      case 'i':
        std::cout << paper.doi << '\n';
        break;
      case 'a':
        paper.get_abstract();
        std::cout << paper.abstract << '\n';
        break;
      case 'f':
        paper.get_author();
        std::cout << paper.first << '\n';
        break;
      case 'w':
        paper.get_author();
        for(int i=0;i<paper.author_given_name.size();i++){
          std::cout << paper.author_given_name[i]<<" "<<paper.author_family_name[i] << '\n';
        }
        break;
      case 't':
        paper.get_title();
        std::cout << paper.title << '\n';
        break;
      case 'y':
        paper.get_date();
        std::cout << paper.year << '\n';
        break;
      case 'm':
        paper.get_date();
        std::cout << paper.month << '\n';
        break;
      case 'd':
        paper.get_date();
        std::cout << paper.day << '\n';
        break;
      case 'j':
        paper.get_journal();
        std::cout << paper.journal << '\n';
        break;
      case 'p':
        paper.get_publisher();
        std::cout << paper.publisher << '\n';
        break;
      case 'c':
        break;
      case 'r':
        paper.get_reference();
        for(std::string reference:paper.reference){
          std::cout << reference << '\n';
        }
        break;
    }
  }

}
