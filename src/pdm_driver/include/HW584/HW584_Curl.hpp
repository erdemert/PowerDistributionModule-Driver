#ifndef HW584_Curl_hpp
#define HW584_Curl_hpp

#include "HW584/pdm_driver_interface.hpp"

class HW584_Curl : public HW584_Interface{
    
    private : 
    char curl_read_buffer[16];
    public : 
    void init(){
        curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    }

    bool send_read_request(char &buffer, int relay_id, std::string url, std::string port){
        CURL* curl_ = curl_easy_init();
        url = url + ":";
        url = url + port;
        url = url + "/99";

      
            if (CURLE_OK != curl_easy_setopt(curl_, CURLOPT_URL, url.c_str()))
        {
            ROS_ERROR("Failed to connect server.");
            return 0;
        }

        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, static_cast<size_t ( *)(char*, size_t, size_t, void*)>(
        [](char* buf, size_t size, size_t nmemb, void* resultBody)
        {
        auto p = static_cast<HW584_Curl *>(resultBody);
        memcpy(p->curl_read_buffer, buf, 17);
        
        return size * nmemb;
        }));


        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, this);
        
        CURLcode perf_res = curl_easy_perform(curl_);
    
    
        if (perf_res != CURLE_PARTIAL_FILE)
        {
            if (perf_res != CURLE_OK)
            {
                ROS_ERROR("Failed to connect server at the perform stage.");
                return 0;
            }
        }

        curl_easy_cleanup(curl_);
        buffer = (int)curl_read_buffer[16 - relay_id] - 48;
        return 1;
    }

    virtual bool send_write_request(std::string path, std::string url, std::string port){
        CURL* curl_ = curl_easy_init();
        url = url + ":";
        url = url + port;
        url = url + path;
       
        if (CURLE_OK != curl_easy_setopt(curl_, CURLOPT_URL, url.c_str()))
        {
            ROS_ERROR("Failed to connect server.");
            return 0;
        }

        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, +[](char *buf, size_t size, size_t nmemb, void *up) -> size_t {
            //do nothing, we are not interested in reading this page.
        return size * nmemb;
        });

        CURLcode perf_res = curl_easy_perform(curl_);
    
    
        if (perf_res != CURLE_PARTIAL_FILE)
        {
            if (perf_res != CURLE_OK)
            {
                ROS_ERROR("Failed to connect server at the perform stage.");
                return 0;
            }
        }

    
        curl_easy_cleanup(curl_);
    
        return 1;


    }



};

#endif