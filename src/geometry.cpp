#include "geometry.h"

#include <iostream>

trace_result origin_to_point_over_aabb_trace(point p, aabb a) { //TODO optimize (many calculations are repeated
    const auto right_x = a.x + a.wdth;
    const auto lower_y = a.y + a.hght;
    using enum trace_result::side;
    
    if(p.x*a.y <= p.y*right_x && p.x*a.y >= p.y*a.x && 0 <= a.y && a.y <= p.y
        && p.y != 0 && !(a.x == 0 && p.x <= 0) && !(right_x == 0 && p.x >= 0)) {
        return {{a.y/p.y*p.x, a.y}, a.y/p.y, neg_y};
    } else if(p.x*a.y <= p.y*a.x && p.x*lower_y >= p.y*a.x && 0 <= a.x && a.x <= p.x
        && p.x != 0 && !(a.y == 0 && p.y <= 0) && !(lower_y == 0 && p.y >= 0)) {
        return {{a.x, a.x/p.x*p.y}, a.x/p.x, neg_x};
    } else if(p.x*lower_y <= p.y*a.x && p.x*lower_y >= p.y*right_x && p.y <= lower_y && lower_y <= 0
        && p.y != 0 && !(a.x == 0 && p.x <= 0) && !(right_x == 0 && p.x >= 0)) {
        return {{lower_y/p.y*p.x, lower_y}, lower_y/p.y, pos_y};
    } else if(p.x*lower_y <= p.y*right_x && p.x*a.y >= p.y*right_x && p.x <= right_x && right_x <= 0
        && p.x != 0 && !(a.y == 0 && p.y <= 0) && !(lower_y == 0 && p.y >= 0)) {
        return {{right_x, right_x/p.x*p.y}, right_x/p.x, pos_x};
    } else {
        return {{p.x, p.y}, 1, none};
    }
}
trace_result aabb_to_point_over_aabb_trace(aabb from, point to, aabb over) {
    auto result = origin_to_point_over_aabb_trace(
        {to.x-from.x, to.y-from.y},
        {over.x-from.x-from.wdth, over.y-from.y-from.hght, over.wdth+from.wdth, over.hght+from.hght});
    result.end_point.x += from.x;
    result.end_point.y += from.y;
    return result;
}
