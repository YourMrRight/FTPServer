#ifndef USER_H_
#define USER_H_
#pragma once

#define USR_INFO_SIZE 50
#define USE_DEFAULT_PORT 5011
#define USER_DEFAULT_IP "127.0.0.1"
class User
{
private:
	int usr_rights_;
	char *current_dir_;
	char *usr_name_;
	char *usr_pwd_;
	int port;
	char *ip;

public:
	User();
	~User();
	char* getpwd(char *) const;
	void setStatus(int);
	int getStatus() const;
	char* getUserName() const;
	void setUserName(char *);
	char* getCurrentDir() const;
	void setCurrentDir(char *);
	int getPort()const;
	void setPort(int p);
	char* getIP()const;
	void setIP(char* IP);
};

#endif