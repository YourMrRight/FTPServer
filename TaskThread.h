#ifndef TASKTHREAD_H_
#define TASKTHREAD_H_
#pragma once

#include <ace/Task.h>
#include <ace/SOCK_Stream.h>

#include <string>

using namespace std;

class TaskThread : public ACE_Task<ACE_MT_SYNCH>
{
    virtual int svc(void);
    int processCmd(ACE_SOCK_Stream &Stream, string &cmd); 
public:
    int processUSER(ACE_SOCK_STREAM &Stream, string &cmd);
    
};

#endif