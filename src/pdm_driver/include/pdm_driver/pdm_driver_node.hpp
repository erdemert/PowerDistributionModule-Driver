
#include <string>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <boost/thread.hpp>
#include "curl/curl.h"
#include "pdm_driver/relay_control.h"
#include "pdm_driver/relay_control_wlabel.h"
#include "HW584/HW584_Curl.hpp"

#ifndef pdm_driver_node_hpp
#define pdm_driver_node_hpp

class PdmDriverNode {
 
  private:
    std::map<std::string, int> relay_map;
    std::string url_;
    std::string port_;
    ros::NodeHandle nh_;
    std::string node_name_{"simple class node"};
    HW584_Curl hw584_curl_obj; 
    std::string buffer; 
    ros::ServiceServer relay_control_service1_; 
    ros::ServiceServer relay_control_service2_; 
 
 public:
    PdmDriverNode(const ros::NodeHandle &node_handle);
    
    ~PdmDriverNode() = default;
    
    void init();
    
    bool relayControlCallbackWid(pdm_driver::relay_control::Request  &req,
         pdm_driver::relay_control::Response &res);

    bool relayControlCallbackWlabel(pdm_driver::relay_control_wlabel::Request  &req,
         pdm_driver::relay_control_wlabel::Response &res);
    
    static size_t readCallbackHelper(char* buf, size_t size, size_t nmemb, void* up);
    
    bool connectServer(std::string path);

    int relayControlCallback(int relay_id, int action, int write_state);
    

}; 

#endif



  