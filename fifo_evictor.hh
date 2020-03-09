#pragma once

#include "evictor.hh"

#include <queue>

class Fifo_Evictor:Evictor{

public:
    void touch_key(const key_type&) = 0;
    key_type evict() = 0;

private:

    std::queue<key_type> mQueue;
};