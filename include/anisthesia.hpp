#pragma once

#ifdef ANISTHESIA_EXPORTS
#define ANISTHESIA_API __declspec(dllexport)
#else
#define ANISTHESIA_API __declspec(dllimport)
#endif

#include <anisthesia/media.hpp>
#include <anisthesia/player.hpp>
#include <anisthesia/win_platform.hpp>

extern "C" ANISTHESIA_API int anisthesiaInit();