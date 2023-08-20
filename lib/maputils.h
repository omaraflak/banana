#if !defined(MAP_UTILS)
#define MAP_UTILS

#include <map>

namespace maputils {
template <class K, class V>
std::map<V, K> reverse(const std::map<K, V>& mapping) {
    std::map<V, K> result;
    for (const auto& pair : mapping) {
        result[pair.second] = pair.first;
    }
    return result;
}
}

#endif // MAP_UTILS
