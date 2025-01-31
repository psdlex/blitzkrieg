#ifndef SCROLL_UTIL_HPP
#define SCROLL_UTIL_HPP

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ScrollUtil {
public:
    static void scrollToTop(const geode::ScrollLayer* scrollLayer) {
        scrollLayer->m_contentLayer->setPositionY(scrollLayer->getContentHeight() - scrollLayer->m_contentLayer->getContentHeight());
    }

    static void scrollToBottom(const geode::ScrollLayer* scrollLayer) {
        scrollLayer->m_contentLayer->setPositionY(0);
    }

    static void scrollToPosition(const geode::ScrollLayer* scrollLayer, int32_t position) {
        scrollLayer->m_contentLayer->setPositionY(position);
    }

private:
    ScrollUtil() {};
};

#endif // SCROLL_UTIL_HPP