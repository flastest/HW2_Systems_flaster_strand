#include "fifo_evictor.hh"


void Fifo_Evictor::touch_key(const key_type & key) {
    mQueue.push(key);
}

key_type Fifo_Evictor::evict(){
    auto ret = mQueue.front();
    mQueue.pop();
    return ret;
}

