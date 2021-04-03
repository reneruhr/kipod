#pragma once
#include "../../kipod.h"


namespace kipod::Template{
class TemplateSidebar :
        public kipod::Sidebar{
        friend class TemplateModule;

protected:
    TemplateSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    virtual void SideBarContent() override;

    void AddTemplate();
    void TemplateList();

};

}
