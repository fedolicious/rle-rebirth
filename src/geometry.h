#pragma once

struct point {
    double x;
    double y;
};

struct aabb {
    double x;
    double y;
    double wdth;
    double hght;
};

[[nodiscard]] point origin_to_point_over_aabb_trace(point to, aabb over); //Returns hit point
[[nodiscard]] point aabb_to_point_over_aabb_trace(aabb from, point to, aabb over); //Returns x,y of colliding aabb 
