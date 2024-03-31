#pragma once

template <bool Expression, typename T = void> struct EnableIf          { };
template <                 typename T>        struct EnableIf<true, T> { typedef T Type; };

template <bool Expression, typename T = void>
using TEnableIf = typename EnableIf<Expression, T>::Type;
