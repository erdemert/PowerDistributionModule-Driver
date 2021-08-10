# PowerDistributionModule-Driver
Driver for the microprocessor HW-584.

Power Distribution Module Driver
This project is coded to drive a 16 port network relay switch, HW-584. Click
[here](http://myosuploads3.banggood.com/products/20190218/20190218204717protocol.pdf) to
see the documentation of the switch.
As a beginner tip, HW-584 is a network control switch with only ethernet port. Therefore, all the
coding is done on server requests. Spesifically on GET requests.
Dependencies
Make sure that CURL library and ROS are installed. The project is developed with ROS Melodic.
Project also may require you to reprogram the switch from
[this](https://github.com/nielsonm236/NetMod-ServerApp) link. During the development phase,
we observed that default program of the switch is neither stable enough nor sufficient.
Usage
You need to build the project at least once to control the network control switch. Create a
workspace and put src folder in. Then, go to the workspace and type "catkin_make". That's all,
you are ready to use the driver. Do not forget to start the roscore, simply type "roscore". Run the
project with the command "rosrun". You can even send commands from terminal typing
"rosservice call"!
You can use the switch with both labels and relay IDs. Two services are defined and located under
srv folder. They do the same job as labels are converted into IDs in the
relayControlCallbackWlabel function. You can turn on and off the relays by simply changing the
write_state. As stated in the service files, action 1 is READ whereas action 2 is WRITE. Note that if
you would like to read the current status of a pin, the program would not care the write_state at
all. In the read case, it must be either 1 or 0. Otherwise the program would return an error code.
The program would returns success status and the relay status. If you WRITE a pin, the program
returns the write_state you typed. In case of any type of error, the program returns a relay_state
of -1.
You can assing labels to the relay IDs at config/relay_map.yaml. You can also change the default
URL and port provided by me from launch/relay_map_launch.launch. Note that as a default URL is
192.168.1.4 and port is 80.
Example usage with relay ID : rosservice call /pdm_driver_node/relay_control_wid "relay_id: 4
action: 2
write_state: 1"
Example usage with relay label : rosservice call /pdm_driver_node/relay_control_wlabel
"relay_label: 'LIDAR_TOP'
action: 1
write_state: 65"
Note that since action 1 is READ, write_state won't matter for the program. 
38
Structure
In the main function a PdmDriverNode class object is created. This class calls the related callback
function according to the requested service. Both callback functions call the main callback
function, relayControlCallback. In this function, a HW584_Curl class object is created. HW584_Curl
implements an interface called HW584_Interface. This interface contains public functions such as
init, send_read_request and send_write_request. Note that if you need to avoid using CURL
library, you can reimplement HW584_Interface class. If you do so, you need to change the private
member of the PdmDriver class, HW584_Curl hw584_curl_obj to your own object.
Bugs
We could not observe too many bugs but a small issue. Time to time if switch cannot draw
enough power to perform the command, it may restart itself. That may cause your program to
stop responding. This is a physical problem more than a bug. Make sure that all the connections
are perfect and the switch is receiving 5V supply. My idea to resolve the issue is to just simply
add a timeout to the code. If the program cannot return a success or error in a certain time, it
could return a timeout error.
