//
//  UniqueId.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include <map>

#include "UniqueId.h"
#include "CRC32.h"

namespace gtf 
{
std::map<uint32_t, std::string> GTF_UNIQUEID_REGISTRY;

void UniqueId::reg(uint32_t uniqueId, const char * nameStr)
{
    auto ref = GTF_UNIQUEID_REGISTRY.find(m_idU32);
    if(ref == GTF_UNIQUEID_REGISTRY.end())
    {
        GTF_UNIQUEID_REGISTRY.insert(std::make_pair(uniqueId, std::string(nameStr)));
    }
}

UniqueId::UniqueId()
: m_idU32(0)
{}

UniqueId::UniqueId(const char * nameStr)
: m_idU32(GTF_CRC32(nameStr))
{
    reg(m_idU32, nameStr);
}

UniqueId::UniqueId(uint32_t uniqueId)
{
    m_idU32 = uniqueId;
}

UniqueId::UniqueId(uint32_t uniqueId, const char * nameStr)
{
    m_idU32 = uniqueId;
    
    reg(m_idU32, nameStr);
}

UniqueId::operator uint32_t() const
{
    return m_idU32;
}

UniqueId::operator const char *() const
{
    return GTF_UNIQUEID_REGISTRY[m_idU32].c_str();
}

UniqueId::operator std::string() const
{
    return GTF_UNIQUEID_REGISTRY[m_idU32];
}

const char * UniqueId::operator*() const
{
    return GTF_UNIQUEID_REGISTRY[m_idU32].c_str();
}

bool UniqueId::operator <(UniqueId const & other) const
{
    return m_idU32 < other.m_idU32;
}

bool UniqueId::operator ==(UniqueId const & other) const
{
    return m_idU32 == other.m_idU32;
}

UniqueId const & UniqueId::operator=(UniqueId const & other)
{
    m_idU32 = other.m_idU32;
    return *this;
}

UniqueId const & UniqueId::operator=(const char * nameStr)
{
    m_idU32 = GTF_CRC32(nameStr);
    return *this;
}

UniqueId const & UniqueId::operator=(uint32_t uniqueId)
{
    m_idU32 = uniqueId;
    return *this;
}
} //namespace gtf
