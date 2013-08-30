/*
 * Copyright (C) 2004, 2005, 2006, 2013 Apple Inc.
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2007-2009 Torch Mobile, Inc.
 * Copyright (C) 2010, 2011 Research In Motion Limited. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

/* Include compiler specific macros */
#include "wtf/Compiler.h"

/* ==== Platform adaptation macros: these describe properties of the target environment. ==== */

/* HAVE() - specific system features (headers, functions or similar) that are present or not */
#define HAVE(WTF_FEATURE) (defined HAVE_##WTF_FEATURE  && HAVE_##WTF_FEATURE)
/* OS() - underlying operating system; only to be used for mandated low-level services like
   virtual memory, not to choose a GUI toolkit */
#define OS(WTF_FEATURE) (defined WTF_OS_##WTF_FEATURE  && WTF_OS_##WTF_FEATURE)

/* ==== Policy decision macros: these define policy choices for a particular port. ==== */

/* USE() - use a particular third-party library or optional OS service */
#define USE(WTF_FEATURE) (defined WTF_USE_##WTF_FEATURE  && WTF_USE_##WTF_FEATURE)
/* ENABLE() - turn on a specific feature of WebKit */
#define ENABLE(WTF_FEATURE) (defined ENABLE_##WTF_FEATURE  && ENABLE_##WTF_FEATURE)

/* ==== OS() - underlying operating system; only to be used for mandated low-level services like
   virtual memory, not to choose a GUI toolkit ==== */

/* OS(ANDROID) - Android */
#ifdef ANDROID
#define WTF_OS_ANDROID 1
#endif

/* OS(DARWIN) - Any Darwin-based OS, including Mac OS X and iPhone OS */
#ifdef __APPLE__
#define WTF_OS_DARWIN 1
#endif

/* OS(FREEBSD) - FreeBSD */
#if defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#define WTF_OS_FREEBSD 1
#endif

/* OS(LINUX) - Linux */
#ifdef __linux__
#define WTF_OS_LINUX 1
#endif

/* OS(NETBSD) - NetBSD */
#if defined(__NetBSD__)
#define WTF_OS_NETBSD 1
#endif

/* OS(OPENBSD) - OpenBSD */
#ifdef __OpenBSD__
#define WTF_OS_OPENBSD 1
#endif

/* OS(WINDOWS) - Any version of Windows */
#if defined(WIN32) || defined(_WIN32)
#define WTF_OS_WINDOWS 1
#endif

/* OS(UNIX) - Any Unix-like system */
#if OS(ANDROID)          \
    || OS(DARWIN)           \
    || OS(FREEBSD)          \
    || OS(LINUX)            \
    || OS(NETBSD)           \
    || OS(OPENBSD)          \
    || defined(unix)        \
    || defined(__unix)      \
    || defined(__unix__)
#define WTF_OS_UNIX 1
#endif

/* Operating environments */

#if OS(ANDROID)
#define WTF_USE_LOW_QUALITY_IMAGE_INTERPOLATION 1
#define WTF_USE_LOW_QUALITY_IMAGE_NO_JPEG_DITHERING 1
#define WTF_USE_LOW_QUALITY_IMAGE_NO_JPEG_FANCY_UPSAMPLING 1
#else
#define WTF_USE_ICCJPEG 1
#define WTF_USE_QCMSLIB 1
#endif

#if OS(DARWIN)
#define WTF_USE_CF 1

/* We can't override the global operator new and delete on OS(DARWIN) because
 * some object are allocated by WebKit and deallocated by the embedder. */
#else /* !OS(DARWIN) */
/* On non-OS(DARWIN), the "system malloc" is actually TCMalloc anyway, so there's
 * no need to use WebKit's copy of TCMalloc. */
#define WTF_USE_SYSTEM_MALLOC 1
#endif /* OS(DARWIN) */

#if !defined(HAVE_ACCESSIBILITY)
#define HAVE_ACCESSIBILITY 1
#endif /* !defined(HAVE_ACCESSIBILITY) */

#if OS(UNIX)
#define HAVE_MMAP 1
#define HAVE_SIGNAL_H 1
#define HAVE_SYS_TIME_H 1
#define WTF_USE_PTHREADS 1
#endif /* OS(UNIX) */

#if !defined(HAVE_VASPRINTF)
#if !COMPILER(MSVC)
#define HAVE_VASPRINTF 1
#endif
#endif

#if !OS(WINDOWS) && !OS(ANDROID)
#define HAVE_TM_GMTOFF 1
#define HAVE_TM_ZONE 1
#define HAVE_TIMEGM 1
#endif

#if OS(DARWIN)
#define HAVE_DISPATCH_H 1
#define HAVE_PTHREAD_SETNAME_NP 1
#endif /* OS(DARWIN) */

#if OS(WINDOWS)

// If we don't define these, they get defined in windef.h.
// We want to use std::min and std::max.
#ifndef max
#define max max
#endif
#ifndef min
#define min min
#endif

#endif /* OS(WINDOWS) */

#ifdef __cplusplus

// These undefs match up with defines in WebCorePrefixMac.h for Mac OS X.
// Helps us catch if anyone uses new or delete by accident in code and doesn't include "config.h".
#undef new
#undef delete
#include "wtf/FastMalloc.h"

#include <ciso646>

#endif

#if COMPILER(MSVC)
#define SKIP_STATIC_CONSTRUCTORS_ON_MSVC 1
#else
#define SKIP_STATIC_CONSTRUCTORS_ON_GCC 1
#endif

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS 1
#endif
