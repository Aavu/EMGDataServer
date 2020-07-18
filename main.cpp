#include "pch.h"
#include "Server.h"
#include "CoAmp.h"
#include "Time.h"

using namespace std::chrono_literals;

int main() {
    Logger::init(Logger::trace);

    CoAmp::SensorParam_t sensorParam;
    sensorParam.iWindowLength = 256;
    sensorParam.iHopLength = 256;
    sensorParam.iGain = 1;
    CoAmp* pSensor = CoAmp::getInstance();
    pSensor->init(sensorParam);

    Server* pServer = Server::getInstance();
    pServer->init(pSensor, 8080, 128);

    pServer->run(5);

    delete pSensor;
    delete pServer;
    return 0;
}
