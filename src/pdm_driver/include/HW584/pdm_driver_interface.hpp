#ifndef pdm_driver_interface_hpp
#define pdm_driver_interface_hpp

// content of the header file



class HW584_Interface {
    public :
    virtual void init() = 0;
    virtual bool send_read_request(char &buffer, int relay_id, std::string url, std::string port) = 0;
    virtual bool send_write_request(std::string path, std::string url, std::string port) = 0;
    
    

};

#endif

