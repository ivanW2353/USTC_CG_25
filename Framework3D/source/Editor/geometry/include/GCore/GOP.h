#pragma once

#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/stage.h>

#include <memory>
#include <string>

#include "GCore/api.h"

USTC_CG_NAMESPACE_OPEN_SCOPE

#define USE_USD_SCRATCH_BUFFER 0

class Stage;
struct GeometryComponent;
class Geometry;
using GeometryHandle = std::shared_ptr<Geometry>;
using GeometryComponentHandle = std::shared_ptr<GeometryComponent>;

class GEOMETRY_API Geometry {
   public:
    Geometry();

    virtual ~Geometry();
    void apply_transform();

    Geometry(const Geometry& operand);
    Geometry(Geometry&& operand) noexcept;

    Geometry& operator=(const Geometry& operand);
    Geometry& operator=(Geometry&& operand) noexcept;

    static Geometry CreateMesh();

    friend bool operator==(const Geometry& lhs, const Geometry& rhs)
    {
        return lhs.components_ == rhs.components_;
        //&& lhs.stage == rhs.stage;
    }

    friend bool operator!=(const Geometry& lhs, const Geometry& rhs)
    {
        return !(lhs == rhs);
    }

    virtual std::string to_string() const;

    template<typename OperandType>
    std::shared_ptr<OperandType> get_component(size_t idx = 0) const;
    void attach_component(const GeometryComponentHandle& component);
    void detach_component(const GeometryComponentHandle& component);

    [[nodiscard]] const std::vector<GeometryComponentHandle>& get_components()
        const
    {
        return components_;
    }

   protected:
    std::vector<GeometryComponentHandle> components_;
};

template<typename OperandType>
std::shared_ptr<OperandType> Geometry::get_component(size_t idx) const
{
    size_t counter = 0;
    for (int i = 0; i < components_.size(); ++i) {
        auto ptr = std::dynamic_pointer_cast<OperandType>(components_[i]);
        if (ptr) {
            if (counter < idx) {
                counter++;
            }
            else {
                return ptr;
            }
        }
    }
    return nullptr;
}

void GEOMETRY_API init(Stage* stage);

#if USE_USD_SCRATCH_BUFFER
void GEOMETRY_API copy_prim(const pxr::UsdPrim& from, const pxr::UsdPrim& to);
#endif

USTC_CG_NAMESPACE_CLOSE_SCOPE
