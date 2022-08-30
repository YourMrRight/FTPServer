// #include "TaskThread.h"


// #include <list>

// #define MAX_BUFF_SIZE 1024

// using namespace std;


// extern list<ServerStream*>g_StreamPool;
// class ServerStream;

// int TaskThread::svc(void)
// {
//     ACE_Message_Block *Msg;
//     while (1)
//     {
//         getq(Msg);
//         ACE_Data_Block *Data_Block = Msg->data_block();
//         MsgData *pData= reinterpret_cast<MsgData *>(Data_Block->base());
//         if (0 == pData->getDataFlog())
//         {
//             std::list<ServerStream*>::iterator it;
//             for (it = g_StreamPool.begin(); it != g_StreamPool.end(); ++it)
//             {
//                 if (get_handle() == (*it)->get_handle())
//                 {
//                     g_StreamPool.erase(it);
//                     delete *it;
//                     break;
//                 }
//             }
//             return 0;
//         }
//         char strBuffer[MAX_BUFF_SIZE];
//         ACE_OS::memset(strBuffer, 0, sizeof(strBuffer));
//         ACE_OS::memcpy(strBuffer, pData->Data, sizeof(strBuffer));
//         if (processCmd(*(pData->getIOHandle())) == -1)
//         {
//             ACE_DEBUG((LM_ERROR, "error!\n"));
//             ACE_OS::exit(-1);
//         }

//         Msg->release();
//     }
//     return 0;
// }

// int TaskThread::processCmd(ACE_SOCK_Stream Stream)
// {
//     Stream.send("server recive data!\n", sizeof("server recive data!")); //响应client数据
//     ACE_OS::sleep(10);                                                   //模拟业务耗时
//     return 0;
// }
