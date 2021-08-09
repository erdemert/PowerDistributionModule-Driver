#include "pdm_driver/pdm_driver_node.hpp"



int main(int argc, char **argv)
{   
  ROS_INFO("OPERATION STARTING");
  ros::init(argc, argv, "pdm_driver_node");

  ros::NodeHandle nh("~");
  PdmDriverNode pdm_driver_instance(nh);
  
  ros::spin();
  
  curl_global_cleanup();
  return 0;
}