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
    m_iTransmissionData = new uint16_t [(m_iBlockSize + 1) * m_iNumChannels];
    return kNoError;
}

Error_t Server::run(int iTimeoutInSec) {
    m_bRunning = true;
    auto err = m_server.start();
    if (err)
        return err;
    m_pThread =  new std::thread(&Server::threadCallback, this);
    m_pReceiveThread =  new std::thread(&Server::receiveCallback, this);
    std::unique_lock<std::mutex> lock(m_mtx);
    if(iTimeoutInSec < 0) {
        m_cv.wait(lock, [this] {return !isRunning();});
    } else {
        m_cv.wait_for(lock, std::chrono::seconds(iTimeoutInSec), [this] {return !isRunning();});
    }

    if (m_bRunning)
        stop();

    joinThreads();
    return kNoError;
}

Server::~Server() {
    delete m_piDataChunk;
    delete [] m_iTransmissionData;
}

void Server::threadCallback() {
    LOG_TRACE("Starting Server thread...");
    while (m_bRunning) {
        if (!m_pSensor->isAvailable(m_iBlockSize))
            continue;

        Time::getTimeStamp(m_iTransmissionData);
        auto offset = m_iNumChannels;
        auto err = m_pSensor->getSamples(&m_iTransmissionData[offset], m_iBlockSize);
        if (err != kNoError)
            break;

        err = m_server.send(m_iTransmissionData, (m_iBlockSize + 1)*m_iNumChannels);
//        err = m_server.send(m_piDataChunk->getSamplesPointer(), m_iBlockSize*m_iNumChannels);
        if (err != kNoError)
            break;
    }
    LOG_TRACE("Terminating Prediction thread...");
}

Error_t Server::stop() {
    LOG_DEBUG("Stopping server.");
    m_bRunning = false;
    return kNoError;
}

void Server::signalHandler(int sig) {
    if (sig == SIGPIPE)
        Server::getInstance()->stop();
}

Server::Server() : m_bRunning(false) {

}

void Server::receiveCallback() {
    while (m_bRunning) {
        auto err = m_server.receive(&m_iReceivedMsg);
        if (err != kNoError)
            break;

        if (m_iReceivedMsg == '1') {
//            std::cout << m_iReceivedMsg << std::endl;
            break;
        }
    }
    stop();
    m_cv.notify_one();
}

bool Server::isRunning() {
    return m_bRunning;
}

void Server::joinThreads() {
    if (m_pThread) {
        if (m_pThread->joinable())
            m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
    if (m_pReceiveThread) {
        if (m_pReceiveThread->joinable()) {
            m_pReceiveThread->join();
        }
        delete m_pReceiveThread;
        m_pReceiveThread = nullptr;
    }
}
