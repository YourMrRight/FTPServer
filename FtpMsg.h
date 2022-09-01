#ifndef FTP_MSG_H_
#define FTP_MSG_H_

#pragma once


#define ESTABLISH   "220 connection  established.\r\n"
#define LOGIN_SUCCESS "230 Login successful.\r\n" //Remote system type is UNIX.\r\nUsing binary mode to transfer files.\r\n"

#define USR_LOGI_FAILED "530 Login incorrect.\r\n"
#define PWD_REQUIRE "331 Please specify the password.\r\n"
#define OPEN_ASCII_MODE "150 Opening ASCII mode data connection for current directory.\r\n"
#define PWD_REQUIRE "331 Password required for open localhost.\r\n"
#define TRANS_COMPELETE "226 Transfer complete.\r\n"
#define SYST_CMD "Remote system type is UNIX.\n"//Using binary mode to transfer files.\r\n"
#define USR_UNLOGIN "530 Please login with USER and PASS.\r\n"
#define CWD_SUCCESS "250 Directory successfully changed.\r\n"
#define DIR_NOT_FIND "550 The system can not find the file specified.\r\n"
#define OPEN_FILE_FAILED "425 File opening failed.\r\n"
#define DATA_CONN_FAILED "425 Cannot open data connection.\r\n"
#define PORT_COMMAND_SUCCESS "200 PORT command successful.\r\n"
#define AUTH_CMD "502 not impliment.\r\n"
#define PWD_CMD "257 "
#define PORT_EXHAUSTED "500 sorry! server port has been EXHAUSTED.\r\n"
#define PASV_CMD "227 start passive mode.\r\n"

#define FILE_DATA_BLOCK 1024
#define REC_BLOCK 2048
#define BEGINPORT 10000
#define ENDPORT 15000

#define DEFAULT_DIR "/home/scutech"
#define ROOT_DIR "/"

#define DEFAULT_IP "127,0,0,1"
#define DEFAULT_PORT 5051

#endif
