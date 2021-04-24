#pragma once

namespace kipod::ImageProcessing{

struct CannyData{
    float sigma_ = 1.0f;
    float low_ = 0.1f;
    float high_ = 0.3f;
};


}
