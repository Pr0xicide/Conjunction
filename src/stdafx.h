#ifndef STDAFX_H_
#define STDAFX_H_

// Standard C++ library headers.
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

// Ogre3D and OIS headers.
#include <Ogre.h>
#include <OIS.h>
#include <SdkTrays.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

// FMOD headers.
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_codec.h>
#include <fmod_dsp.h>
#include <fmod_errors.h>
#include <fmod_memoryinfo.h>
#include <fmod_output.h>

// Windows headers.
#if _WIN32
# define WIN32_LEAN_AND_MEAN
# define NOMINMAX
# include <windows.h>
#endif

#endif
