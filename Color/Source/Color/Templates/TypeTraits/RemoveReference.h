#pragma once

template <typename T> struct RemoveReference { typedef T Type; };
template <typename T> struct RemoveReference<T&> { typedef T Type; };
template <typename T> struct RemoveReference<T&&> { typedef T Type; };

template <typename T>
using TRemoveReference = typename RemoveReference<T>::Type;
