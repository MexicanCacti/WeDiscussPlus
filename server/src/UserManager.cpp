#include "server_include/UserManager.hpp"

template<typename WorkType>
void UserManager<WorkType>::start(){
    if(setUpDone){
        checkForWork();
        return;
    }

    // Call function to set up connection to database & set up maps!
    checkForWork();
}

/* NOTE: Maybe Try/Catch statement so if something happens to thread, then it will crash gracefully? */
template<typename WorkType>
void UserManager<WorkType>::checkForWork(){
    try{
        while(_shouldDoWork){
            if(_processQueue.empty()){
                std::this_thread::sleep_for(2s);
                continue;
            }
    
            WorkType workToDo = std::move(_processQueue.front());
            std::unique_ptr<WorkType> work = workToDo;
            _processQueue.pop();
    
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
}

template<typename WorkType>
void UserManager<WorkType>::logoutUser(WorkType* work){
    // Dereference work pointer to get username & userID... remove references from map
}

template<typename WorkType>
void UserManager<WorkType>::addUser(WorkType* work){
    // Dereference work pointer to get userID & userName & socket to send confirmation to
    // Also Update the DB!
}

template<typename WorkType>
void UserManager<WorkType>::changeUserPassword(WorkType* work){
    // Dereference work pointer to get userID and password to change User object contained in map
    // Also update the DB!
}


template<typename WorkType>
void UserManager<WorkType>::changeUserName(WorkType* work){
    // Dereference work pointer to get userID and password to change User object contained in map, Also change Maps w/ new userName, delete old & replace with new!
    // Also update the DB!
}


template<typename WorkType>
void UserManager<WorkType>::deleteUser(WorkType* work){
    // Dereference work pointer to get userID and userName to delete references in maps
    // Also update the DB!
}
