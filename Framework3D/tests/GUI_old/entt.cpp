#include <entt/core/hashed_string.hpp>
#include <entt/core/utility.hpp>
#include <entt/entity/registry.hpp>
#include <entt/locator/locator.hpp>
#include <entt/meta/context.hpp>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/policy.hpp>
#include <entt/meta/resolve.hpp>

#include "utils/Logger/Logger.h"
#include "boost/python/object.hpp"
#include "gtest/gtest.h"

struct data_field {
    int a;
    int b;
};
#include <iostream>
using std::cout;

class test_base {
   public:
    test_base()
    {
        USTC_CG::logging("Called default constructor");
    }

    test_base(const test_base& other) : i(other.i), d(other.d)
    {
        USTC_CG::logging("Called copy constructor");
    }

    test_base(test_base&& other) noexcept : i(other.i), d(std::move(other.d))
    {
        USTC_CG::logging("Called move constructor");
    }

    ~test_base()
    {
        USTC_CG::logging("Called destructor");
    }

    test_base& operator=(const test_base& other)
    {
        USTC_CG::logging("Called copy operator");

        if (this == &other)
            return *this;
        i = other.i;
        d = other.d;
        return *this;
    }

    test_base& operator=(test_base&& other) noexcept
    {
        USTC_CG::logging("Called move operator");

        if (this == &other)
            return *this;
        i = other.i;
        d = std::move(other.d);
        return *this;
    }

    int i = 0;
    data_field d;
};

TEST(ENTT, reflection)
{
    using namespace entt::literals;

    entt::meta<test_base>().type("test_base"_hs);

    auto t = entt::type_info(std::in_place_type<test_base>);

    auto r = entt::resolve(t).construct();
    auto r2 = std::move(r);

    cout << r.data() << '\n' << r2.data() << std::endl;
}

template<typename T>
inline constexpr bool always_false_v = false;

TEST(ENTT, boost_python)
{
    using namespace entt::literals;

    namespace bp = boost::python;

    Py_Initialize();
    entt::meta<bp::api::object_operators<bp::object>>().type("bp::object"_hs);

    auto obj = entt::resolve<bp::object>().construct();

    cout << obj.cast<bp::object>().ptr() << std::endl;
}