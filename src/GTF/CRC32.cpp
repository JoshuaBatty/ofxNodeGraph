//
//  CRC32.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "CRC32.h"

namespace gtf 
{
namespace CRC32
{
  
  //Non constexpr implementation from http://www.opensource.apple.com/source/xnu/xnu-1456.1.26/bsd/libkern/crc32.c
  uint32_t crc32(const char* buf, uintptr_t size, uint32_t crc)
  {
    const char *p = buf;
    
    while (size--)
    {
      crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    }
    
    return crc ^ ~0U;
  }
  
  uint32_t WSID(const char* str)
  {
    //return const_crc32((U8*)str, strlen(str));
    return crc32(str, strlen(str), ~0);
  }
  
} // namespace CRC32
} // namespace gtf
