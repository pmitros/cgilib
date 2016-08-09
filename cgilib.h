/*
    cgilib.h - A simple library I wrote in high school to make it easier
    to write CGI scripts. It's ancient code, and probably sucks. Such is
    the nature of life...

    Uses some code snatched from NCSA at some point.

    Copyright (C) 1995, 1996, 2000 Piotr F. Mitros

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

    Contact: pmitros@mit.edu. After 2001, hopefully pmitros@alum.mit.edu :)
*/

#define LF 10
#define CR 13

typedef struct {
	 char *name;
	 char *val;
} entry;

char *makeword(char *line, char stop);
char *fmakeword(FILE *f, char stop, int *len);
void getword(char *word, char *line, char stop);
char x2c(char *what);
void unescape_url(char *url);
void plustospace(char *str);
void BeginPage(char*);
void EndPage();

// CGI functions
char* GetEntry(char* Entry);
void ReadCGIEntries();
void AddEntry(entry ce);

// Misc. functions
void ErrorMsg(char*Error);
void DebugMsg(char*Text);

void BeginPage(char*Title);
void EndPage();

