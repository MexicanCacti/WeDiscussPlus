#include <atomic>

#ifndef SERVER_HPP
#define SERVER_HPP
class Server{
    private:
        std::atomic<bool> isRunning = false;
    public:
        void run(){
            startServer();
            for(int i = 0 ; i < 3; ++i){
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            stopServer();
        }
        void startServer(){
            isRunning = true;
            std::cout << "Server Started\n";
        }

        void stopServer(){
            isRunning = false;
            std::cout << "Server Stopped!\n";
        }
        
        bool IsRunning() const {return isRunning;}
};

#endif