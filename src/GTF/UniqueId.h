//
//  UniqueId.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <string>
#include <cstdint>

#include "CRC32.h"

#define GTF_UID(__str) (gtf::UniqueId(gtf::CRC32::WSID_CONST(__str), __str))
#define GTF_HASH(__str) (gtf::CRC32::WSID_CONST(__str))

//typedef BHashMap<U32, BString> BNameRegistry;
namespace gtf 
{
class UniqueId
{
    
public:
    UniqueId();
    explicit UniqueId(const char * nameStr);
    explicit UniqueId(uint32_t uniqueId);
    UniqueId(uint32_t uniqueId, const char * nameStr);
    operator uint32_t() const;
    operator const char*() const;
    operator std::string() const;
    const char * operator*() const;
    bool operator <(UniqueId const & other) const;
    bool operator ==(UniqueId const & other) const;
    UniqueId const & operator=(UniqueId const & other);
    UniqueId const & operator=(const char * nameStr);
    UniqueId const & operator=(uint32_t uniqueId);
private:
    void reg(uint32_t uniqueId, const char * nameStr);
    uint32_t m_idU32;
};
} //namespace gtf
