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
#ifndef _MFILE_H_
#define _MFILE_H_

#include <stdio.h>

typedef struct {
    char *fname;
    FILE *fp;
    char *data;
    size_t alloced;
    int eof;
    size_t size;
    size_t offset;
    size_t flush_pos;
} mFILE;

mFILE *mfreopen(const char *path, const char *mode, FILE *fp);
mFILE *mfopen(const char *path, const char *mode);
int mfclose(mFILE *mf);
int mfdestroy(mFILE *mf);
int mfseek(mFILE *mf, long offset, int whence);
long mftell(mFILE *mf);
void mrewind(mFILE *mf);
void mftruncate(mFILE *mf, long offset);
int mfeof(mFILE *mf);
size_t mfread(void *ptr, size_t size, size_t nmemb, mFILE *mf);
size_t mfwrite(void *ptr, size_t size, size_t nmemb, mFILE *mf);
int mfgetc(mFILE *mf);
int mungetc(int c, mFILE *mf);
mFILE *mfcreate(char *data, int size);
void mfrecreate(mFILE *mf, char *data, int size);
char *mfgets(char *s, int size, mFILE *mf);
int mfflush(mFILE *mf);
int mfprintf(mFILE *mf, char *fmt, ...);
mFILE *mstdin(void);
mFILE *mstdout(void);
mFILE *mstderr(void);

#endif /* _MFILE_H_ */
