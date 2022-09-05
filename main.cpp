#include "Server.cpp"

#define SERVER_PORT 5010

int main()
{
    Server svr(SERVER_PORT,(char *)"127.0.0.1");
    ACE_Reactor::instance()->register_handler(&svr,ACE_Event_Handler::ACCEPT_MASK);
    ACE_Reactor::instance()->run_reactor_event_loop();  
    return 0;
}   