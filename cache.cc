#include "cache.hh"

#include <map>

class Cache::Impl
{
private:
	Cache::size_type mMaxmem;
	float mMax_load_factor;
	Evictor* mEvictor;
	hash_func mHasher;

	
	std::unique_ptr<Cache::byte_type> data;

	
	Cache::size_type index_that_we_are_up_to = 0;

	//find free space
	// iterate through data, looking for a place to put something 

	//the cache should look through the list of keys to see if that key is
	//in the array
	std::map<key_type,Cache::val_type> keys;


public:
	

	//for the trivial impplementation, the cache will stop adding things when
	// it gets full
	bool is_full = false;

	Impl(Cache::size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher) :
	mMaxmem(maxmem), mMax_load_factor(max_load_factor),
	mEvictor(evictor), mHasher(hasher)
	{
		data = std::make_unique<Cache::byte_type>(new Cache::byte_type[mMaxmem]());
							//here, the parens on the end initialize
							// all of the things in data to zeros.
	}

	//uses mHasher function to choose whether or not to encache something
	void hash_and_insert(key_type key, Cache::val_type val, Cache::size_type size)
	{
		//evictor: 
		// if cache is full, just don't add to the cache

		//replace !is_full with evictor thing:
		if (!is_full)
		{
			//first determine if there's room in data for the thing.
			//needs to be imporved.
			if (index_that_we_are_up_to + size < mMaxmem ) 
			{
				//add key to the key map
				keys[key] = &(data.get()[size]);
				// keys is of val_type
				// data is of pointer to byte_type
				// size is of size_type

				//first put the data in
				for (size_type i = 0; i < size; i++)	
				{
					data.get()[index_that_we_are_up_to + i] = val[i];
				}

				//the increment data for trivial implementation
				index_that_we_are_up_to += size;
			}

		}
		//there isn't room for anything if there's only 1 byte left
		if (index_that_we_are_up_to + 1  >= mMaxmem) 
		{
			is_full = true;
		}
	}

	Cache::val_type get(key_type key, size_type& val_size) 
	{


		if(keys.count(key) ==0 ){
			return nullptr;	
		}

		val_size = get_size_of_val(keys.at(key));



		return keys.at(key);
	//idfk what eitan wants us to do with val_size
	}

	//get size given a ptr to the start of val, should be easy seeing as all
	// values are char* that end with '\0'
	Cache::size_type get_size_of_val(Cache::val_type val)
	{
		//this goes on the assumption that val is in the cache. should
		// update this to make sure there are keys too.
		uint32_t iter = 0;
		Cache::size_type size = 0;
		while(val[iter] != '\0')
		{
			size++;
			iter++;
		}
		return size;
	}
};

Cache::Cache(Cache::size_type maxmem,
        float max_load_factor,
        Evictor* evictor,
        hash_func hasher):
	pImpl_ (new Impl(maxmem,max_load_factor,evictor,hasher))
	{}



void Cache::set(key_type key, Cache::val_type val, Cache::size_type size)
{
	//finds a space in data where we can put things. I want to use a ring 
	//buffer because I think they are OP
	pImpl_ -> hash_and_insert(key, val, size);

}

Cache::val_type Cache::get(key_type key, size_type& val_size) const 
{

	return pImpl_ -> get(key, val_size);	
	

}


//for now, in initial implementation, only use this to delete the last value
bool Cache::del(key_type key)
{
	//this impl should be in impl 
	if(!(pImpl_->keys.find(key))){
		return false;
	}

	pImpl_ ->is_full = false;

	//need to get size of key somehow
	
	//first go to the pointer of key
	auto ptr_to_value = pImpl_ -> keys[key];

	//then iterate though values til u get to "\0",deleting everything
	//this should be in impl.

	//then remove key from the table


	index_that_we_are_up_to = ptr_to_value;
	return true;
}


//all unused space should be '\0'
Cache::size_type Cache::space_used() const 
{
	//iterate through the array, counting all the '\0's that occur
	// after other '\0s'
}

void Cache::reset()
{
	// I think with unique_ptrs, I can just 
	pImpl_ = new Impl();
	//and nothing bad happens, other thing is outta scope so we gucci
}
