#include <type_traits>

template<typename E>
struct enable_bitmask_operators {
    static constexpr bool enable = false;
};

template<typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator|(E lhs, E rhs) {
    using underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator&(E lhs, E rhs) {
    using underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator^(E lhs, E rhs) {
    using underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type
operator~(E rhs) {
    using underlying = typename std::underlying_type<E>::type;
    return static_cast<E>(~static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E&>::type
operator|=(E& lhs, E rhs) {
    using underlying = typename std::underlying_type<E>::type;
    lhs = static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
    return lhs;
}

template<typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E&>::type
operator&=(E& lhs, E rhs) {
    using underlying = typename std::underlying_type<E>::type;
    lhs = static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
    return lhs;
}

template<typename E>
typename std::enable_if<enable_bitmask_operators<E>::enable, E&>::type
operator^=(E& lhs, E rhs) {
    using underlying = typename std::underlying_type<E>::type;
    lhs = static_cast<E>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
    return lhs;
}

#define DEFINE_ENUM_BITMASK(EnumType) \
template<> \
struct enable_bitmask_operators<EnumType> { \
    static constexpr bool enable = true; \
};