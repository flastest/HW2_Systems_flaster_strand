/*
 * Declare interface for eviction policies and a few implementations.
 */

#pragma once

#include <string>

// Data type to use as keys for Cache and Evictors:
using key_type = std::string;

// Abstract base class to define evictions policies.
// It allows touching a key (on a set or get event), and request for
// eviction, which also deletes a key. There is no explicit deletion
// mechanism other than eviction.
class Evictor {
 public:
  Evictor() = default;
  virtual ~Evictor() = default;
  Evictor(const Evictor&) = delete;  // noncopiable
  Evictor& operator=(const Evictor&) = delete;

  // Inform evictor that a certain key has been set or get:
  virtual void touch_key(const key_type&) = 0;

  // Request evictor for the next key to evict, and remove it from evictor.
  // If evictor doesn't know what to evict, return an empty key ("").
  virtual const key_type evict() = 0;
};
