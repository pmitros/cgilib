/*
    cgilib.cpp - A simple library I wrote in high school to make it easier 
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

// This is the CGI script for a TITLE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cgilib.h"

// CGI entries
entry *Entries;
int cEntries;


void DebugMsg(char*Text)
{
	FILE*file;
	file=fopen("d:\\www\\Debug", "a");
	fprintf(file, "\n%s", Text);
	fclose(file);
}

void BeginPage(char*Title)
{
	// Header
	printf("Content-type: text/html%c%c",LF,LF); // Specify return type
	puts("<HTML><Head>");
	printf("<Title>%s</Title>\n", Title);
	puts("</Head><Body>");
}

void EndPage()
{
	puts("<p><a href=\"/index.html\">Back to main page.</a></p>");
	puts("</Body></HTML>\n");
}


////////////////////////////////////////////////////////////
// Misc. Routines
void ErrorMsg(char*Error)
{
	printf("Content-type: text/html%c%c",LF,LF); // Specify return type
	puts("<HTML><Head>");
	puts("<Title>Error Message for CGI program</Title>");
	puts("</Head><Body>");
	puts("<h1>Error Message</h1>");
	puts("<h3>By Piotr Mitros</h3>");

	puts("<p>Congratulations!  You have either:<br>");
	puts("a) Found a mistake in a bullet-proof"
		" CGI Program by Piotr F. Mitros<br>");
	puts("\tor<br>");
	puts("b) Made a mistake</p>");
	puts("<br>");
	puts("<p>Your error is:</p><p>Drum roll, please: ");
	puts(Error);

	puts("</p></Body></HTML>\n");
	exit(0);
}

////////////////////////////////////////////////////////////
// CGI routines - debugged

char* GetEntry(char* Entry)
{
	int i=0;
	if(cEntries==0)
		return NULL;
	while(strcmp(Entries[i].name, Entry))
	{
		i++;

		if(i==cEntries)
			return NULL;
	}

	return Entries[i].val;
}

void ReadCGIEntries()
{
	cEntries=0;

	char*ReqMethod=getenv("REQUEST_METHOD");
	int PostMethod=strcmp(ReqMethod, "POST");
	if(PostMethod)
		return;

	if(strcmp(getenv("CONTENT_TYPE"), "application/x-www-form-urlencoded"))
		ErrorMsg("This application can only take form data.");

	// Yes, I know the standard says that CGI input doesn't have
	// to end with an EOF.  But it always does.  I hope.
	int cl = atoi(getenv("CONTENT_LENGTH"));

	for(int x=0;cl && (!feof(stdin));x++)
	{
		entry ce;  // Current entry
		ce.val = fmakeword(stdin,'&',&cl);
		plustospace(ce.val);
		unescape_url(ce.val);
		ce.name = makeword(ce.val,'=');
		AddEntry(ce);
	}
}

// This code is very slow.  It deallocates, copies, and
// reallocates the block of memory each iteration.  But
// who cares?  Its fast enough on any decent computer,
// and I'm (hopefully) not paying (much) for CPU time.

// Seriously, considering 20 entries max, it won't take
// too long.
void AddEntry(entry ce)
{
	++cEntries;
	Entries=(entry*)realloc(Entries, sizeof(entry)*cEntries);
	if(Entries==NULL)
		ErrorMsg("Out of Memory.  "
			"(On server side, there's nothing you can do about it)");

	memcpy(&Entries[cEntries-1], &ce, sizeof(entry));
}

//////////////////////////////////////////////////////////
// Misc. functions stolen from NCSA code on their web site, optimized a bit.

void getword(char *word, char *line, char stop)
{
	int x = 0,y;

	for(x=0;((line[x]) && (line[x] != stop));x++)
		word[x] = line[x];

	word[x] = '\0';
	if(line[x])
		++x;
	y=0;

	while(line[y++] = line[x++]);
}

char *makeword(char *line, char stop)
{
	int x = 0,y;
	char *word = (char *) malloc(sizeof(char) * (strlen(line) + 1));

	for(x=0;((line[x]) && (line[x] != stop));x++)
		word[x] = line[x];

	word[x] = '\0';
	if(line[x])
		++x;
	y=0;

	while(line[y++] = line[x++])
		NULL;

	return word;
}

char *fmakeword(FILE *f, char stop, int *cl)
{
	int wsize;
	char *word;
	int ll;

	wsize = 102400;
	ll=0;
	word = (char *) malloc(sizeof(char) * (wsize + 1));

	while(1)
	{
		word[ll] = (char)fgetc(f);
		if(ll==wsize)
		{
			word[ll+1] = '\0';
			wsize+=102400;
			word = (char *)realloc(word,sizeof(char)*(wsize+1));
		}

		--(*cl);
		if((word[ll] == stop) || (feof(f)) || (!(*cl)))
		{
			if(word[ll] != stop)
				ll++;
			word[ll] = '\0';
//			return word;
			return realloc(word, sizeof(char)*(ll+1));
		}
		++ll;
	}
}

char x2c(char *what)
{
	register char digit;

	digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
	digit *= 16;
	digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));

	return(digit);
}

void unescape_url(char *url)
{
	register int x,y;

	for(x=0,y=0;url[y];++x,++y)
	{
		if((url[x] = url[y]) == '%')
		{
			url[x] = x2c(&url[y+1]);
			y+=2;
		}
	}
	url[x] = '\0';
}

void plustospace(char *str)
{
	register int x;

	for(x=0;str[x];x++)
		if(str[x] == '+')
			str[x] = ' ';
}

int rind(char *s, char c)
{
	register int x;
	for(x=strlen(s) - 1;x != -1; x--)
		if(s[x] == c)
			return x;
	return -1;
}

int getline(char *s, int n, FILE *f)
{
	register int i=0;

	while(1)
	{
		s[i] = (char)fgetc(f);

		if(s[i] == CR)
			s[i] = fgetc(f);

		if((s[i] == 0x4) || (s[i] == LF) || (i == (n-1)))
		{
			s[i] = '\0';
			return (feof(f) ? 1 : 0);
		}
		++i;
	}
}

void send_fd(FILE *f, FILE *fd)
{
	int num_chars=0;
	char c;

	while (1)
	{
		c = fgetc(f);
		if(feof(f))
			return;
		fputc(c,fd);
	}
}

int ind(char *s, char c)
{
	register int x;

	for(x=0;s[x];x++)
		if(s[x] == c)
			return x;

	return -1;
}

void escape_shell_cmd(char *cmd)
{
	register int x,y,l;

	l=strlen(cmd);
	for(x=0;cmd[x];x++)
	{
		if(ind("&;`'\"|*?~<>^()[]{}$\\",cmd[x]) != -1)
		{
			for(y=l+1;y>x;y--)
				cmd[y] = cmd[y-1];
			l++; /* length has been increased */
			cmd[x] = '\\';
			x++; /* skip the character */
		}
	}
}


