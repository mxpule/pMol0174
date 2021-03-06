/***************************************************************************
 *   Copyright (C) 2006 by Martin Pule   *
 *   martin@pulecyte.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*
 * Copyright (c) Medical Research Council 1994. All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * this copyright and notice appears in all copies.
 *
 * This file was written by James Bonfield, Simon Dear, Rodger Staden,
 * as part of the Staden Package at the MRC Laboratory of Molecular
 * Biology, Hills Road, Cambridge, CB2 2QH, United Kingdom.
 *
 * MRC disclaims all warranties with regard to this software.
 */

/*
 * Our own memory alloc routines that output error messages as appropriate
 * for us. Could also be done as macros, but hopefully there are no tight
 * using malloc many times so efficiency shouldn't be a problem.
 *
 * This also allows for dropping in a debugging malloc as we're intercepting
 * all alloc & free commands.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "error.h"

void *xmalloc(size_t size) {
    void *c = malloc(size);
    
    if (NULL == c) {
	errout("Not enough memory.\n");
	return NULL;
    }

    return c;
}

void *xrealloc(void *ptr, size_t size) {
    void *c;

    /*
     * realloc _should_ allocate memory for us when ptr is NULL.
     * Unfortunately this is not the case with the non-ANSI conformant
     * C library provided with SunOS4.1
     */
    if (ptr)
	c = realloc(ptr, size);
    else
	c = malloc(size);
    
    if (NULL == c) {
	errout("Not enough memory.\n");
	return NULL;
    }

    return c;
}

void *xcalloc(size_t num, size_t size) {
    void *c = calloc(num, size);

    if (NULL == c) {
	errout("Not enough memory.\n");
	return NULL;
    }

    return c;
}

void xfree(void *ptr) {
    free(ptr);
}
