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

auto makeCache(){
    Cache* my_cache = new Cache(2048);
    char value[]{ "four" };
    my_cache->set("apple", value, 5);
    return my_cache;
}

bool testGet(bool DEBUG_PRINT_MESSAGES){
    auto my_cache = makeCache();
    Cache::size_type size;
    auto ret = (my_cache->get("apple", size));
    if (DEBUG_PRINT_MESSAGES) {
        std::string p(ret);
        std::cout << p << std::endl;
    }
    return true;
}

bool testGetNull(bool DEBUG_PRINT_MESSAGES){
    auto my_cache = makeCache();
    Cache::size_type size;
    auto ret = (my_cache->get("pear", size));
    if (ret == nullptr){
        return true;
    }
    return false;
}

bool testDel(bool DEBUG_PRINT_MESSAGES){
    auto my_cache = makeCache();
    Cache::size_type size;
    return ((my_cache->del("apple")) && (my_cache->get("apple", size)== nullptr));
}

bool testDelNull(bool DEBUG_PRINT_MESSAGES){
    auto my_cache = makeCache();
    return !my_cache->del("pear");
}

bool testSpaceUsed(bool DEBUG_PRINT_MESSAGES){
    auto my_cache = makeCache();
    Cache::size_type size = 5;
    auto ret = my_cache->space_used();
    if (DEBUG_PRINT_MESSAGES) std::cout<<"ret is: "<< ret <<", size is: "<<size<<std::endl;
    return (ret == size);
}


int main(){
    assert(testGet(false));
    assert(testGetNull(false));
    assert(testDel(false));
    assert(testDelNull(false));
    assert(testSpaceUsed(false));
    return 0;
}

