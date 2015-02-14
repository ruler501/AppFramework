#ifndef _URI_H_
#define _URI_H_

#include <map>

#include "curl/curl.h"

std::string url_encode(std::string value);

size_t write_data(void *ptr, size_t size, size_t nmemb, void* stream);

class HTTPRequest{
protected:
    std::map<std::string, std::string> uris;
    std::vector<std::string> cookies;
    std::string url;

public:
    std::string host;
    
    HTTPRequest(std::string path) : url(path) {}

    void addURI(std::string parameter, std::string value){
        uris[parameter] = value;
    }

    void addCookie(std::string cookie){
        cookies.push_back(cookie);
    }

    void sendRequest(const char* outfile){
        std::string temppath = url;
        if(!uris.empty()){
            temppath += '?';
            for(auto &a : uris){
                temppath += a.first;
                temppath += '=';
                temppath += a.second;
                temppath += '&';
            }
            temppath = temppath.substr(0, temppath.size()-1);
        }
        temppath = url_encode(temppath);
        CURL *curl;
        curl = curl_easy_init();
        if (curl) {
            for(auto& cookie : cookies){
                curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
            }
            FILE *fp = nullptr;
            curl_easy_setopt(curl, CURLOPT_URL, (temppath).c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            if(outfile){
                fp = fopen(outfile,"wb");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            }
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if(outfile) fclose(fp);
        }
    }
};

#endif // _URI_H_
