#include <server_objects.h>
#include <string.h>
#include <assert.h>

linear_buf_t::linear_buf_t(){assert(1==0);}

linear_buf_t::linear_buf_t(int cap){
    read_pos = 0;
    this->cap = cap;
    buf.reserve(cap);
}

void linear_buf_t::append(char *data){
    buf.append(data);
    if(buf.size() > cap){/* error */}
}

void linear_buf_t::append(std::string &data){
    buf += data;
    if(buf.size() > cap){/* error */}
}

void linear_buf_t::set_buf(std::string &&data){
    buf = data;
    if(buf.size() > cap){/* error */}
}

char *linear_buf_t::getline(){
    int pos;
    char *line;
    if((pos = buf.find("\n", read_pos)) == std::string::npos)
        return NULL;
    line = strdup(buf.substr(read_pos, pos-read_pos).c_str());
    if(read_pos > cap/2){
        buf.erase(0, read_pos);
        read_pos = 0;
    }
    else{
        read_pos = pos + 1;
    }
    return line;
}