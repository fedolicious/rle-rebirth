#pragma once

#include "geometry.h"

#include <vector>
#include <tuple>

namespace world {
    constexpr double wdth = 1280*16;
    constexpr double hght = 800*16;
    extern std::vector<aabb> platforms;
    
    void draw(double scale);
}

template<class... Args, std::invocable<Args...,aabb> F>
[[nodiscard]] trace_result world_trace(F f, Args&&... args) {
    // static_assert(is_invocable<Args>)
    trace_result world_trace = {
        std::get<sizeof...(Args)-1>(std::forward_as_tuple(args...)),
        1, trace_result::side::none};
    for(const auto& platform : world::platforms) {
        const auto platform_trace = f(std::forward<Args>(args)..., platform);
        if(platform_trace.factor < world_trace.factor) {
            world_trace = platform_trace;
        }
    }
    return world_trace;
}
