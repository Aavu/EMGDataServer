//
// Created by Raghavasimhan Sankaranarayanan on 7/29/20.
//

#ifndef EMGDATASERVER_KEYLOGGER_H
#define EMGDATASERVER_KEYLOGGER_H

#include "pch.h"

// https://www.flipcode.com/archives/_kbhit_for_Linux.shtml
// https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1042856625&id=1043284385

class KeyLogger {
public:
//    // Deleting Copy constructor
//    KeyLogger (const KeyLogger&) = delete;
//    KeyLogger& operator= (const KeyLogger&) = delete;

    static KeyLogger* getInstance();
    ~KeyLogger();
    [[nodiscard]] char getKeyPress() const;
private:
    KeyLogger();
    void init();

    static KeyLogger* pInstance;
    bool m_bInitialized = false;
};


#endif //EMGDATASERVER_KEYLOGGER_H
