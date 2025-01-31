#pragma once
#ifndef NODE_INITIATOR_HPP
#define NODE_INITIATOR_HPP

#include "Geode/Geode.hpp"

using namespace geode::prelude;

template<class Derived, typename... InitArgs>
class NodeInitiator {
protected:
    virtual bool init(InitArgs... args) = 0;

public:
    static Derived* createNode(InitArgs... args) {  
        static_assert(std::is_base_of<CCNode, Derived>::value, "Derived must derive from CCNode");

        auto ret = new Derived();
        if (ret->init(args...)) {
            ret->autorelease();  
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

#endif // NODE_INITIATOR_HPP