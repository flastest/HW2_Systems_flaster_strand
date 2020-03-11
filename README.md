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

The tests in our test files have pretty good code coverage. For each test we call a makeCache function, which, unless other arguments are included, makes a cache of size 2048 and populates it with a single key/value pair, <“apple”, “four”>. This cache object uses our fifo_evictor. We then call several other functions to ensure functionality.

As for simple tests, we make sure that get and del both return as expected, and we also make sure that calling get and del on things that are not in the cache work as expected as well. Then we test space_used by calling it on the cache and comparing it to the expected value. 

To test reset, we create a cache (as described), and reset it. Then we call get on something that used to exist in the cache, and return true if the get returns a nullptr. 

We then have three more involved tests. The first tests what happens when we put something with the same key in twice. We expect the that putting the second item in will write over the first item, as per our collision policy. The second tests what happens when we try to add something to the cache when it’s already full. We expect that the evictor will evict the first item that was loaded in in order to make room for the new item, and it should leave all other items alone. Finally, we test what happens when we add two items with the same key to the cache (triggering the collision policy) and then need to evict that item later. We expect that the evictor will be aware that the collision policy has been triggered, and will know that there is only one copy of that object in the cache.
 
