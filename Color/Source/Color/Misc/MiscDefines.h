#pragma once

#define CL_CONCAT_IMPL(L, R) L##R
#define CL_CONCAT(L, R)      CL_CONCAT_IMPL(L, R)

#define CL_STRINGIFY_IMPL(X) #X
#define CL_STRINGIFY(X)      CL_STRINGIFY_IMPL(X)

#define CL_NODISCARD [[nodiscard]]
