#include "pdm_driver/pdm_driver_node.hpp"
#include "HW584/HW584_Curl.hpp"



PdmDriverNode::PdmDriverNode(const ros::NodeHandle &node_handle)
    : nh_(node_handle)
{
    this->init();
}

void PdmDriverNode::init()
{   
    nh_.getParam("relay_map", relay_map);
    nh_.getParam("url", url_);
    nh_.getParam("port", port_);
    
    relay_control_service1_ =  nh_.advertiseService("relay_control_wid", &PdmDriverNode::relayControlCallbackWid, this);
    relay_control_service2_ =  nh_.advertiseService("relay_control_wlabel", &PdmDriverNode::relayControlCallbackWlabel, this);

    // Check the content of the map for debugging purposes only
    for (auto elem : relay_map)
    {
    std::cout << elem.first << " " << elem.second << "\n";
    }

    HW584_Curl hw584_curl_obj;

    

}
bool PdmDriverNode::relayControlCallbackWlabel(pdm_driver::relay_control_wlabel::Request &req,
                                  pdm_driver::relay_control_wlabel::Response &res)
{   
    int relay_id;
    std::string relay_label = req.relay_label;
    if (relay_map.find(relay_label) != relay_map.end())
        {
        relay_id = relay_map.find(relay_label)->second;
        }
    else
        {
        res.relay_state = -1;
        res.success = 0;
        ROS_ERROR("There is no relay such!");    
        return 0;
        }
        int relay_state = this->relayControlCallback(relay_id, req.action, req.write_state);
        
        if(relay_state == -1){
            res.success = 0;
            res.relay_state = -1;
            ROS_ERROR("Callback function returned an error code!");
            return 0;
        }

        else if(relay_state == 0 || relay_state == 1){
            res.success = 1;
            res.relay_state = relay_state;
            return 1;
        }

        else{
            ROS_ERROR("Even i do not know what happened?");
            res.success = 0;
            res.relay_state = -99;
            return 0;
        }
    



}
//With this new function, there is a slight change. If any error occures this function returns -1, 
//normally it is expected to return relay state.
int PdmDriverNode::relayControlCallback(int relay_id, int action, int write_state){
    
    if (relay_id > 16 || relay_id < 1)
    {
        action = -99;
    }
    
    switch (action)
    {
    case 1: // Read
    {   char buffer;
        ROS_INFO("Identified 1 - read");
       
        if (!hw584_curl_obj.send_read_request(buffer, relay_id, url_, port_))
        {
            
            ROS_ERROR("Failed to connect server.");
            return -1;
            
        }

        
        

        return buffer;
        

        break;
    }

    case 2: // Write
    {
        ROS_INFO("Identified 2 - write");
        int port = 0;
        if (write_state != 0 && write_state != 1)
        {
            ROS_ERROR("Invalid writestate!");
            return -1;
        }
        if (write_state == 1)
        {
            port = 2 * relay_id - 1;
        }
        else if (write_state == 0)
        {
            port = 2 * relay_id - 2;
        }
        std::string str_port = std::to_string(port);

        if (str_port.length() == 1)
        {
            str_port = "0" + str_port;
        }

        str_port = "/" + str_port;

        

        if (!hw584_curl_obj.send_write_request(str_port, url_, port_))
        {   
            ROS_ERROR("Failed to connect server.");
            return -1;
            
        }
        
        
        return write_state;
        
        break;
    }

    default:
    {
        

        ROS_ERROR("You either typed an invalid relay ID or action." );
        return -1;
        break;
    }

        return 1;
    }

}

bool PdmDriverNode::relayControlCallbackWid(pdm_driver::relay_control::Request &req,
                                  pdm_driver::relay_control::Response &res)
{
    
   
    
        int relay_state = this->relayControlCallback(req.relay_id, req.action, req.write_state);
        
        if(relay_state == -1){
            res.success = 0;
            res.relay_state = -1;
            ROS_ERROR("Callback function returned an error code!");
            return 0;
        }

        else if(relay_state == 0 || relay_state == 1){
            res.success = 1;
            res.relay_state = relay_state;
            return 1;
        }

        else{
            ROS_ERROR("Even i do not know what happened?");
            res.success = 0;
            res.relay_state = -99;
            return 0;
        }
    
    
}
