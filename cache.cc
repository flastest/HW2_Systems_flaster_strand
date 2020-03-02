#include "cache.hh"



class Cache::Impl
{
private:
	Cache::size_type mMaxmem;
	float mMax_load_factor;
	Evictor* mEvictor;
	hash_func mHasher;

	//the cache should look through the list of keys to see if that key is
	//in the array
	std::vector<key_type> keys;
	std::unique_ptr<Cache::val_type> data;

	//for the trivial impplementation, the cache will stop adding things when
	// it gets full
	bool is_full = false;



public:
	Impl(Cache::size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher) :
	mMaxmem(maxmem), mMax_load_factor(max_load_factor),
	mEvictor(evictor), mHasher(hasher)
	{
		data = new Cache::val_type[mMaxmem]();
							//here, the parens on the end initialize
							// all of the things in data to zeros.
	}

	//uses mHasher function to choose whether or not to encache something
	void hash(key_type key, Cache::val_type val, Cache::size_type size)
	{
		//evictor:
		// if cache is full, just don't add to the cache

		//to determine where to put in the cache,

	}
};

Cache::Cache(Cache::size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher):
	pImpl_ (new Impl(maxmem,max_load_factor,evictor,hasher))
	{}



void set(key_type key, Cache::val_type val, Cache::size_type size)
{
	//finds a space in data where we can put things. I want to use a ring
	//buffer because I think they are OP


}

Cache::val_type get(key_type key, Cache::size_type& Cache::val_size) const
{
	return nullptr;
}

bool del(key_type key)
{
	return true;
}

Cache::size_type space_used() const
{
	return static_cast<uint32_t>(966);
}

void reset()
{
	
}
