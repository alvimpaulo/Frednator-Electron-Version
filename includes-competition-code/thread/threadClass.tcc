#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

template<class T, void(T::*mem_fn)(void *unBoard)> void* thunk(void* args)
{
   //std::cout << "THUNK " << std::endl;
   void *p = ((ThreadArgs*)args)->threadManager;
   UnBoard *ub = ((ThreadArgs*)args)->unBoard;
   (static_cast<T*>(p)->*mem_fn)(ub);

   //Blackboard *b = ((ThreadArgs*)args)->blackboard;
   //(static_cast<T*>(p)->*mem_fn)(b);


   return 0;
}


template <class T> void threadClass::runningThread(void *unBoard)
{
    bool stopThread = false;
    //std::cout << "Running Thread " << name << ", BB address: " << blackboard << std::endl;

    T t(unBoard);

    while(!stopThread)
    {
        boost::system_time tickTime = boost::get_system_time();

        t.tick();

        boost::system_time finishTime = boost::get_system_time();
        boost::posix_time::time_duration duration = finishTime - tickTime;
        long long timestamp = duration.total_microseconds();

        //std::cout << "Running Thread " << name << ", BB address: " << unBoard << " TIME: " << timestamp << std::endl;

        if(duration < boost::posix_time::microseconds(Time))
            boost::this_thread::sleep(boost::posix_time::microseconds(Time) - duration);


    }
    //pthread_exit(NULL);

    //Thread will be running here!!

}

template <class T> void threadClass::createThread(UnBoard* ub)
{
    int threadCheck;
    unBoard = ub;
    args.unBoard = ub;
    args.threadManager = this;

    if( name == "Motion")
    {
        //std::cout << "Setting Motion Priority ...";

        struct sched_param param;
        int error, policy;
        pthread_attr_t attr;

        error = pthread_attr_init(&attr);
        if(error != 0)
        {
            //std::cout << "init error" << std::endl;
            error = 0;

        }

        error = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        if(error != 0)
        {
            //std::cout << "inheritance error" << std::endl;
            error = 0;

        }

        error = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        if(error != 0)
        {
            //std::cout << "policy error" << std::endl;
            error = 0;

        }

        param.sched_priority = 65;
        error = pthread_attr_setschedparam(&attr, &param);
        if(error != 0)
        {
            //std::cout << "param error" << std::endl;
            error = 0;

        }


        threadCheck = pthread_create(&thread, &attr, &thunk<threadClass, &threadClass::runningThread<T> >, &args);


        pthread_attr_destroy(&attr);


        /*error = pthread_getschedparam(thread, &policy, &param);
        if(error != 0)
        {
            std::cout << "param error" << std::endl;
            error = 0;

        }*/

        //std::cout << "Motion Priority Set" << std::endl;
        if (threadCheck)
        {
           //std::cout << "Error:unable to create thread," << threadCheck << std::endl;
           exit(-1);
        }

    }
    else
    {
        threadCheck = pthread_create(&thread, NULL, &thunk<threadClass, &threadClass::runningThread<T> >, &args);
        if (threadCheck)
        {
           //std::cout << "Error:unable to create thread:" << name << std::endl;
           exit(-1);
        }
    }
    //std::cout << "Thread Created" << std::endl;
}
