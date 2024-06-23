#ifndef STDAFX_H_
#define STDAFX_H_

// Standard C++ library headers.
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

// Ogre3D and OIS headers.
#include <Ogre.h>
#include <OIS.h>
#include <SdkTrays.h>

// Windows headers.
#if _WIN32
# define WIN32_LEAN_AND_MEAN
# define NOMINMAX
# include <windows.h>
#endif

#endif
