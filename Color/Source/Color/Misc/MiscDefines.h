#pragma once

#define CL_CONCAT_IMPL(l, r) l##r
#define CL_CONCAT(l, r)      CL_CONCAT_IMPL(l, r)

#define CL_STRINGIFY_IMPL(x) #x
#define CL_STRINGIFY(x)      CL_STRINGIFY_IMPL(x)
