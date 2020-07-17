//
// Created by Raghavasimhan Sankaranarayanan on 7/15/20.
//

#include "TCPServer.h"

Error_t TCPServer::init(int port) {
    m_iPort = port;

    bzero(&m_serverAddress, sizeof(m_serverAddress));
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    m_serverAddress.sin_port = htons(m_iPort);

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd == -1) {
        LOG_ERROR("socket creation failed...");
        return kUnknownError;
    }
    LOG_DEBUG("Socket successfully created.");

    auto err = setSocketOptions();
    if (err != kNoError) {
        return err;
    }

    auto iErr = bind(m_sockfd, (sockaddr*)&m_serverAddress, sizeof(m_serverAddress));
    if (iErr != 0) {
        LOG_ERROR("socket bind failed... Err: {}", err);
        return kUnknownError;
    }
    LOG_DEBUG("Socket successfully binded.");

    return kNoError;
}

Error_t TCPServer::setSocketOptions() const {
    int opt = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        LOG_ERROR("set socket option (address) failed");
        return kUnknownError;
    }

    if(setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0) {
        LOG_ERROR("set socket option (port) failed");
        return kUnknownError;
    }

    return kNoError;
}

Error_t TCPServer::start() {
    if ((listen(m_sockfd, 5)) != 0) {
        LOG_ERROR("Listen failed...");
        return kUnknownError;
    }

    LOG_INFO("TCP Server listening.");

    return acceptClient();
}

Error_t TCPServer::send(uint16_t *pEMGSamples, int iNumValues) const {
    int nBytes = int(sizeof(uint16_t)) * iNumValues;
    int ret = write(m_connfd, pEMGSamples, nBytes);
//    std::cout << "ret: " << ret << "\tsize: " << nBytes << std::endl;
    if (ret != nBytes) {
        LOG_ERROR("TCP connFD write error. {}", errno);
        return kFileWriteError;
    }

    return kNoError;
}

Error_t TCPServer::send(data_t *pData_t, int iNumValues) const {
    int nBytes = (int(sizeof(uint16_t)) * iNumValues) + (int(sizeof(char)) * Time::timeStampSize);
    int ret = write(m_connfd, pData_t, nBytes);
//    std::cout << "ret: " << ret << "\tsize: " << nBytes << std::endl;
    if (ret != nBytes) {
        LOG_ERROR("TCP connFD write error. {}", errno);
        return kFileWriteError;
    }

    return kNoError;
}

TCPServer::~TCPServer() {
    close(m_connfd);
}

Error_t TCPServer::acceptClient() {
    socklen_t len = sizeof(m_cli);
    m_connfd = accept(m_sockfd, (sockaddr*)&m_cli, &len); // blocks until there is a connection
    if (m_connfd < 0) {
        LOG_ERROR("Server accept failed.");
        return kUnknownError;
    }
    LOG_INFO("Client successfully connected to the server!");
    return kNoError;
}
