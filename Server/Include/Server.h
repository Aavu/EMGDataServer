//
// Created by Raghavasimhan Sankaranarayanan on 7/15/20.
//

#ifndef EMGDATASERVER_SERVER_H
#define EMGDATASERVER_SERVER_H

#include "TCPServer.h"
#include "CoAmp.h"
#include "pch.h"
#include "Time.h"
#include "ErrorDef.h"

class Server {
public:
    ~Server();
    static Server* getInstance();
    Error_t init(CoAmp* pSensor, int port=8080, int iPacketSize=128);
    Error_t start();
    Error_t stop();

private:
    Server();
    void threadCallback();
    static void signalHandler(int sig);

    TCPServer::data_t transmissionData;

    static Server* pInstance;
    CoAmp* m_pSensor = nullptr;

    TCPServer m_server;
    std::thread* m_pThread = nullptr;

    std::atomic<bool> m_bRunning;

    Data_t<uint16_t>* m_piDataChunk = nullptr;
    int m_iBlockSize    = 0;
    int m_iNumChannels  = 0;
};


#endif //EMGDATASERVER_SERVER_H
