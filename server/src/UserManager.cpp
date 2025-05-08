#include "server_include/UserManager.hpp"
#include "server_include/Server.hpp"

template<typename WorkType>
std::unordered_map<int, User> UserManager<WorkType>::_userMap;

template<typename WorkType>
std::unordered_map<int, std::string> UserManager<WorkType>::_userIDToName;

template<typename WorkType>
std::unordered_map<std::string, int> UserManager<WorkType>::_usernameToID;

template<typename WorkType>
void UserManager<WorkType>::setUpDatabaseConnection() {
    // Set up connection to database & set up maps!
}

// Notes this->_processQueue is needed for proper name lookup from compiler!
template<typename WorkType>
void UserManager<WorkType>::handleClient(){
    try{
        while(this->_shouldDoWork){
            this->_processQueueMutex.lock();
            if(this->_processQueue.empty()){
                this->_processQueueMutex.unlock();
                std::this_thread::sleep_for(2s);
                continue;
            }
    
            WorkType workToDo = std::move(this->_processQueue.front());
            this->_processQueueMutex.unlock();
            this->_processQueue.pop();
    
            // Go Ahead & Look at the Work Type ID to figure out what it should be doing!. Then Socket should be in WorkType to set back data to client if needed
            // EX: For AUTH USER:
            // authUser(work)
        }      
    }
    catch(const std::exception& e){
        std::cerr << "Thread Exception: " << e.what() << std::endl;
    }
}

template<typename WorkType>
void UserManager<WorkType>::authUser(WorkType* work){
    // Then dereference work pointer to get username & password & socket to send confirmation to
    // Also Update the DB! -> For online status?
    // Send Message using Server's sendMessageToSocket function!
    // Message should also contain a mapping of all online username & userID!
}

template<typename WorkType>
void UserManager<WorkType>::logoutUser(WorkType* work){
    // Dereference work pointer to get username & userID... remove references from map
    // Also Update the DB! -> For online status?
    // Send Message using Server's sendMessageToSocket function!
}

template<typename WorkType>
void UserManager<WorkType>::addUser(WorkType* work){
    // Dereference work pointer to get userID & userName & socket to send confirmation to
    // Also Update the DB!
    // Send Message using Server's sendMessageToSocket function!
}

template<typename WorkType>
void UserManager<WorkType>::changeUserPassword(WorkType* work){
    // Dereference work pointer to get userID and password to change User object contained in map
    // If not in map, look in DB & add to map!
    // Also update the DB!
    // Send Message using Server's sendMessageToSocket function!
}

template<typename WorkType>
void UserManager<WorkType>::changeUserName(WorkType* work){
    // Dereference work pointer to get userID and password to change User object contained in map, Also change Maps w/ new userName, delete old & replace with new!
    // If not in map, look in DB & add to map!
    // Also update the DB!
    // Send Message using Server's sendMessageToSocket function!
}


template<typename WorkType>
void UserManager<WorkType>::deleteUser(WorkType* work){
    // Dereference work pointer to get userID and userName to delete references in maps
    // Also update the DB!
    // Send Message using Server's sendMessageToSocket function!
}

template<typename WorkType>
void UserManager<WorkType>::sendMessageToUser(WorkType* work){
    // Dereference work pointer to get userID and userName to send message to
    // Send Message using Server's sendMessageToSocket function!
    // Server should have a function to add message to the logBalancer's work queue!
}

template class UserManager<Message>;