// unordered map hashes stuff
// store value and size in map
// read unordered map documentation
// when something doesnt fit then evict some stuff to make room
// use pair as map object


#include "cache.hh"

#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>



using cache_val_type = std::shared_ptr<Cache::byte_type>;
using map_val_type = std::pair<Cache::size_type,cache_val_type>;

class Cache::Impl
{
private:

	Cache::size_type mMaxmem;
	float mMax_load_factor;
	Evictor* mEvictor;
	hash_func mHash;
	Cache::size_type memory_used = 0;
	std::unordered_map<key_type, std::pair<size_type, cache_val_type>, hash_func > mCache;

public:

	Impl(Cache::size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher) :
	mMaxmem(maxmem), mMax_load_factor(max_load_factor),
	mEvictor(evictor), mHash(hasher), mCache(maxmem, hasher)
    {}

	//uses mHasher function to choose whether or not to encache something
	void set(key_type key, Cache::val_type val, Cache::size_type size)
	{
		//evictor:
		// if cache is full, just don't add to the cache

		if (memory_used + size < mMaxmem)  // TODO: fix this
		{
            //first we ned to add something to the heap
            //this needs to point to a new thing in the heap
            cache_val_type new_cache_item_pointer(new byte_type[size]);

            //now we need to copy val into new_cache_item_pointer
            std::copy(val, val+size, new_cache_item_pointer.get());

            //add key to the key map
            mCache[key] = map_val_type(size	,new_cache_item_pointer	);


            //if there's an evictor, use it!
            if (mEvictor) mEvictor->touch_key(key);

            memory_used += size;

		}
		else if(mEvictor){
			//if there's no room, and an evictor, use the evictor!

			//first, pop something from the evictor.
			key_type key_to_evict = mEvictor->evict();

			//keep evicting until key_to_evict is actually something in the cache
			while (mCache.find(key_to_evict) == mCache.cend()) key_to_evict = mEvictor->evict();

			del(key_to_evict);

			//now try to set again. if more space is needed, another item should be evicted
			set(key, val, size);

		}
	}

	cache_val_type get(key_type key, size_type& val_size)
	{
	    if (mCache.find(key) != mCache.cend()){
	        auto ret = mCache[key].second;
	        val_size = mCache[key].first;
	        return ret;
	    }
	    return nullptr;
	}


	bool del(key_type key)
	{
	    size_type size;
	    cache_val_type item = get(key, size);
	    if (item){

            mCache.erase(key);
            memory_used -= size;
            return true;

	    }
		return false;
	}


	//this will iterate thru data, counting the things that aren't \0 followed by \0.
	//uh technically I think all the space is used beceause it's not like
	// I'm gonna store other things in this allocated space.
	Cache::size_type space_used() 
	{
	    return memory_used;
	}

	void reset() 
	{
        mCache = std::unordered_map<key_type, std::pair<size_type, cache_val_type>, hash_func > (mMaxmem, mHash);
        memory_used = 0;
    }

	//iterate thru every element and add it to the string
/*	std::string to_string()
	{
		std::string elements;
		for (size_type i = 0; i < mMaxmem; i++)
		{
			
			elements = elements	+ *data.get()[i];
		}
		return elements;
	}
*/
};

Cache::Cache(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher):
	pImpl_ (new Impl(maxmem,max_load_factor,evictor,hasher))
	{}



void Cache::set(key_type key, val_type val, size_type size)
{
	//finds a space in data where we can put things. I want to use a ring
	//buffer because I think they are OP
	pImpl_ -> set(key, val, size);

}

Cache::val_type Cache::get(key_type key, size_type& val_size) const
{

	return pImpl_ -> get(key, val_size).get();

}


//for now, in initial implementation, only use this to delete the last value
bool Cache::del(key_type key)
{

	return pImpl_ -> del(key);
	
}


//all unused space should be '\0'
Cache::size_type Cache::space_used() const
{
	return pImpl_ -> space_used();
	//iterate through the array, counting all the '\0's that occur
	// after other '\0s'
}

void Cache::reset()
{
	pImpl_ -> reset();
}

/*std::string Cache::to_string()
{
	return pImpl_ -> to_string();
}*/
