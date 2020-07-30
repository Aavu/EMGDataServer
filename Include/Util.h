//
// Created by Raghavasimhan Sankaranarayanan on 7/1/20.
//

#ifndef GOOGLEDRUMMINGARM_UTIL_H
#define GOOGLEDRUMMINGARM_UTIL_H

#include "pch.h"


#include "Data.h"
#include "ErrorDef.h"


class Util {
public:
    static int argMax(float* pfArray, size_t iLength) {
        return std::distance(pfArray, std::max_element(pfArray, pfArray + iLength));
    }

    static int mode(int* piArray, size_t iLength) {
        // TODO: Understand and customize it
        std::unordered_map<int, int> hash;

        for (size_t i=0; i<iLength; i++)
            hash[piArray[i]]++;

        int max_count = 0, res = -1;
        for (auto i : hash) {
            if (max_count < i.second) {
                res = i.first;
                max_count = i.second;
            }
        }
        return res;
    }

    static int char2Int(char key) {
        return key - '0';
    }
};

#endif //GOOGLEDRUMMINGARM_UTIL_H
