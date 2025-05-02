#ifndef LOGMANGER_HPP
#define LOGMANAGER_HPP
#include <queue>
#include <mutex>

template<typename WorkType>
class LogManager{
    private:
        std::queue<WorkType> _processQueue;
        std::mutex _processQueueMutex;
    public:
        int getQueueSize() const {return _processQueue.size();}
        void addWork(WorkType& work);
        void start();
        void stop();
};

#endif