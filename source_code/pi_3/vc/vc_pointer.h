#pragma once

#include "type_definitions.h"

class vc_pointer
{
  u32 raw_value;

public:
  vc_pointer(u32 raw_value); //todo: make private.

public:
  void operator=(const vc_pointer &source) volatile;

  static vc_pointer arm_to_vc_pointer(void *arm_pointer);
  static vc_pointer arm_to_vc_pointer(volatile void *arm_pointer);
  void *to_arm_pointer() const;
  u32 get_raw_value();

  friend vc_pointer operator+(vc_pointer lhs, const u32 &rhs)
  {
    lhs.raw_value += rhs;
    return lhs;
  }

  friend vc_pointer operator-(vc_pointer lhs, const u32 &rhs)
  {
    lhs.raw_value -= rhs;
    return lhs;
  }
} __attribute__((packed));

class vc_handle
{
private:
  u32 raw_value;

public:
  vc_handle(u32 raw_value) : raw_value(raw_value) {}
  static vc_handle make_null_handler() { return vc_handle(0u); }
  u32 get_raw_value() { return raw_value; }
  u32 is_null() { return raw_value == 0u; }
} __attribute__((packed));