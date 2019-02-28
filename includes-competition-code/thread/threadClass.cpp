
#include "threadClass.hpp"

threadClass::threadClass(std::string name, int Time)
{
    this->name = name;
    this->Time = Time;
    running = false;
    //std::cout << "Thread Registered" << std::endl;

}

void threadClass::join()
{
    pthread_join(thread, NULL);
}


