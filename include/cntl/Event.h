// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef CNTL_EVENT_H_INCLUDED
#define CNTL_EVENT_H_INCLUDED

//
// Win32 Event Object Wrapper.
//
#if defined(__APPLE__) || defined(__linux__)
# include "EventPthread.h"
#elif defined(WIN32)
# include "EventWindows.h"
#endif

#endif // CNTL_EVENT_H_INCLUDED
