#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <unBoard.hpp>

class threadClass;

struct ThreadArgs
{
   threadClass *threadManager;
   UnBoard *unBoard;
};

template<class T, void(T::*mem_fn)(void *unBoard)> void* thunk(void* args);

class threadClass
{
public:
    threadClass(std::string name, int Time);
    //~threadClass();
    template<class T>
    void createThread(UnBoard* ub);

    void destroyThread();
    void join();
    
    std::string name;
    int Time;
    pthread_t thread;
    bool running;
    UnBoard *unBoard;
private:
    ThreadArgs args;
    template <class T>
    void runningThread(void *unBoard);


};

#include "threadClass.tcc"



