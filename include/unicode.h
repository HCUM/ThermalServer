#pragma once

#pragma warning( push )
#pragma warning(disable : 4995)

#include <cstddef>
#include <fstream>

#pragma warning( pop )

#ifdef WIN32
#ifndef __UNICODE__
#define __UNICODE__
#endif

#define __STDC_WANT_LIB_EXT1__
#define _CRT_STDIO_ISO_WIDE_SPECIFIERS
#include <wchar.h>
#endif

#ifdef __UNICODE__
typedef wchar_t WCHAR;
typedef WCHAR Tchar, *PTchar;
typedef std::wifstream tifstream;
typedef std::wstringstream tstringstream;
typedef std::wstring tstring;
#define GEN_L(ARG) L##ARG
#else
typedef char Tchar, *PTchar;
typedef std::ifstream tifstream;
typedef std::stringstream tstringstream;
typedef std::string tstring;
#define GEN_L(ARG) ARG
#endif

int tstrcmp(const char *s1, const char *s2);

int tstrcmp(const wchar_t *s1, const wchar_t *s2);

char *tstrcpy(char *dst, std::size_t size, const char *src);

wchar_t *tstrcpy(wchar_t *dst, std::size_t size, const wchar_t *src);

char *tstrcat(char *dst, std::size_t size, const char *src);

wchar_t *tstrcat(wchar_t *dst, std::size_t size, const wchar_t *src);

std::size_t tstrlen(const char *s);

std::size_t tstrlen(const wchar_t *s);

int tsnprintf(char *s, std::size_t size, const char *format, ...);

int tsnprintf(wchar_t *wcs, size_t maxlen, const wchar_t *format, ...);

char *tstrlwr(char *s);

wchar_t *tstrlwr(wchar_t *s);

char *tstrupr(char *s);

wchar_t *tstrupr(wchar_t *s);

char *tstrstr(char *haystack, char *needle);

wchar_t *tstrstr(wchar_t *haystack, wchar_t *needle);

int tscanf(const char *s, const char *format, int *arg);

int tscanf(const wchar_t *s, const wchar_t *format, int *arg);

int tscanf(const char *s, const char *format, long *arg);

int tscanf(const wchar_t *s, const wchar_t *format, long *arg);

int tscanf(const char *s, const char *format, float *arg);

int tscanf(const wchar_t *s, const wchar_t *format, float *arg);