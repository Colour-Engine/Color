#pragma once

template <typename T> struct RemoveCV { typedef T Type; };
template <typename T> struct RemoveCV<const T> { typedef T Type; };
template <typename T> struct RemoveCV<volatile T> { typedef T Type; };
template <typename T> struct RemoveCV<const volatile T> { typedef T Type; };

template <typename T> struct RemoveConst { typedef T Type; };
template <typename T> struct RemoveConst<const T> { typedef T Type; };

template <typename T> struct RemoveVolatile { typedef T Type; };
template <typename T> struct RemoveVolatile<volatile T> { typedef T Type; };

template <typename T>
using TRemoveCV = typename RemoveCV<T>::Type;

template <typename T>
using TRemoveConst = typename RemoveConst<T>::Type;

template <typename T>
using TRemoveVolatile = typename RemoveVolatile<T>::Type;
