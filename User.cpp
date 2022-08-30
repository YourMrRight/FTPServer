#include "User.h"

#include <ace/OS.h>

#include <fstream>
#include <iostream>
#include <string.h>

User::User()
{
	this->usr_name_ = new char[USR_INFO_SIZE];
	this->usr_pwd_ = new char[USR_INFO_SIZE];
	this->current_dir_ = new char[USR_INFO_SIZE];
	usr_rights_ = 0;
	this->port = USE_DEFAULT_PORT;
	this->ip = new char[USR_INFO_SIZE];
	ACE_OS::strcpy(current_dir_, DEFAULT_DIR);
	ACE_OS::strcpy(ip, USER_DEFAULT_IP);
}

User::~User()
{
	delete[] usr_name_;
	delete[] this->usr_pwd_;
	delete[] this->ip;
	delete[] this->current_dir_;
}

char* User::getpwd(char *u_name)const
{
	std::ifstream ifst;
	ifst.open("Userinfo.txt");
	char usrname[USR_INFO_SIZE];
	char *usrpwd = new char[USR_INFO_SIZE];
	int exist = 1;
	while (ifst.getline(usrname, USR_INFO_SIZE))
	{
		if (strcmp(u_name, usrname) == 0)
		{
			ifst.getline(usrpwd, USR_INFO_SIZE);
			exist = 0;
			break;
		}
	}
	if (exist)
	{
		usrpwd = nullptr;
	}
	return usrpwd;
}

void User::setStatus(int stat)
{
	this->usr_rights_ = stat;
}

int User::getStatus()const
{
	return this->usr_rights_;
}

char* User::getUserName() const
{
	return this->usr_name_;
}

void User::setUserName(char* name)
{
	ACE_OS::strcpy(this->usr_name_ ,name);
}

char* User::getCurrentDir() const
{
	return this->current_dir_;
}

void User::setCurrentDir(char* dir)
{
	ACE_OS::strcpy(this->current_dir_, dir);	
}

int User::getPort() const
{
	return this->port;	
}

void User::setPort(int p)
{
	this->port = p;
}

char* User::getIP() const
{
	return this->ip;
}

void User::setIP(char* IP)
{
	ACE_OS::strcpy(this->ip,IP);
}
