/*
 * Interface for a generic cache object.
 * Data is given as blobs (void *) of a given size, and indexed by a string key.
 */

#pragma once

#include <functional>
#include <memory>

#include "evictor.hh"

class Cache {
 private:
   // All internal data and functionality is hidden using the Pimpl idiom
   // (see here: https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/)
  class Impl;
  std::unique_ptr<Impl> pImpl_;

 public:
  using byte_type = char;
  using val_type = const byte_type*;   // Values for K-V pairs
  using size_type = uint32_t;         // Internal indexing to K-V elements

  // A function that takes a key and returns an index to the internal data
  using hash_func = std::function<std::size_t(key_type)>;

  // Create a new cache object with the following parameters:
  // maxmem: The maximum allowance for storage used by values.
  // max_load_factor: Maximum allowed ratio between buckets and table rows.
  // evictor: Eviction policy implementation (if nullptr, no evictions occur
  // and new insertions fail after maxmem has been exceeded).
  // hasher: Hash function to use on the keys. Defaults to C++'s std::hash.
  Cache(size_type maxmem,
        float max_load_factor = 0.75,
        Evictor* evictor = nullptr,
        hash_func hasher = std::hash<key_type>());

  ~Cache();

  // Disallow cache copies, to simplify memory management.
  Cache(const Cache&) = delete;
  Cache& operator=(const Cache&) = delete;

  // Add a <key, value> pair to the cache.
  // If key already exists, it will overwrite the old value.
  // Both the key and the value are to be deep-copied (not just pointer copied).
  // If maxmem capacity is exceeded, enough values will be removed
  // from the cache to accomodate the new value. If unable, the new value
  // isn't inserted to the cache.
  void set(key_type key, val_type val, size_type size);

  // Retrieve a pointer to the value associated with key in the cache,
  // or nullptr if not found.
  // Sets the actual size of the returned value (in bytes) in val_size.
  val_type get(key_type key, size_type& val_size) const;

  // Delete an object from the cache, if it's still there
  bool del(key_type key);

  // Compute the total amount of memory used up by all cache values (not keys)
  size_type space_used() const;

  // Delete all data from the cache
  void reset();
};

