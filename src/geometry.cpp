#include "geometry.h"

#include <iostream>

point origin_to_point_over_aabb_trace(point p, aabb a) {
    const auto right_x = a.x + a.wdth;
    const auto lower_y = a.y + a.hght;
    decltype(p.x) factor;
    if(p.x*a.y <= p.y*right_x && p.x*a.y >= p.y*a.x && p.y > a.y && a.y > 0) {
        factor = a.y/p.y;
    } else if(p.x*a.y <= p.y*a.x && p.x*lower_y >= p.y*a.x && p.x > a.x && a.x > 0) {
        factor = a.x/p.x;
    } else if(p.x*lower_y <= p.y*a.x && p.x*lower_y >= p.y*right_x && p.y < lower_y && lower_y < 0) {
        factor = lower_y/p.y;
    } else if(p.x*lower_y <= p.y*right_x && p.x*a.y >= p.y*right_x && p.x < right_x && right_x < 0) {
        factor = right_x/p.x;
    } else {
        factor = 1;
    }
    return {factor*p.x, factor*p.y};
}
point aabb_to_point_over_aabb_trace(aabb from, point to, aabb over) {
    const point shifted = origin_to_point_over_aabb_trace(
        {to.x-from.x, to.y-from.y},
        {over.x-from.x-from.wdth, over.y-from.y-from.hght, over.wdth+from.wdth, over.hght+from.hght});
    return {shifted.x+from.x, shifted.y+from.y};
}
