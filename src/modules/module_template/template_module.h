#pragma once
#include "../../kipod.h"

namespace kipod::Template{

class TemplateModule : public kipod::Module
{
public:
    TemplateModule(std::string name, int width = 1024, int height = 768);
};


class TemplateScene;
class TemplateModuleMenu : public kipod::MenuModule
{
    friend class TemplateModule;
    TemplateScene* scene_;
protected:
    void Draw() override;
public:    
    TemplateModuleMenu(TemplateScene* scene)
        : scene_(scene){}
};


}

