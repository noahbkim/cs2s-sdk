#pragma once

#include <string>
#include <utility>

#include <tier0/logging.h>

#include <ISmmPlugin.h>
#include <metamod_oslink.h>

#include "cs2s/service.h"

#ifdef WIN32
#define ROOT_BIN_DIRECTORY "/bin/win64/"
#define GAME_BIN_DIRECTORY "/csgo/bin/win64/"
#define LIBRARY_PREFIX ""
#define LIBRARY_SUFFIX ".dll"
#else
#define ROOT_BIN_DIRECTORY "/bin/linuxsteamrt64/"
#define GAME_BIN_DIRECTORY "/csgo/bin/linuxsteamrt64/"
#define LIBRARY_PREFIX "lib"
#define LIBRARY_SUFFIX ".so"
#endif

// Adapted heavily from https://github.com/Source2ZE/CS2Fixes/blob/main/src/gameconfig.h
// Combines https://github.com/Source2ZE/CS2Fixes/blob/main/src/utils/module.h
namespace cs2s::service
{

template<typename T>
struct Pattern
{
    const uint8_t* data;
    size_t size;
};

template<typename T>
struct Symbol
{
    const char* name;
};

// Wraps introspection for shared libraries
struct Library
{
    std::string path;
    std::string name;
    HINSTANCE handle{nullptr};
    void* address{nullptr};
    size_t size{0};

public:
    ~Library()
    {
        if (this->handle != nullptr)
        {
            dlclose(this->handle);
            this->handle = nullptr;
        }
    }

public:
    void* Resolve(const uint8_t* pattern_data, size_t pattern_size) const;

    template<typename T>
    T* Resolve(const Pattern<T>& pattern) const
    {
        return reinterpret_cast<T*>(this->Resolve(pattern.data, pattern.size));
    }

    template<typename T>
    T* Resolve(const Symbol<T>& symbol) const
    {
        return reinterpret_cast<T*>(dlsym(this->handle, symbol.name));
    }
};

class PluginLibraryService : public PluginService
{
protected:
    ISmmAPI* metamod{nullptr};
    IVEngineServer2* server{nullptr};

    // Cached
    std::string game_directory;

public:
    using PluginService::PluginService;

public:
    bool Load(PluginId id, ISmmAPI* ismm, bool late) override;
    bool Unload() override;

public:
    virtual bool Resolve(const std::string& subpath, std::string name, Library* library);
};

}
