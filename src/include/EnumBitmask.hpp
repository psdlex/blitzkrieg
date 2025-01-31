#define ENUM_BITWISE_OPERATORS(T)                                  \
constexpr T operator&(T a, T b) {                                  \
    using U = std::underlying_type_t<T>;                           \
    return static_cast<T>(static_cast<U>(a) & static_cast<U>(b));  \
}                                                                  \
constexpr T operator|(T a, T b) {                                  \
    using U = std::underlying_type_t<T>;                           \
    return static_cast<T>(static_cast<U>(a) | static_cast<U>(b));  \
}                                                                  \
constexpr T operator^(T a, T b) {                                  \
    using U = std::underlying_type_t<T>;                           \
    return static_cast<T>(static_cast<U>(a) ^ static_cast<U>(b));  \
}                                                                  \
constexpr T operator~(T a) {                                       \
    using U = std::underlying_type_t<T>;                           \
    return static_cast<T>(~static_cast<U>(a));                     \
}                                                                  \
inline T& operator|=(T& a, T b) {                                  \
    a = a | b;                                                     \
    return a;                                                      \
}                                                                  \
inline T& operator&=(T& a, T b) {                                  \
    a = a & b;                                                     \
    return a;                                                      \
}                                                                  \
inline T& operator^=(T& a, T b) {                                  \
    a = a ^ b;                                                     \
    return a;                                                      \
}