/*
 *  MyLogger.h
 *  openFrameworks
 *
 *  Created by Jeffrey Crouse on 5/7/09.
 *  Copyright 2009 Eyebeam. All rights reserved.
 *
 */

#include "ARToolKitPlus.h"

class MyLogger : public ARToolKitPlus::Logger
{
    void artLog(const char* nStr)
    {
        printf(nStr);
    }
};