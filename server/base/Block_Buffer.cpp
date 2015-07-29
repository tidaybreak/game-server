/*
 * Event_Buffer.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: Root
 */

#include "Block_Buffer.h"

Block_Buffer::Block_Buffer(unsigned short max_use_times, size_t init_size, size_t init_offset)
: max_use_times_(max_use_times),
  use_times_(0),
  init_size_(init_size),
  init_offset_(init_offset),
  read_index_(init_offset),
  write_index_(init_offset),
  buffer_(init_offset + init_size),
  read_error_abort_(true),
  ext_flag_(false)
{ }
