#pragma once
#ifndef CHECKABLE_NODE_HPP
#define CHECKABLE_NODE_HPP

#include "Geode/Geode.hpp"

using namespace geode::prelude;

template<class... PArgs>
class CheckableNode : public CCNode  {
protected:
    std::function<void(PArgs... args)> m_checkFunc;

protected:
    void registerCheckable(CCMenuItemToggler* toggler) {
        if (toggler == nullptr) {
            return;
        }

        toggler->m_pfnSelector = menu_selector(CheckableNode::onCheckImpl);
        toggler->m_pListener = this;
    }

    virtual void onCheckImpl(CCObject* sender) = 0;

public:
    void onCheck(const std::function<void(PArgs... args)>& func) {
        m_checkFunc = func;
    }
};

#endif // CHECKABLE_NODE_HPP