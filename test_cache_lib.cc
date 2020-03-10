#include "cache.hh"
#include "fifo_evictor.hh"

#include <iostream>
#include <string>
#include <cassert>

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

auto makeCache(Cache::size_type cache_size=2048){
    Fifo_Evictor *my_evictor = new Fifo_Evictor();
    Cache* my_cache = new Cache(cache_size, 0.75, my_evictor);
    char value[]{ "four" };
    my_cache->set("apple", value, 5);
    return my_cache;
}

bool testGet(bool DEBUG_PRINT_MESSAGES){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing putting and getting something from the cache"<<std::endl;
    auto my_cache = makeCache();
    Cache::size_type size;
    auto ret = (my_cache->get("apple", size));
    std::string p(ret);
    if (DEBUG_PRINT_MESSAGES) {
        std::cout << p << std::endl;
    }
    return (p == "four");
}

bool testGetNull(bool DEBUG_PRINT_MESSAGES){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing getting something that isn't in the cache"<<std::endl;
    auto my_cache = makeCache();
    Cache::size_type size;
    auto ret = (my_cache->get("pear", size));
    if (ret == nullptr){
        return true;
    }
    return false;
}

bool testDel(bool DEBUG_PRINT_MESSAGES){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing that a deleted item is no longer in the cache"<<std::endl;
    auto my_cache = makeCache();
    Cache::size_type size;
    return ((my_cache->del("apple")) && (my_cache->get("apple", size)== nullptr));
}

bool testDelNull(bool DEBUG_PRINT_MESSAGES){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing deleting something that isn't in the cache"<<std::endl; 
    auto my_cache = makeCache();
    return !my_cache->del("pear");
}

bool testSpaceUsed(bool DEBUG_PRINT_MESSAGES){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing to make sure that space used is equal to the space of everything we put in"<<std::endl;
    auto my_cache = makeCache();
    Cache::size_type size = 5;
    auto ret = my_cache->space_used();
    if (DEBUG_PRINT_MESSAGES) std::cout<<"ret is: "<< ret <<", size is: "<<size<<std::endl;
    return (ret == size);
}

bool testReset(bool DEBUG_PRINT_MESSAGES){
    if (DEBUG_PRINT_MESSAGES) std::cout<<"Testing resetting the cache and making sure it's empty afterwards"<<std::endl;
    auto my_cache = makeCache();
    Cache::size_type size;
    if (DEBUG_PRINT_MESSAGES) std::cout<<"s(he's) br(ok)en"<<std::endl;
    my_cache->reset();
    if (DEBUG_PRINT_MESSAGES) std::cout<<"s(he) be(lie)ve(d)"<<std::endl;
    auto ret = (my_cache->get("apple", size));
    if (DEBUG_PRINT_MESSAGES) std::cout<<"real eyes realize real lies..."<<std::endl;
    if ((ret == nullptr) && (my_cache->space_used() == 0)){
        return true;
    }
    return false;
}

//should try putting in something with the same key to see what happens
bool testSameKey(bool DEBUG_PRINT_MESSAGES)
{
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing putting something into the array that a key already exists for"<<std::endl;
    auto my_cache = makeCache();
    char value[]{ "six" };
    Cache::size_type size;
    my_cache->set("apple", value, 4);
    auto ret = (my_cache->get("apple", size));
    std::string p(ret);
    if (DEBUG_PRINT_MESSAGES) {
        std::cout << p << std::endl;
    }
    return (p == "six");
}


//this test should fill up the array, then use the evictor to remove something to put something new in
bool testEvictorWithFullCache(bool DEBUG_PRINT_MESSAGES)
{
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing evicting from a full array and putting something new in"<<std::endl;
    auto my_cache = makeCache(13);
    // i like bridge
    char spades[]{ "AK10xxx" };
    my_cache->set("spades", spades, 8);

    char diamonds[]{ "KJxx" };
    my_cache->set("diamonds", diamonds, 5);

    if(DEBUG_PRINT_MESSAGES) std::cout<<"array is full"<<std::endl;

    //at this point, the cache should be full with spades and diamonds
    //spades should be evicted so that hearts can be inserted.
    char hearts[]{ "QJx" };
    my_cache->set("hearts", hearts, 4);    

    if(DEBUG_PRINT_MESSAGES) std::cout<<"attempted to add hearts"<<std::endl;

    Cache::size_type size;
    auto ret = (my_cache->get("hearts", size));
    
    if(DEBUG_PRINT_MESSAGES) std::cout<<"hearts should've been added, ret is "<< ret <<"."<<std::endl;
    if(DEBUG_PRINT_MESSAGES) std::cout<<"compare ret to hearts, which is "<<hearts<<"."<<std::endl;

    //convert ret to a string
    std::string hearts_holding_from_cache(ret);
    if(hearts_holding_from_cache == "QJx") return true;


    //test that spades are removed
    //check that diamonds are still there
    
    return false;
}


//test cache evictor where evictor needs to evict multiple items

//evicts the same item twice
bool testEvictorEvictingSameItemTwice(bool DEBUG_PRINT_MESSAGES)
{
    if (DEBUG_PRINT_MESSAGES) std::cout<<"testing what happens when the same key is in the evictor twice, and we need to remove things twice."<<std::endl;
    auto my_cache = makeCache(9);
    char value[]{ "six" };
    my_cache->set("apple", value, 4);
    
    //try adding something, see if it's in the cache and that the other thing's been removed

    char number_of_bananas[]{ "three" };
    Cache::size_type size;
    my_cache->set("banana", number_of_bananas, 6);
    
    //make sure bananas is in the cache
    auto bananas_from_cache = (my_cache->get("banana", size));
    std::string bananas(bananas_from_cache);
    int banana_num = size;

    auto apples_from_cache = (my_cache->get("apple", size));

    //check that apples is not in the cache
    return (apples_from_cache == nullptr && bananas == "three" && size == 6);


}


int main(){
    assert(testGet(false));
    assert(testGetNull(false));
    assert(testDel(false));
    assert(testDelNull(false));
    assert(testSpaceUsed(false));
    assert(testReset(false));
    assert(testSameKey(false));
    assert(testEvictorWithFullCache(false));
    assert(testEvictorEvictingSameItemTwice(false));

    std::cout<<"all tests pass!"<<std::endl;
    return 0;
}

