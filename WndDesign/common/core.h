#pragma once


#ifdef WNDDESIGN_EXPORT
#define WNDDESIGN_API __declspec(dllexport)
#else
#define WNDDESIGN_API __declspec(dllimport)
#endif

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name)   }

BEGIN_NAMESPACE(WndDesign)

using uint = unsigned int;


END_NAMESPACE(WndDesign)
