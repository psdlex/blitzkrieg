#pragma once
#ifndef DIALOGS_UTIL_HPP
#define DIALOGS_UTIL_HPP

#include "Geode/Geode.hpp"

using namespace geode::prelude;

class DialogsUtil {
public:
    enum class ButtonResult { Left, Right };

    static void showAlert(const char* title, const char* content) {
        FLAlertLayer::create(title, content, "Ok")->show();
    }

    static void showAlert(const char* title, const char* content, const char* button) {
        FLAlertLayer::create(title, content, button)->show();
    }

    static void showChoice(
        const char* title,
        const char* content,
        const char* choiceLeft,
        const char* choiceRight,
        std::function<void(ButtonResult)> callback) {
        // body    
        createQuickPopup(title, content, choiceLeft, choiceRight, [callback](auto, bool button){
            callback(button ? ButtonResult::Right : ButtonResult::Left);
        });
    }

private:
    DialogsUtil() {};
};

#endif // DIALOGS_UTIL_HPP