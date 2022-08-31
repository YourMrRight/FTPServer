#ifndef FTP_MSG_H_
#define FTP_MSG_H_

#pragma once


#define ESTABLISH   "220 connection  established.\n"
#define LOGIN_SUCCESS "230 Login successful.\nRemote system type is UNIX.\nUsing binary mode to transfer files.\n"
#define DEFAULT_DIR "/home/scutech"
#define USR_LOGI_FAILED "530 Login incorrect"
#define PWD_REQUIRE "331 Please specify the password.\n"
#define OPEN_ASCII_MODE "150 Opening ASCII mode data connection for current directory.\n"
#define PWD_REQUIRE "331 Password required for open localhost.\n"
#define TRANS_COMPELETE "226 Transfer complete.\n"
#define SYST_CMD "Remote system type is UNIX.\nUsing binary mode to transfer files.\n"
#define USR_UNLOGIN "530 Please login with USER and PASS.\n"
#define CWD_SUCCESS "250 CWD command successful.\n"
#define DIR_NOT_FIND "550 The system can not find the file specified.\n"
#define OPEN_FILE_FAILED "425 File opening failed.\n"
#define FILE_DATA_BLOCK 1024
#define DATA_CONN_FAILED "425 Cannot open data connection.\n"
#define PORT_COMMAND_SUCCESS "200 PORT command successful.\n"
#define REC_BLOCK 2048
#define AUTH_CMD "502 not impliment.\n"
#define PWD_CMD "257 command successful.\n"
#define PORT_EXHAUSTED "500 sorry! server port has been EXHAUSTED!"
#define PASV_CMD "227 start passive mode "

#define BEGINPORT 10000
#define ENDPORT 15000


#define DEFAULT_IP "127,0,0,1"

#endif
