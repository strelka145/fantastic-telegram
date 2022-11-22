#include "headers/getinfo.h"
#include "headers/include/rapidjson/document.h"
#include <string>
#include <vector>
#include <iostream>
#include <ext/stdio_filebuf.h>
#include <cstdlib>
#include <curl/curl.h>
#include <regex>

using namespace rapidjson;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

std::string http_get(std::string url,std::string header_option=""){
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  curl = curl_easy_init();
  if(curl) {
    if(header_option!=""){
      struct curl_slist *slist1;
      slist1 = NULL;
      slist1 = curl_slist_append(slist1, header_option.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
    //Elsevier is evil and won't let me simply curl to get meta data.So I set the UA as firefox.
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:106.0) Gecko/20100101 Firefox/106.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return readBuffer;
  }
  return std::string("");
}

void get_info::get_doi(std::string input_url){
  std::string line,doi_sep="";
  std::cmatch regex_temp;

  html=http_get(input_url);
  html=std::regex_replace(html, std::regex(">\\s*<"), ">\r\n<");//Search one line at a time with appropriate line breaks to prevent segmentation fault.
  std::istringstream f(html);
  std::regex regx(R"(name\s*=\s*\"(citation_doi|dc.identifier)\")", std::regex::icase);
  while (std::getline(f, line)) {
    if (std::regex_search(line.c_str(), regex_temp, regx)) {
      std::smatch match;
      if(std::regex_search(line, match, std::regex(R"(content\s*=\s*\".+\")"))){
        doi_sep=match[0].str();
      }
      doi = std::regex_replace(doi_sep.c_str(), std::regex(R"(content\s*=\s*\")"), "");
      doi.pop_back();
      break;
    }
  }
  if(doi==""){
    std::cerr << "Failed to retrieve DOI, please specify DOI directly instead of URL." << '\n';
    exit(1);
  }
}

void get_info::get_from_doi(std::string input_doi){
  std::string doi_json,line;
  std::cmatch regex_temp;
  doi_json=http_get("https://api.crossref.org/works/"+doi);
  paper_info.Parse(doi_json.c_str());
}

void get_info::get_url(){
  if (paper_info["message"].HasMember("URL")){
     url=paper_info["message"]["URL"].GetString();
  }
}

void get_info::get_abstract(){
  std::string line;
  std::cmatch regex_temp;
  if (paper_info["message"].HasMember("abstract")){
     abstract=paper_info["message"]["abstract"].GetString();
  }else {
    get_url();
    get_doi(url);
    std::istringstream f(html);
    while (std::getline(f, line)) {
      if (std::regex_search(line.c_str(), regex_temp, std::regex(R"(<\s*meta\s+name\s*=\s*\"(citation_abstract|dc.description)\"\s*content\s*=\s*"(.+)\")"))) {
        abstract = std::regex_replace(regex_temp.str(), std::regex(R"(<\s*meta\s+name\s*=\s*\"(citation_abstract|dc.description)\"\s*content\s*=\s*")"), "");
        abstract.pop_back();
        break;

      }
    }
  }
}


void get_info::get_title(){
  if (paper_info["message"].HasMember("title")){
    Value& title_list = paper_info["message"]["title"];
    title=title_list[title_list.Size()-1].GetString();
  }
}

void get_info::get_date(){
  if (paper_info["message"].HasMember("published")&&paper_info["message"]["published"].HasMember("date-parts")){
    Value& date_list = paper_info["message"]["published"]["date-parts"];
    year =date_list[date_list.Size()-1][0].GetInt();
    if(date_list[date_list.Size()-1].Size()>1)month=date_list[date_list.Size()-1][1].GetInt();
    if(date_list[date_list.Size()-1].Size()>2)day  =date_list[date_list.Size()-1][2].GetInt();
  }
}

void get_info::get_journal(){
  if (paper_info["message"].HasMember("container-title")){
    Value& container_title_list = paper_info["message"]["container-title"];
    if(container_title_list.Size()>0){
      journal=container_title_list[container_title_list.Size()-1].GetString();
    }
  }
}

void get_info::get_publisher(){
  if (paper_info["message"].HasMember("publisher")){
     publisher=paper_info["message"]["publisher"].GetString();
  }
}

void get_info::get_reference(){
  if (paper_info["message"].HasMember("reference")){
     Value& reference_list = paper_info["message"]["reference"];
     for(int i=0;i<reference_list.Size();i++){
       if(reference_list[i].HasMember("DOI"))reference.push_back(reference_list[i]["DOI"].GetString());
     }
  }
}

void get_info::get_author(){
  if (paper_info["message"].HasMember("author")){
     Value& author_list = paper_info["message"]["author"];
     for(int i=0;i<author_list.Size();i++){
       author_given_name.push_back(author_list[i]["given"].GetString());
       author_family_name.push_back(author_list[i]["family"].GetString());
       if(author_list[i]["sequence"].GetString()=="first"){
         first=author_list[i]["given"].GetString()+std::string(" ")+author_list[i]["family"].GetString();
       }
     }
  }
}

void get_info::get_citation(std::string style){
  get_url();
  citation=http_get(url,"Accept: text/x-bibliography; style="+style);
}
