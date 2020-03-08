// unordered map hashes stuff
// store value and size in map
// read unordered map documentation
// when something doesnt fit then evict some stuff to make room
// use pair as map object
// implement butt counter


#include "cache.hh"

#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>



using cache_val_type = std::shared_ptr<Cache::byte_type>;


class Cache::Impl
{
private:

	Cache::size_type mMaxmem;
	float mMax_load_factor;
	Evictor* mEvictor;
	Cache::size_type memory_used;
	std::unordered_map<key_type, std::pair<size_type, cache_val_type>, hash_func > mCache;
	//this is a butt counter
	int butt_counter = 0;

public:

	//for the trivial implementation, the cache will stop adding things when
	// it gets full
	bool is_full = false;
	//this will be removed in the final version.

	Impl(Cache::size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher) :
	mMaxmem(maxmem), mMax_load_factor(max_load_factor),
	mEvictor(evictor), mCache(maxmem, hasher)
    {}

	//uses mHasher function to choose whether or not to encache something
	void set(key_type key, Cache::val_type val, Cache::size_type size)
	{
		//evictor:
		// if cache is full, just don't add to the cache

		//replace !is_full with evictor thing:
		if (memory_used + size < mMaxmem)  // TODO: fix this
		{
				//first we ned to add something to the heap
				//this needs to point to a new thing in the heap
				cache_val_type new_cache_item_pointer(new byte_type[size]);

				//now we need to copy val into new_cache_item_pointer
				std::copy(val, val+size, new_cache_item_pointer.get());

				//add key to the key map
				auto key_val_pair = std::pair(key, std::pair(size, new_cache_item_pointer));
				mCache.insert(key_val_pair);
				
				memory_used += size;

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

	    }

		//need to get size of key somehow

		//first go to the pointer of key
		//auto ptr_to_value = keys[key];

		//then iterate though values til u get to "\0",deleting everything
		//this should be in impl.

		//then remove key from the table


		//index_that_we_are_up_to = ptr_to_value;
		return true;
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
		//just go thru the c-style array and set errythin to \0

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
