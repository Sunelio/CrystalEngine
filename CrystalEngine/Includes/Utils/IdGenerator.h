#pragma once

#include "Typedefs.h"
#include <vector>

namespace Utils
{
    // - IdGenerator: generates unique IDs - //
    class IdGenerator
    {
    private:
        std::vector<size_t> ids;
        
    public:
        // Generates a new unique ID and returns it.
        size_t Generate() {
            size_t freeId = ids.size();
            for (uint i = 1; i < ids.size(); i++) {
                if (ids[i] - 1 != ids[i - 1]) {
                    freeId = ids[i - 1];
                    break;
                }
            }
            
            ids.insert(ids.begin() + freeId, freeId+1); // TODO: crash (when whole hierarchy deleted and then adding new objects)
            return freeId+1;
        }

        // Frees the ID.
        void Free(const size_t& id)
        {
            if (const auto it = std::find(ids.begin(), ids.end(), id); it != ids.end())
                ids.erase(it);
        }

        void Clear()
        {
            ids.clear();
            ids.resize(0);
        }
    };
}
