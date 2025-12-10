#include <server_objects.h>

Uid_generator::Uid_generator(){
    std::srand(std::time(0));
}

std::string Uid_generator::new_uid(int len){
    std::string uid;
    do{
        for(int i=0; i<len; i++){
            uid += character_set[std::rand()%character_set.size()];
        }
    }while(distributed_uid.find(uid)!=distributed_uid.end());
    distributed_uid.insert(uid);
    return uid;
}

bool Uid_generator::rm_uid(std::string uid){
    auto it = distributed_uid.find(uid);
    if(it==distributed_uid.end()){
        return false;
    }
    else{
        distributed_uid.erase(it);
        return true;
    }
}