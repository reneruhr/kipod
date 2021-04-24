#pragma once
#include "../../kipod.h"


namespace kipod::ImageProcessing{
class ImageProcessingSidebar :
        public kipod::Sidebar{
        friend class ImageProcessingModule;

protected:
    ImageProcessingSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    virtual void SideBarContent() override;

    void AddImage();
    void ImageList();
    void Filter();
    void FilterList();

    void AlgorithmList();

    void Canny();
};

}
