// cache.cc
// Ariel Flaster & Sarah Strand
//


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

	void set(key_type key, Cache::val_type val, Cache::size_type size)
	{

	    // Don't allow items that are bigger than the cache, or else everything will get evicted and the new item will not be added
	    if (size > mMaxmem){
	        return;
	    }

		if (memory_used + size < mMaxmem)  // TODO: fix this
		{
            //first we ned to add something to the heap
            //this needs to point to a new thing in the heap
            cache_val_type new_cache_item_pointer(new byte_type[size]);

            //now we need to copy val into new_cache_item_pointer
            std::copy(val, val+size, new_cache_item_pointer.get());

            //add key to the key map
            mCache[key] = map_val_type(size, new_cache_item_pointer	);

            //if there's an evictor, use it!
            if (mEvictor) mEvictor->touch_key(key);

            memory_used += size;

		}
		else if(mEvictor){
			// if there's no room, and an evictor, use the evictor!

			// first, pop something from the evictor.
			key_type key_to_evict = mEvictor->evict();

			// keep evicting until key_to_evict is actually something in the cache
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

	Cache::size_type space_used() 
	{
	    return memory_used;
	}

	// Reset will set the mCache variable to a new unordered map, writing over the other one
	void reset() 
	{
        mCache = std::unordered_map<key_type, std::pair<size_type, cache_val_type>, hash_func > (mMaxmem, mHash);
        memory_used = 0;
    }

};

Cache::Cache(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher):
	pImpl_ (new Impl(maxmem,max_load_factor,evictor,hasher))
	{}

void Cache::set(key_type key, val_type val, size_type size)
{
	//finds a space in data where we can put things
	pImpl_ -> set(key, val, size);

}

Cache::val_type Cache::get(key_type key, size_type& val_size) const
{

	return pImpl_ -> get(key, val_size).get();

}

bool Cache::del(key_type key)
{

	return pImpl_ -> del(key);
	
}

//all unused space should be '\0'
Cache::size_type Cache::space_used() const
{
	return pImpl_ -> space_used();

}

void Cache::reset()
{
	pImpl_ -> reset();
}
