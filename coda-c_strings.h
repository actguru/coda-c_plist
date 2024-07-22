#pragma once
/* www.coda-c.com  coda-c_strings.h

Copyright (C) 2024 Stephen M. Jones

This file is part of Coda-C_PList.

Coda-C_PList is free software: you can redistribute it and/or modify it
under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

Coda-C_PList is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Coda-C_PList. If not, see <https://www.gnu.org/licenses/>.

*/

	#include <string.h>

	#define cs_length(string)	((int)strlen(string))
	#define cs_exact(str1,str2)	(0==strcmp(str1,str2))
	#define cs_strcopy(str1,str2)	strcpy(str1,str2)
	#define cs_blockCopy(dest,sour,nel) memcpy(dest,sour,nel)
	#define cs_blockFill(dest,chr,nel) memset(dest,chr,nel)
	#define cs_blockCmp(a,b,nel) memcmp(a,b,nel)
	#define cs_blockZero(addr,nel) cs_blockFill(addr,0,nel)
	#define alocS(size)  cs_blockZero(alloca(size),size);

	#define cc_inString(c,str) ((-1)!=cc_pos(c,str))
	#define cs_inString(key,str) ((-1)!=cs_pos(key,str))
bool cc_isCap(int c);
bool cc_isLow(int c);
bool cc_isAlpha(int c);
bool cc_is09(int c);
bool cc_isWhite(int c);
int cc_toCap(int c);
int cc_toLow(int c);
int cc_pos(char c,const char *str);
int cc_posCap(char c,const char *str);
int cc_revPos(char c,const char *str);
bool cs_prefix(const char *key,const char *str);
bool cs_prefixCap(const char *key,const char *str);
void cs_copy(char *dest,const char *sour);
bool cs_exactCap(const char *str1,const char *str2);
int cs_pos(const char *key,const char *str);
int cs_posCap(const char *key,const char *str);
int4 cs_toInt4(const char *string);
double cs_toReal(const char *string);
Array Char_Split(char* string,char* delimiter);
huge cs_toHuge(const char* str);
void cs_rightTrim(char *str);
void cs_leftTrim(char *str);
void cs_trim(char *str);
int file_getCs(FILE *f,char *s,int mx);

