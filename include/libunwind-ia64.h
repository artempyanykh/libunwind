/* libunwind - a platform-independent unwind library
   Copyright (C) 2001-2002 Hewlett-Packard Co
	Contributed by David Mosberger-Tang <davidm@hpl.hp.com>

This file is part of libunwind.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#ifndef LIBUNWIND_H
#define LIBUNWIND_H

#include <stdint.h>
#include <ucontext.h>

#define UNW_TARGET	ia64

typedef uint64_t unw_tdep_word_t;

typedef enum
  {
    /* Note: general registers are excepted to start with index 0.
       This convention facilitates architecture-independent
       implementation of the C++ exception handling ABI.  See
       _Unwind_SetGR() and _Unwind_GetGR() for details.  */
    UNW_IA64_GR = 0,			/* general registers (r0..r127) */
     UNW_IA64_GP = UNW_IA64_GR + 1,
     UNW_IA64_TP = UNW_IA64_GR + 13,

    UNW_IA64_NAT = UNW_IA64_GR + 128,	/* NaT registers (nat0..nat127) */

    UNW_IA64_FR = UNW_IA64_NAT + 128,	/* fp registers (f0..f127) */

    UNW_IA64_AR = UNW_IA64_FR + 128,	/* application registers (ar0..r127) */
     UNW_IA64_AR_RSC = UNW_IA64_AR + 16,
     UNW_IA64_AR_BSP = UNW_IA64_AR + 17,
     UNW_IA64_AR_BSPSTORE = UNW_IA64_AR + 18,
     UNW_IA64_AR_RNAT = UNW_IA64_AR + 19,
     UNW_IA64_AR_25 = UNW_IA64_AR + 25,	/* reserved (scratch) */
     UNW_IA64_AR_26 = UNW_IA64_AR + 26,	/* reserved (scratch) */
     UNW_IA64_AR_CCV = UNW_IA64_AR + 32,
     UNW_IA64_AR_UNAT = UNW_IA64_AR + 36,
     UNW_IA64_AR_FPSR = UNW_IA64_AR + 40,
     UNW_IA64_AR_PFS = UNW_IA64_AR + 64,
     UNW_IA64_AR_LC = UNW_IA64_AR + 65,
     UNW_IA64_AR_EC = UNW_IA64_AR + 66,

    UNW_IA64_BR = UNW_IA64_AR + 128,	/* branch registers (b0..p7) */
    UNW_IA64_PR = UNW_IA64_BR + 8,	/* predicate registers (p0..p63) */
    UNW_IA64_CFM,

    /* frame info (read-only): */
    UNW_IA64_BSP,
    UNW_IA64_IP,
    UNW_IA64_SP,
    UNW_IA64_PROC_START,
    UNW_IA64_HANDLER,
    UNW_IA64_LSDA,

    UNW_TDEP_LAST_REG = UNW_IA64_LSDA,

    UNW_TDEP_IP = UNW_IA64_IP,
    UNW_TDEP_SP = UNW_IA64_SP,
    UNW_TDEP_PROC_START = UNW_IA64_PROC_START,
    UNW_TDEP_HANDLER = UNW_IA64_HANDLER,
    UNW_TDEP_LSDA = UNW_IA64_LSDA,
  }
ia64_regnum_t;

/* Info needed for a single IA-64 unwind.  A pointer to this structure
   is expected in the acquire/release callbacks of the unwind
   accessors.  */
typedef struct unw_ia64_table
  {
    const char *name;		/* table name (or NULL if none) */
    unw_tdep_word_t segbase;	/* base for offsets in the unwind table */
    unw_tdep_word_t start;	/* starting IP covered by table */
    unw_tdep_word_t end;	/* first IP _not_ covered table */
    unw_tdep_word_t gp;		/* global pointer for this load-module */
    unw_tdep_word_t length;	/* number of entries in unwind table array */

    /* Pointer to local copy of the unwind descriptor table: */
    void *array;

    /* Local copy of the unwind descriptor information.  This is
       initialized such that adding the unwind entry's info_offset
       yields the address at which the corresponding descriptors can
       be found.  */
    const unsigned char *unwind_info_base;
  }
unw_ia64_table_t;

typedef struct unw_tdep_save_loc
  {
    /* Additional target-dependent info on a save location.  On IA-64,
       we could use this to specify the bit number in which a NaT bit
       gets saved.  For now, nobody wants to know this, so it's not
       currently implemented.  */
  }
unw_tdep_save_loc_t;

/* On IA-64, we can directly use ucontext_t as the unwind context.  */
typedef ucontext_t unw_tdep_context_t;

/* XXX this is not ideal: an application should not be prevented from
   using the "getcontext" name just because it's using libunwind.  We
   can't just use __getcontext() either, because that isn't exported
   by glibc...  */
#define unw_tdep_getcontext(uc)		(getcontext (uc), 0)

#define unw_tdep_is_fpreg(r)		((unsigned) ((r) - UNW_IA64_FR) < 128)

#include "libunwind-common.h"

#endif /* LIBUNWIND_H */
