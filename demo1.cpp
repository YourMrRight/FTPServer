#include<iostream>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>//memcmp()
#include<grp.h>//getgrgid
#include<unistd.h>//getcwd
#include<pwd.h>//getpwuid
#include<cstdio>
#include <sstream>
#include "Util.h"
using namespace std;

int ls(char *dirpath);//ls命令
int ls_l(char *dirpath);//ls -l命令

// char filename[100][255];
// int filenum = 0;

int main(int args,char* argv[]) {
	char dir[255];
	getcwd(dir,255);//获取当前工作环境绝对路径,存入dir中
	// if( args > 1){//ls -l命令
		ls_l(dir);
		//cout<<"000";
	// }else{//ls命令
    //     cout<<dir<<endl;
	// 	ls(dir);
	// }


    string str = "/user/local";

	return 0;
}
int ls(char *dirpath){
	DIR *dir;//用于存放opendir()成功时的返回值
	struct dirent *file;//用于存放readir()的返回值
	/*函数DIR *opendir(const char *pathname)，用于打开pathname，
	若成功将返回值DIR类型的值，若失败，返回空指针*/
	struct stat info;//用于存放文件信息
	if(!(dir = opendir(dirpath))){
		//打开目录失败
		//printf("error opendir %s\n",dirpath);
		cout<<"fail to read"<<dirpath;
		return -1;
	}
	//读取opendir的返回值，其中d_name是文件名
	while((file = readdir(dir))!= NULL){
		//文件无文件名只有后缀
		if(strncmp(file->d_name, ".", 1) == 0)
			continue;
		//获取文件信息，存储在info中
		stat(file->d_name,&info);
		//打印文件名
		cout<<file->d_name<<endl;
	}
	cout<<endl;
	closedir(dir);
	return 1;
}

int ls_l(char *dirpath){
	DIR *dir;//用于存放opendir()成功时的返回值
	struct dirent *file;//用于存放readir()的返回值
	struct stat info;//目录项描述结构体
	if(!(dir = opendir(dirpath))){
		//打开目录失败
		cout<<"fail to read"<<dirpath;
		return -1;
	}
	struct passwd *userinfo;
	struct group *groupinfo;
	while((file = readdir(dir))!= NULL){
		//文件无文件名只有后缀
		if(strncmp(file->d_name, ".", 1) == 0)
			continue;
		strcpy(filename[filenum++],file->d_name);
		//获取文件信息，存储在info中
		stat(file->d_name,&info); 
		userinfo = getpwuid(info.st_uid);//所有者用户名 
		groupinfo = getgrgid(info.st_gid);//所有者所在组用户名
		char pw[11];
		power(info.st_mode,pw);//文件权限信息
        string file_info;

        int fz = info.st_size;
		file_info.append(pw).append("\t").append(userinfo->pw_name).append("\t").append(groupinfo->gr_name).append("\t");
        stringstream ss;
        ss<<fz<<"\t";
        file_info.append(ss.str());
        file_info+= 4 + ctime(&info.st_mtime);
        trimToParentDir(file_info);
		file_info.append("\t").append(file->d_name).append("\n");
		cout<<file_info;
	}
	closedir(dir);
	return 1;
}
