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

class Cache::Impl
{
private:

	Cache::size_type mMaxmem;
	float mMax_load_factor;
	Evictor* mEvictor;
	Cache:size_type memory_used;
	unordered_map<key_type, std::pair<size_type, val_type>, hash_func > mCache;
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
	mEvictor(evictor), mCache(hasher)
    {}

	//uses mHasher function to choose whether or not to encache something
	void set(key_type key, Cache::val_type val, Cache::size_type size)
	{
		//evictor:
		// if cache is full, just don't add to the cache

		//replace !is_full with evictor thing:
		if (memory_used + size < mMaxmem)  // TODO: fix this
		{
				//add key to the key map
				auto hashed_key = mHasher(key);
				cache[hashed_key] = std::pair(key, val);
				// keys is of val_type
				// data is of val_type
				// size is of size_type
				memory_used += size;

		}
	}

	Cache::val_type get(key_type key, size_type& val_size)
	{
	    auto hashed_key = mHasher(key);
	    if (cache[hashed_key] && cache[hashed_key].first == key){
	        auto ret = cache[hashed_key].second;
	        val_size = get_size_of_val(ret);
	        return ret;
	    }
	    return nullptr;
	}

	//get size given a ptr to the start of val, should be easy seeing as all
	// values are char* that end with '\0'
	Cache::size_type get_size_of_val(Cache::val_type val)
	{
		//this goes on the assumption that val is in the cache. should
		// update this to make sure there are keys too.
		Cache::size_type iter = 0;
		Cache::size_type size = 0;
		while(*(val+iter) != '\0')
		{
			size++;
			iter++;
		}
		return size;
	}



	bool del(key_type key)
	{
	    size_type size;
	    val_type item = get(key, size);
	    if (item)){
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
		Cache::size_type count = 0;
		Cache::byte_type prev = ' '; //can be any char that isn't '\0'
		Cache::byte_type current;

		for (size_type i = 0; i < mMaxmem; i++)
		{
			current = *data.get()[i];
			if (prev == '\0' && current == '\0') {
				count++;
			}
			prev = current;
		}

		return count;
	}

	void reset() 
	{
		//just go thru the c-style array and set errythin to \0

	}

	//iterate thru every element and add it to the string
	std::string to_string()
	{
		std::string elements;
		for (size_type i = 0; i < mMaxmem; i++)
		{
			
			elements = elements	+ *data.get()[i];
		}
		return elements;
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
	//finds a space in data where we can put things. I want to use a ring
	//buffer because I think they are OP
	pImpl_ -> set(key, val, size);

}

Cache::val_type Cache::get(key_type key, size_type& val_size) const
{

	return pImpl_ -> get(key, val_size);	

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
