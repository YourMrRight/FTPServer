#include "TaskThread.h"

#include <unordered_map>
#include <string>

using namespace std;

unordered_map<int, User *> handlerToUser;
extern list<ServerStream *> g_StreamPool;
int TaskThread::processCmd(ACE_SOCK_Stream &Stream, string &cmd)
{
    cout << Stream.get_handle() << endl;
    cout << "--------------" << cmd << "--------------" << endl;
    if (cmd.substr(0,4) == "USER")
    {
        if(processUSER(Stream, cmd)==-1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }else if (cmd.substr(0,4) == "PASS")
    {
        string file_pwd = handlerToUser[Stream.get_handle()]->getpwd(handlerToUser[Stream.get_handle()]->getUserName());

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
    else if (cmd[0] == 'L' && cmd[1] == 'I')
    {
        if (handlerToUser.find(Stream.get_handle()) == handlerToUser.end() &&
            handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN), 0);
            return 0;
        }
        Stream.send(OPEN_ASCII_MODE, ACE_OS::strlen(OPEN_ASCII_MODE), 0);
        ACE_SOCK_Stream *data_stream = new ACE_SOCK_STREAM();
        ACE_SOCK_Connector *data_conn = new ACE_SOCK_Connector();
        ACE_INET_Addr *inet_addr = new ACE_INET_Addr();
        User *currentUser = handlerToUser[Stream.get_handle()];
        inet_addr->set(currentUser->getPort(), currentUser->getIP());
        if (data_conn->connect(*data_stream, *inet_addr) == -1)
        {
            Stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
            return 0;
        }
        ACE_DIR *dir = ACE_OS::opendir(currentUser->getCurrentDir());
        dirent *diren;
        while ((diren = ACE_OS::readdir(dir)) != nullptr)
        {
            if (data_stream->send(diren->d_name, ACE_OS::strlen(diren->d_name), 0) == -1)
            {
                break;
            }
            data_stream->send("\n", 3, 0);
        }
        data_stream->close();
        Stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
        delete data_conn;
        delete data_stream;
        delete inet_addr;
    }
    else if (cmd[0] == 'P' && cmd[1] == 'O')
    {
        if (handlerToUser.find(Stream.get_handle()) == handlerToUser.end() || handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN), 0);
            return 0;
        }
        char *ip_cmd = cmd + 5;
        char ip_addr[COMMAND_SIZE];
        ACE_OS::strcpy(ip_addr, ip_cmd);

        int i = 0;
        for (; !isspace(ip_addr[i]); i++)
            ;
        ip_addr[i] = 0;

        int port1 = 0;
        int port2 = 0;
        int comma_counter = 0;
        int num_index = 0;
        i--;

        while (!(comma_counter == 1 && ip_addr[i] == ','))
        {
            if (ip_addr[i] == ',')
            {
                comma_counter++;
                num_index = 0;
            }
            else // process number
            {
                double radix = 10;
                if (comma_counter == 0)
                {
                    // the last segment of the ip address
                    int curr_cha_number = ip_addr[i] - '0';

                    port1 += int(curr_cha_number * pow(radix, num_index));

                    num_index++;
                }
                else
                {
                    int curr_cha_number = ip_addr[i] - '0';
                    port2 += int(curr_cha_number * pow(radix, num_index));
                    num_index++;
                }
            }
            i--;
        }

        int addr_len = ACE_OS::strlen(ip_addr);
        for (int j = 0; j < addr_len; j++)
            if (ip_addr[j] == ',')
                ip_addr[j] = '.';
        ip_addr[i] = 0;
        handlerToUser[Stream.get_handle()]->setIP(ip_addr);

        int port = port2 * 256 + port1;
        handlerToUser[Stream.get_handle()]->setPort(port);
        Stream.send(PORT_COMMAND_SUCCESS, ACE_OS::strlen(PORT_COMMAND_SUCCESS), 0);
        ACE_DEBUG((LM_DEBUG, "user %s PORT command successful\n", handlerToUser[Stream.get_handle()]->getUserName()));
    }
    else if (cmd[0] == 'S' && cmd[1] == 'T' && cmd[2] == 'O' && cmd[3] == 'R')
    {
        if (handlerToUser.find(Stream.get_handle()) == handlerToUser.end() || handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN), 0);
            return 0;
        }
        User *user = handlerToUser[Stream.get_handle()];
        Stream.send(OPEN_ASCII_MODE, ACE_OS::strlen(OPEN_ASCII_MODE), 0);
        char *f_name_ = cmd + 5;
        char *f_name = new char[COMMAND_SIZE];
        ACE_OS::strcpy(f_name, f_name_);
        int leng = ACE_OS::strlen(f_name);
        f_name[leng - 2] = 0;

        std::string file_path(user->getCurrentDir());
        file_path.append(f_name);
        char f_path[COMMAND_SIZE];
        ACE_OS::strcpy(f_path, file_path.c_str());

        FILE *fp;
        fp = ACE_OS::fopen(f_path, "wb+");

        ACE_SOCK_Stream *data_stream = new ACE_SOCK_Stream();
        ACE_SOCK_Connector *data_con = new ACE_SOCK_Connector();
        ACE_INET_Addr *inet_add = new ACE_INET_Addr();
        inet_add->set(user->getPort(), user->getIP());

        if (data_con->connect(*data_stream, *inet_add) == -1)
        {
            Stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
            ACE_DEBUG((LM_DEBUG, "user %s connection error.\n", user->getUserName()));
            return 0;
        }

        char *rec_buf = new char[FILE_DATA_BLOCK];

        int rec_counter = data_stream->recv(rec_buf, REC_BLOCK);

        if (rec_counter < REC_BLOCK)
            rec_buf[rec_counter] = 0;

        int total_count = rec_counter;

        while (rec_counter != -1 && rec_counter == REC_BLOCK)
        {
            rec_counter = data_stream->recv(rec_buf, REC_BLOCK);
            ACE_OS::fwrite(rec_buf, 1, rec_counter, fp);
            total_count += rec_counter;
            if (rec_counter < REC_BLOCK)
                break;
        }
        rec_buf[total_count] = 0;

        ACE_OS::fclose(fp);
        data_stream->close();

        Stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
        ACE_DEBUG((LM_DEBUG, "user %s upload file %s in the current directory.\n", user->getUserName(), f_path));

        delete data_stream;
        delete data_con;
        delete inet_add;
        delete[] f_name;

        delete[] rec_buf;
    }
    else if (cmd[0] == 'X' && cmd[1] == 'P')
    {
        if (handlerToUser.find(Stream.get_handle()) == handlerToUser.end() || handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN), 0);
            return 0;
        }
        User *user = handlerToUser[Stream.get_handle()];
        char *dir_root = new char[COMMAND_SIZE];
        std::string str("257");
        str.append(user->getCurrentDir());
        str.append(" is current directory.\r\n");
        ACE_OS::strcpy(dir_root, str.c_str());
        Stream.send(dir_root, ACE_OS::strlen(dir_root), 0);
        ACE_DEBUG((LM_DEBUG, "user %s display current directory\n", user->getUserName()));

        delete[] dir_root;
    }
    else if (cmd[0] == 'A' && cmd[1] == 'U')
    {
        Stream.send(AUTH_CMD, ACE_OS::strlen(AUTH_CMD), 0);
    }
    else if (cmd[0] == 'P' && cmd[1] == 'W')
    {
        if (handlerToUser.find(Stream.get_handle()) == handlerToUser.end() || handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            return 0;
        }
        User *user = handlerToUser[Stream.get_handle()];
        // ACE_SOCK_Stream *data_stream = new ACE_SOCK_Stream();
        // ACE_SOCK_Connector *data_con = new ACE_SOCK_Connector();
        // ACE_INET_Addr *inet_add = new ACE_INET_Addr();
        // inet_add->set(user->getPort(), user->getIP());

        // if (data_con->connect(*data_stream, *inet_add) == -1)
        // {
        //     Stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
        //     ACE_DEBUG((LM_DEBUG, "user %s connection error.\n", user->getUserName()));
        //     return 0;
        // }
        // data_stream->send(user->getCurrentDir(), ACE_OS::strlen(user->getCurrentDir()), 0);
        Stream.send(PWD_CMD, ACE_OS::strlen(PWD_CMD), 0);
        Stream.send(user->getCurrentDir(), ACE_OS::strlen(user->getCurrentDir()), 0);
        // delete data_stream;
        // delete data_con;
        // delete inet_add;
    }
    else if(cmd[0] == 'P' && cmd[1] == 'A' && cmd[2] == 'S' && cmd[3] == 'V'||(cmd[0] == 'E'&& cmd[1] == 'P'))
    {
        if(handlerToUser.find(Stream.get_handle()) == handlerToUser.end() || handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN), 0);
            return 0;
        }
        User* user = handlerToUser[Stream.get_handle()];

        ACE_INET_Addr *file_addr = new ACE_INET_Addr;
        ACE_SOCK_Acceptor *file_accept = new ACE_SOCK_Acceptor; 
        int pPort;
        for(pPort = BEGINPORT; pPort < ENDPORT; pPort++){
            if(file_addr->set(pPort) == -1){
                continue;
            }
            if(file_accept->open(*file_addr)==-1){
                continue;
            }
        }
        if(pPort>=ENDPORT){
            Stream.send(PORT_EXHAUSTED, ACE_OS::strlen(PORT_EXHAUSTED));
        }
        std::stringstream ipAndPort;
        ipAndPort<<PASV_CMD<<"("<<DEFAULT_IP<<","<<(pPort>>8)<<","<<(pPort & 0xFF)<<").";
        Stream.send(ipAndPort.str().c_str(), ACE_OS::strlen(ipAndPort.str().c_str()),0);
        user->setPort(pPort);
        user->setIP(DEFAULT_IP);
    }
    
    return 0;
}
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
        if (this->processCmd(*(pData->getIOHandle()), string(pData->Data)) == -1)
        {
            ACE_DEBUG((LM_ERROR, "error!\n"));
            ACE_OS::exit(-1);
        }

        Msg->release();
    }
    return 0;
}

int TaskThread::processUSER(ACE_SOCK_Stream &Stream, string &cmd){
        User *user = new User;
        string u_name = cmd.substr(0,4);
        user->setUserName(u_name);
        if(Stream.send(PWD_REQUIRE, ACE_OS::strlen(PWD_REQUIRE), 0)==-1){
            return -1;
        }   
        ACE_DEBUG((LM_DEBUG, "wait for password\n"));
        handlerToUser[Stream.get_handle()] = user;
        return 0
}