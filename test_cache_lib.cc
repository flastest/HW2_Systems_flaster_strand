#include "cache.hh"

#include <iostream>
#include <string>

//include a tester for a hasher
//this hasher just returns the hash given from the key value mod size of cache.
//struct key_hasher
//{
//
//
//	~key_hasher() = default;
//
//	key_hasher(const distance_map_t& distances)
//	: distances_(distances)
//	{}
//
//	//this is wrong, it's just a functor I used elsewhere
//	bool operator()(const vertex_descriptor &a,
//					const vertex_descriptor &b ) const
//	{
//		return (distances_.at(a) > distances_.at(b));
//	}
//};

bool test1(bool DEBUG_PRINT_MESSAGES){
    Cache* my_cache = new Cache(2048);
    std::cout<<"line 27"<<std::endl;
    char value[]{ "four" };
    std::cout<<"line 29"<<std::endl;
    Cache::size_type size;
    std::cout<<"line 31"<<std::endl;
    my_cache->set("apple", value, 4);
    std::cout<<"line 33"<<std::endl;
    auto ret = (my_cache->get("apple", size));
    std::cout<<"line 35"<<std::endl;
    if (ret == nullptr){
        std::cout<<"null!!"<<std::endl;
    }
    std::string p(ret);
    std::cout<<"line 37"<<std::endl;
    std::cout<<p<<std::endl;
    return true;
}

int main(){
    assert(test1(false));
    return 0;
}

