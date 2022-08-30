#ifndef SERVERSTREAM_H_
#define SERVERSTREAM_H_
#pragma once

#include "MsgData.cpp"
#include "TaskThread.h"
#include "FtpMsg.h"
#include "User.h"
#include "FilesCtr.cpp"

#include <ace/Event_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Task.h>
#include <ace/Reactor.h>
#include <ace/OS.h>

#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

#define COMMAND_SIZE 80

static unordered_map<int, User *> handlerToUser;

class TaskThread : public ACE_Task<ACE_MT_SYNCH>
{
    virtual int svc(void);
    int processCmd(ACE_SOCK_Stream Stream, char *cmd);
};
class ServerStream : public ACE_Event_Handler
{
private:
    /* data */
    ACE_SOCK_STREAM m_Svr_stream;
    TaskThread *m_MsgQueue;

public:
    ServerStream(TaskThread *pMsgQueue);
    ~ServerStream();
    virtual int handle_input(ACE_HANDLE fd) override;
    void close();
    virtual ACE_HANDLE get_handle(void) const override;
    ACE_SOCK_STREAM &get_stream();
};
extern list<ServerStream *> g_StreamPool;
int TaskThread::svc(void)
{
    ACE_Message_Block *Msg;
    while (1)
    {
        getq(Msg);
        ACE_Data_Block *Data_Block = Msg->data_block();
        MsgData *pData = reinterpret_cast<MsgData *>(Data_Block->base());
        if (0 == pData->getDataFlog())
        {
            std::list<ServerStream *>::iterator it;
            for (it = g_StreamPool.begin(); it != g_StreamPool.end(); ++it)
            {
                if (get_handle() == (*it)->get_handle())
                {
                    g_StreamPool.erase(it);
                    delete *it;
                    break;
                }
            }
            return 0;
        }
        char strBuffer[MAX_BUFF_SIZE];
        ACE_OS::memset(strBuffer, 0, sizeof(strBuffer));
        ACE_OS::memcpy(strBuffer, pData->Data, sizeof(strBuffer));
        if (processCmd(*(pData->getIOHandle()), pData->Data) == -1)
        {
            ACE_DEBUG((LM_ERROR, "error!\n"));
            ACE_OS::exit(-1);
        }

        Msg->release();
    }
    return 0;
}

int TaskThread::processCmd(ACE_SOCK_Stream Stream, char *cmd)
{
    cout << Stream.get_handle() << endl;
    cout << cmd << endl;
    if (cmd[0] == 'U')
    {
        User *user = new User;
        char *u_name = cmd + 5;
        user->setUserName(u_name);
        // handlerToUser[Stream.get_handle()]->setUserName(u_name);
        int i;
        for (i = 0; !isspace(user->getUserName()[i]); i++)
            ;
        user->getUserName()[i] = 0;
        Stream.send(PWD_REQUIRE, ACE_OS::strlen(PWD_REQUIRE), 0);
        ACE_DEBUG((LM_DEBUG, "wait for password\n"));
        handlerToUser[Stream.get_handle()] = user;
    }
    else if (cmd[0] == 'P' && cmd[1] == 'A')
    {
        char *file_pwd = handlerToUser[Stream.get_handle()]->getpwd(handlerToUser[Stream.get_handle()]->getUserName());

        char *password = cmd + 5;

        char pwd_cmd[COMMAND_SIZE];

        ACE_OS::strcpy(pwd_cmd, password);

        int i;
        for (i = 0; !isspace(pwd_cmd[i]); ++i)
            ;
        pwd_cmd[i] = 0;

        if (ACE_OS::strcmp(file_pwd, pwd_cmd) == 0)
        {
            handlerToUser[Stream.get_handle()]->setStatus(1);

            Stream.send(LOGIN_SUCCESS, ACE_OS::strlen(LOGIN_SUCCESS));
        }
        else
        {
            Stream.send(USR_LOGI_FAILED, ACE_OS::strlen(USR_LOGI_FAILED));
            handlerToUser[Stream.get_handle()]->setStatus(0);
            ACE_DEBUG((LM_DEBUG, "user %s login failed! \n", handlerToUser[Stream.get_handle()]->getUserName()));
            if (handlerToUser.find(Stream.get_handle()) != handlerToUser.end())
            {
                User *del = handlerToUser[Stream.get_handle()];
                handlerToUser.erase(Stream.get_handle());
                delete del;
            }
        }
    }
    else if (cmd[0] == 'S' && cmd[1] == 'Y' && handlerToUser.find(Stream.get_handle()) != handlerToUser.end() &&
             handlerToUser[Stream.get_handle()]->getStatus() == 0)
    {
        Stream.send(SYST_CMD, ACE_OS::strlen(SYST_CMD));
    }
    else if (cmd[0] == 'C' && cmd[1] == 'W' && handlerToUser.find(Stream.get_handle()) != handlerToUser.end())
    {
        if (handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN));
            return 0;
        }
        User *currentUser = handlerToUser[Stream.get_handle()];
        char *dir_cmd = cmd + 4;
        char dir_info[COMMAND_SIZE];
        ACE_OS::strcpy(dir_info, dir_cmd);
        // currentUser->setCurrentDir(dir_cmd);
        char *upper_flag = "..";
        int i;
        for (i = 0; !isspace(dir_info[i]); i++)
            ;
        dir_info[i] = 0;
        if (ACE_OS::strcmp(dir_info, upper_flag) == 0)
        {
            if (ACE_OS::strcmp(currentUser->getCurrentDir(), DEFAULT_DIR) == 0)
            {
                Stream.send(CWD_SUCCESS, ACE_OS::strlen(CWD_SUCCESS));
            }
            else
            {
                int length = ACE_OS::strlen(currentUser->getCurrentDir()) - 2;
                while (currentUser->getCurrentDir()[length] != '/' && currentUser->getCurrentDir()[length] != '\\' && length >= 0)
                {
                    length--;
                }
                currentUser->getCurrentDir()[length] = 0;
                Stream.send(CWD_SUCCESS, ACE_OS::strlen(CWD_SUCCESS));
            }
            ACE_DEBUG((LM_DEBUG, "user %s modified directory!\n", currentUser->getUserName()));
        }
        else
        {
            string str(currentUser->getCurrentDir());
        }
    }
    else if (cmd[0] == 'Q' && cmd[1] == 'U')
    {
        // if user have not logged in
        if (handlerToUser.find(Stream.get_handle()) == handlerToUser.end() || handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN), 0);
        }
        else
        {
            char *q_cmd = "221\n";
            Stream.send(q_cmd, ACE_OS::strlen(q_cmd), 0);
            if (handlerToUser.find(Stream.get_handle()) != handlerToUser.end())
            {
                User *del = handlerToUser[Stream.get_handle()];
                handlerToUser.erase(Stream.get_handle());
                delete del;
            }
        }
    }
    else if (cmd[0] == 'R' && cmd[1] == 'E')
    {
        if (handlerToUser.find(Stream.get_handle()) == handlerToUser.end() || handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN), 0);
            return 0;
        }
        User *currentUser = handlerToUser[Stream.get_handle()];
        char *file_name_ = cmd + 5;
        char file_name[COMMAND_SIZE];
        ACE_OS::strcpy(file_name, file_name_);
        int len = ACE_OS::strlen(file_name);
        string str(file_name);
        char *ch_file_name = new char[COMMAND_SIZE];
        ACE_OS::strcpy(ch_file_name, str.c_str());
        FilesCtr fileCtr;
        if (!fileCtr.fileExist(ch_file_name))
        {
            Stream.send(DIR_NOT_FIND, ACE_OS::strlen(DIR_NOT_FIND), 0);
            return 0;
        }
        else
        {
            FILE *fp;
            fp = ACE_OS::fopen(ch_file_name, "rb");
            if (fp == nullptr)
            {
                Stream.send(OPEN_FILE_FAILED, ACE_OS::strlen(OPEN_FILE_FAILED), 0);
                return 0;
            }

            ACE_OS::fseek(fp, 0L, 2);
            int len = ACE_OS::ftell(fp);
            ACE_OS::fseek(fp, 0L, 0);

            char *file_block = new char[FILE_DATA_BLOCK];
            int read_len = ACE_OS::fread(file_block, 1, FILE_DATA_BLOCK, fp);
            file_block[read_len] = 0;
            int data_read = 1024;
            ACE_DEBUG((LM_DEBUG, "opening file: %s\n", ch_file_name));
        }
    }
    else if (cmd[0] == 'L' && cmd[1] == 'I' && handlerToUser.find(Stream.get_handle()) != handlerToUser.end() &&
             handlerToUser[Stream.get_handle()]->getStatus() == 0)
    {
        ACE_SOCK_Stream *data_stream = new ACE_SOCK_STREAM();
        ACE_SOCK_Connector *data_conn = new ACE_SOCK_Connector();
        ACE_INET_Addr *inet_addr = new ACE_INET_Addr();
        User *currentUser = handlerToUser[Stream.get_handle()];
        inet_addr->set(currentUser->getPort(),currentUser->getIP());
        if(data_conn->connect(*data_stream, *inet_addr)==-1)
        {
            Stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
            return 0;
        } 
        ACE_DIR* dir = ACE_OS::opendir(currentUser->getCurrentDir());
        dirent* diren;
        while ((diren=ACE_OS::readdir(dir))!=nullptr)
        {
            if(data_stream->send(diren->d_name, ACE_OS::strlen(diren->d_name), 0) == -1)
            {
                break;
            }
            data_stream->send("\n",3,0);
        }
        data_stream->close();
        Stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
        delete data_conn;
        delete data_stream;
        delete inet_addr;
        Stream.
    }
    return 0;
}

#endif