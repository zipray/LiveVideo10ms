
#ifndef FPV_VR_UDPRECEIVER_H
#define FPV_VR_UDPRECEIVER_H

#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <android/log.h>
#include <iostream>
#include <thread>
#include <atomic>

//Starts a new thread that continuously checks for new data on UDP port

class UDPReceiver {
public:
    typedef std::function<void(const uint8_t[],size_t)> DATA_CALLBACK;
    typedef std::function<void(const char*)> SOURCE_IP_CALLBACK;
public:
    //@param port : The port to listen on
    //@param CPUPriority: The priority the receiver thread will run with
    //@param buffsize: Too small values can result in packet loss
    //@param onDataReceivedCallback: called every time new data is received
    UDPReceiver(int port,const std::string& name,int CPUPriority,size_t buffsize,const DATA_CALLBACK& onDataReceivedCallback);
    //Register a callback that is called once and contains the IP address of the first received packet's sender
    void registerOnSourceIPfound(const SOURCE_IP_CALLBACK& onSourceIP);
    //Start receiver thread, open UDP port
    void startReceiving();
    //Close port, stop receiver thread, join receiver thread, delete receive thread
    void stopReceiving();
    long getNReceivedBytes()const;
    std::string getSourceIPAddress()const;
    int getPort()const;
private:
    void receiveFromUDPLoop();
    DATA_CALLBACK onDataReceivedCallback=nullptr;
    SOURCE_IP_CALLBACK onSourceIP= nullptr;
    const int mPort;
    const int mCPUPriority;
    const size_t RECV_BUFF_SIZE;
    const std::string mName;
    std::string mIP="0.0.0.0";
    std::atomic<bool> receiving;
    std::atomic<long> nReceivedBytes;
    int mSocket;
    std::unique_ptr<std::thread> mUDPReceiverThread;
};

#endif // FPV_VR_UDPRECEIVER_H