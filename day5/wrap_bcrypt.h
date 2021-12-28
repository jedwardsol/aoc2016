#pragma once

#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib,"bcrypt")

#include "include/thrower.h"

namespace BCrypt
{

struct Provider
{
    Provider()
    {
        if(BCryptOpenAlgorithmProvider(&provider,BCRYPT_MD5_ALGORITHM,nullptr,0))
        {
            throw_system_error("BCryptOpenAlgorithmProvider");
        }
    }

    ~Provider()
    {
        BCryptCloseAlgorithmProvider(provider,0);
    }

    operator BCRYPT_ALG_HANDLE() const
    {
        return provider;
    }

private:    
    BCRYPT_ALG_HANDLE provider;
};


struct Hash
{
    Hash()
    {
        if(BCryptCreateHash(provider, &hash,nullptr,0,nullptr,0,0))
        {
            throw_system_error("BCryptCreateHash");
        }
    }

    ~Hash()
    {
        BCryptDestroyHash(hash);
    }



    Hash &append(std::string const &s)
    {
        if (BCryptHashData (hash, 
                            const_cast<UCHAR *>(reinterpret_cast<UCHAR const *>(s.data())), 
                            static_cast<DWORD>(s.size()), 0))
        {
            throw_system_error("BCryptHashData");
        }

        return *this;
    }

    std::string digest()
    {
        std::array<BYTE,16> bytes{};

        if (BCryptFinishHash(hash, bytes.data(), static_cast<ULONG>(bytes.size()), 0))
        {
            throw_system_error("BCryptFinishHash");
        }

        std::string  digest;

        for(auto b : bytes)
        {
            digest+= std::format("{:02x}",b);
        }

        return digest;
    }


private:    
    
    static inline Provider provider;

    BCRYPT_HASH_HANDLE   hash;
};




}