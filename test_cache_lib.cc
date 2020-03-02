#include "cache.hh"

//include a tester for a hasher
//this hasher just returns the hash given from the key value mod size of cache.
struct key_hasher
{


	~key_hasher() = default;

	key_hasher(const distance_map_t& distances)
	: distances_(distances)
	{}

	//this is wrong, it's just a functor I used elsewhere
	bool operator()(const vertex_descriptor &a, 
					const vertex_descriptor &b ) const 
	{
		return (distances_.at(a) > distances_.at(b));
	}
};



int test_construct(){
    return 0;
}

int test_get(){
    return 0;
}

int test_set(){
    return 0;
}

int test_del(){
    return 0;
}

int test_space_used(){
    return 0;
}

int test_reset(){
    return 0;
}


int main(){
    if (test_construct() > 0){
        std::cout<<"Constructor failed"<<std::endl
    }
}

