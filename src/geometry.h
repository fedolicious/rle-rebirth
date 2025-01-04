#pragma once

#include <cstdint>

struct point {
    using numeric_t = double;
    numeric_t x;
    numeric_t y;
    
    numeric_t norm() const;
    
    point operator<=>(const point&) const = default;
    point operator+(const point other) const { return {x+other.x, y+other.y}; }
    point operator-() const { return {-x,-y}; }
    point operator-(const point other) const { return *this +- other; }
    point operator*(const numeric_t other) const { return {x*other, y*other}; }
    friend point operator*(const numeric_t scalar, const point other) { return other*scalar; }
};

struct aabb { //TODO allow x,y and wdth,hght to be treated as points for use in point addition/subtraction
    using numeric_t = double;
    numeric_t x;
    numeric_t y;
    numeric_t wdth;
    numeric_t hght;
};
struct trace_result {
    //TODO hit types for corners
    enum class side: uint8_t {none, neg_x, pos_x, neg_y, pos_y};
    point end_point;
    point::numeric_t factor;
    side hit_side;
};

[[nodiscard]] trace_result origin_to_point_over_aabb_trace(point to, aabb over);
[[nodiscard]] trace_result aabb_to_point_over_aabb_trace(aabb from, point to, aabb over);
[[nodiscard]] trace_result point_to_point_over_aabb_trace(point from, point to, aabb over);

[[nodiscard]] point::numeric_t dot_prod(point p1, point p2);
