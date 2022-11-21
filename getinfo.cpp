#include "headers/getinfo.h"
#include "headers/include/rapidjson/document.h"
#include <string>
#include <vector>
#include <iostream>
#include <ext/stdio_filebuf.h>
#include <cstdlib>
#include <regex>

using namespace rapidjson;

std::string start_com(std::string cmd){
  std::string ans="";
  FILE *fp = popen(cmd.c_str(), "r");
  __gnu_cxx::stdio_filebuf<char> *p_fb = new __gnu_cxx::stdio_filebuf<char>(fp, std::ios_base::in);
  std::istream input(static_cast<std::streambuf *>(p_fb));
  std::string buffer;
  while(getline(input, buffer)){
    ans+=buffer;
  }
  delete p_fb;
  pclose(fp);
  return ans;
}

void get_info::get_doi(std::string input_url){
  std::string cmd = "curl -s -L -A \"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:106.0) Gecko/20100101 Firefox/106.0\" \""+input_url+"\"",line_;
  std::cmatch regex_temp;
  std::smatch match;
  html=start_com(cmd);
  html=std::regex_replace(html, std::regex(">\\s*<"), ">\r\n<");//Search one line at a time with appropriate line breaks to prevent segmentation fault.
  std::istringstream f(html);
  while (std::getline(f, line_)) {
    if (std::regex_search(line_.c_str(), regex_temp, std::regex(R"(<\s*meta\s+name\s*=\s*\"(citation_doi|dc.identifier)\"\s*content\s*=\s*"(.+)\")"))) {
      doi = std::regex_replace(regex_temp.str(), std::regex(R"(<\s*meta\s+name\s*=\s*\"(citation_doi|dc.identifier)\"\s*content\s*=\s*")"), "");
      doi.pop_back();
      break;
    }
  }
}

void get_info::get_from_doi(std::string input_doi){
  std::string doi_json,line;
  std::cmatch regex_temp;
  doi_json=start_com("curl -s -L \"https://api.crossref.org/works/"+doi+"\"");
  Document paper_info;
  paper_info.Parse(doi_json.c_str());

  //get abstract
  if (paper_info["message"].HasMember("abstract")){
     abstract=paper_info["message"]["abstract"].GetString();
  }else{
    std::istringstream f(html);
    while (std::getline(f, line)) {
      if (std::regex_search(line.c_str(), regex_temp, std::regex(R"(<\s*meta\s+name\s*=\s*\"(citation_abstract|dc.description)\"\s*content\s*=\s*"(.+)\")"))) {
        abstract = std::regex_replace(regex_temp.str(), std::regex(R"(<\s*meta\s+name\s*=\s*\"(citation_abstract|dc.description)\"\s*content\s*=\s*")"), "");
        abstract.pop_back();
        break;

      }
    }
  }

  //get URL
  if (paper_info["message"].HasMember("URL")){
     url=paper_info["message"]["URL"].GetString();
  }

  //get title
  if (paper_info["message"].HasMember("title")){
    Value& title_list = paper_info["message"]["title"];
    title=title_list[title_list.Size()-1].GetString();
  }

  //get date
  if (paper_info["message"].HasMember("published")&&paper_info["message"]["published"].HasMember("date-parts")){
    Value& date_list = paper_info["message"]["published"]["date-parts"];
    year =date_list[date_list.Size()-1][0].GetInt();
    if(date_list[date_list.Size()-1].Size()>1)month=date_list[date_list.Size()-1][1].GetInt();
    if(date_list[date_list.Size()-1].Size()>2)day  =date_list[date_list.Size()-1][2].GetInt();
  }

  //get journal
  if (paper_info["message"].HasMember("container-title")){
    Value& container_title_list = paper_info["message"]["container-title"];
    if(container_title_list.Size()>0){
      journal=container_title_list[container_title_list.Size()-1].GetString();
    }
  }

  //get publisher
  if (paper_info["message"].HasMember("publisher")){
     publisher=paper_info["message"]["publisher"].GetString();
  }

  //get reference list
  if (paper_info["message"].HasMember("reference")){
     Value& reference_list = paper_info["message"]["reference"];
     for(int i=0;i<reference_list.Size();i++){
       if(reference_list[i].HasMember("DOI"))reference.push_back(reference_list[i]["DOI"].GetString());
     }
  }


  //get author
  if (paper_info["message"].HasMember("author")){
     Value& author_list = paper_info["message"]["author"];
     for(int i=0;i<author_list.Size();i++){
       author.push_back(author_list[i]["given"].GetString()+std::string(" ")+author_list[i]["family"].GetString());
       if(author_list[i]["sequence"].GetString()=="first"){
         first=author_list[i]["given"].GetString()+std::string(" ")+author_list[i]["family"].GetString();
       }
     }
  }

}
