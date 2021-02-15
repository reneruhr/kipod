#pragma once
#include "../core.h"
#include "engine_events.h"
namespace kipod{

void Menu();


enum class MenuEventTypes
{
    None = 0,
    AppClose,
    ChangeModule
};


class MenuEvent : public Event
{
public:
    MenuEvent(const MenuEventTypes menu_select) : menu_select_(menu_select) {};
    MenuEventTypes menu_select_;

    MenuEventTypes GetMenuSelection() { return menu_select_; }
    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "AppEvent.";
        return ss.str();
    }
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryApplication)
    EVENT_CLASS_TYPE(Menu)
private:

};
}
