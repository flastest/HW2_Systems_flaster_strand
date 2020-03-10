#pragma once

#include "evictor.hh"

#include <queue>
#include <iostream>


class Fifo_Evictor: public Evictor{

public:
	Fifo_Evictor() : Evictor(){};
	~Fifo_Evictor(){};
    void touch_key(const key_type&);
    key_type evict();

private:

    std::queue<key_type> mQueue;
};