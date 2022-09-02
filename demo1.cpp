// #include <stdio.h>
// #include <string.h>
// #include <errno.h>

// #include <unistd.h>

// #include <dirent.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <iostream>
// using namespace std;
// extern int errno;

// #define MODE (S_IRWXU | S_IRWXG | S_IRWXO)

// int mk_dir(char *dir)
// {
// 	DIR *mydir = NULL;
// 	if ((mydir = opendir(dir)) == NULL) //判断目录
// 	{
// 		int ret = mkdir(dir, MODE); //创建目录
// 		if (ret != 0)
// 		{
// 			return -1;
// 		}
// 		printf("%s created sucess!/n", dir);
// 	}
// 	else
// 	{
// 		printf("%s exist!/n", dir);
// 	}

// 	return 0;
// }

// int mk_all_dir(char *dir)
// {
// 	cout << dir<<endl;
	
// 	bool flag = true;
// 	char *pDir = dir;
// 	while (flag)
// 	{
// 		char *pIndex = index(pDir, '/');
// 			cout << pIndex<<endl;
// 		if (pIndex != NULL && pIndex != dir)
// 		{
// 			char buffer[512] = {0};
// 			int msg_size = pIndex - dir;
// 			memcpy(buffer, dir, msg_size);
// 			int ret = mk_dir(buffer);
// 			if (ret < 0)
// 			{
// 				printf("%s created failed!/n", dir);
// 			}
// 		}
// 		else if (pIndex == NULL && pDir == dir)
// 		{
// 			printf("dir is not directory!/n");
// 			return -1;
// 		}
// 		else if (pIndex == NULL && pDir != dir)
// 		{
// 			int ret = mk_dir(dir);
// 			if (ret < 0)
// 			{
// 				printf("%s created failed!/n", dir);
// 			}

// 			break;
// 		}

// 		pDir = pIndex + 1;
// 	}

// 	return 0;
// }
// size_t getFileSize1(const char *fileName) {

// 	if (fileName == NULL) {
// 		return 0;
// 	}
	
// 	// 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
// 	struct stat statbuf;

// 	// 提供文件名字符串，获得文件属性结构体
// 	stat(fileName, &statbuf);
	
// 	// 获取文件大小
// 	size_t filesize = statbuf.st_size;

// 	return filesize;
// }

#include<iostream>
#include<cstdio>

using namespace std;

// void mkdir_output(const string &output_path){
//     if (access(output_path.c_str(), 0) == -1) {
//         // mkdir(output_path.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
//         system(("mkdir "+output_path).c_str());
//         std::cout<<output_path<<" create! "<<std::endl;
//     }
//     else {
//         system(("rm -rf "+output_path).c_str());
//         system(("mkdir "+output_path).c_str());
//         std::cout<<output_path<<" recover create! "<<std::endl;
//     }
// }



int main()
{
    char *savePath = "/home/scutech/ChatRoom.cpp";
        
    if(remove(savePath)==0)
    {
        cout<<"删除成功"<<endl;        
    }
    else
    {
        cout<<"删除ututgu8hu失败"<<endl;        
    }
    return 0;
}