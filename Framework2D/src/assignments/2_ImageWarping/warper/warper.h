// HW2_TODO: Please implement the abstract class Warper
// 1. The Warper class should abstract the **mathematical mapping** involved in
// the warping problem, **independent of image**.
// 2. The Warper class should have a virtual function warp(...) to be called in
// our image warping application.
//    - You should design the inputs and outputs of warp(...) according to the
//    mathematical abstraction discussed in class.
//    - Generally, the warping map should map one input point to another place.
// 3. Subclasses of Warper, IDWWarper and RBFWarper, should implement the
// warp(...) function to perform the actual warping.
#pragma once

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>
#include <Eigen/Dense>

namespace USTC_CG
{
    using Point2D = Eigen::Vector2f;
    using uchar = unsigned char;

class Warper
{
   public:
    virtual ~Warper() = default;

    // HW2_TODO: A virtual function warp(...)
    virtual Point2D warp(const Point2D& p) const = 0;

    // HW2_TODO: other functions or variables if you need
};
}  // namespace USTC_CG