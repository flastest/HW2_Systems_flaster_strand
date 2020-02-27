#include <cache.hh>


Cache::Impl 
{
private:
	size_type mMaxmem;
	float mMax_load_factor;
	Evictor* mEvictor;
	hash_func mHasher;

public:
	Impl(size_type maxmem,
        float max_load_factor = 0.75,
        Evictor* evictor = nullptr,
        hash_func hasher = std::hash<key_type>())
	mMaxmem : maxmem, mMax_load_factor: max_load_factor,
	mEvictor : evictor, mHasher : hasher
	{}


}

Cache::Cache(size_type maxmem,
        float max_load_factor = 0.75,
        Evictor* evictor = nullptr,
        hash_func hasher = std::hash<key_type>())
{

}


Cache::~Cache();