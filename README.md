# Cache Impelementation
Ariel Flaster and Sarah Strand

This is our implementation for the Hash it out assignment for Systems. This is an implementation of a cache that also comes equipped 
with an evictor.

## Cache Structure

Our cache hashes keys to an unordered map of size, value pairs. Data is stored in our cache as a shared pointer to the heap. 
We used shared pointers so that items in our cache only get deleted when the shared pointer itself is deleted. This is good, 
because it makes it easier for us to manage memory.

We run all of our tests with the FIFO evictor that we made, but we also implemented (and tested) that our code works without an
evictor too. In the case where there is no evictor, if an item is set in the cache that doesn't fit, we just don't add that item
to the cache. Thankfully, however, we have an evictor that we use to wisely determine what happens when an item doesn't fit in
our cache.

### Collision policy

In the case where two items with the same key are inserted into the cache, the old value is overwritten. We chose this method
because in the case where we are using a real cache, we would probably want to update old values in the cache if we are given new
data.

### Performance

Since we use an unordered map with a hasher, all operations are O(1). Hashing is great. As long as we stay below the load factor, 
finding items in our cache is a piece of cake. Our evictor is O(1) because we're just popping things from a queue. 


## Evictor

Our evictor is a standard FIFO queue that pops things to evict from the top of the queue and adds things to the bottom of the queue. It 
has a private queue that it uses to keep track of what needs to be evicted. 

### Eviction policy

Things are evicted when there isn't enough space in the cache. We evict items until there is enough room in the cache for the next item. 
When there are duplicate items in the evictor, the first instance is evicted and we keep evicting the already-evicted items until we 
find something else that can be evicted. 

This is implemented in the set function. When we set something and find that the size of the new item would excede the cache's maximum
allocated memory, we find something to evict (this includes sifting through and ignoring keys that have already been evicted), evict it, then try to set the item again. If the item being set is still too big, we evict another item using the same algorithm.

#### Extra big items
 
In the case where we are trying to set an item that is larger than the size of the cache, we don't evict any items and just don't add 
that item to the cache. This way, we don't evict everything in the cache for no reason. 

## Tests
