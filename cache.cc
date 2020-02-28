#include "cache.hh"


class Cache::Impl
{
private:
	size_type mMaxmem;
	float mMax_load_factor;
	Evictor* mEvictor;
	hash_func mHasher;

public:
	Impl(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher)
	mMaxmem : maxmem, mMax_load_factor: max_load_factor,
	mEvictor : evictor, mHasher : hasher
	{}


}

Cache::Cache(size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher)
{
	pImpl_ = new Impl(maxmem,max_load_factor,evictor,hasher);
}


void set(key_type key, val_type val, size_type size)
{


}

val_type get(key_type key, size_type& val_size) const 
{
	return nullptr;
}

bool del(key_type key)
{
	return true;
}

size_type space_used() const 
{
	return static_cast<uint32_t>(966);
}

void reset()
{
	
}
