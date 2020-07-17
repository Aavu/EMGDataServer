//
// Created by Raghavasimhan Sankaranarayanan on 7/15/20.
//

#include "Server.h"

Server* Server::pInstance = nullptr;

Server *Server::getInstance() {
    pInstance = new Server();
    return pInstance;
}

Error_t Server::init(CoAmp* pSensor, int port, int iPacketSize) {
    m_pSensor = pSensor;
    m_iBlockSize = iPacketSize;
    m_iNumChannels = CoAmp::NUM_EMG_CHANNELS;

    signal(SIGPIPE, Server::signalHandler);
    auto err = m_server.init(port);
    if (err != kNoError)
        return err;

    m_piDataChunk = new Data_t<uint16_t>(m_iBlockSize, m_iNumChannels);
    transmissionData.buffer = new uint16_t [m_iBlockSize * m_iNumChannels];
    return kNoError;
}

Error_t Server::start() {
    m_bRunning = true;
    auto err = m_server.start();
    if (err)
        return err;
    m_pThread =  new std::thread(&Server::threadCallback, this);
    return kNoError;
}

Server::~Server() {
    delete m_piDataChunk;
    delete [] transmissionData.buffer;
}

void Server::threadCallback() {
    LOG_TRACE("Starting Server thread...");
    while (m_bRunning) {
        if (!m_pSensor->isAvailable(m_iBlockSize))
            continue;

        auto err = m_pSensor->getSamples(transmissionData.buffer, m_iBlockSize);
        if (err != kNoError)
            break;

        strcpy(transmissionData.timeStamp, Time::getTimeStamp().c_str());
//        std::cout << transmissionData.timeStamp << std::endl;
        err = m_server.send(&transmissionData, m_iBlockSize*m_iNumChannels);
//        err = m_server.send(m_piDataChunk->getSamplesPointer(), m_iBlockSize*m_iNumChannels);
        if (err != kNoError)
            break;
    }
    LOG_TRACE("Terminating Prediction thread...");
}

Error_t Server::stop() {
    LOG_DEBUG("Stopping server.");
    m_bRunning = false;
    if (m_pThread == nullptr) {
        if (m_pThread->joinable())
            m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
    return kNoError;
}

void Server::signalHandler(int sig) {
    if (sig == SIGPIPE)
        Server::getInstance()->stop();
}

Server::Server() : m_bRunning(false) {

}
