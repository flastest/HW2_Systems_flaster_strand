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
    // We use std::queue to keep track of the order of inputs for the purpose of fifo
    std::queue<key_type> mQueue;
};