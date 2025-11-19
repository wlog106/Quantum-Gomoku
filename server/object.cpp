#include <set>
#include <string>
#include <ctime>

using std::string;
using std::set;

struct Room{

};

class Uid_generator{
    set<string> distributed_uid;
    string character_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    Uid_generator(){
        std::srand(std::time(0));
    }
public:
    string new_uid(int len){
        string uid;
        do{
            for(int i=0; i<len; i++){
                uid += character_set[std::rand()%character_set.size()];
            }

        }while(distributed_uid.find(uid)==distributed_uid.end());
        return uid;
    }
};