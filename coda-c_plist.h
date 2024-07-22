#pragma once
/* www.coda-c.com  coda-c_plist.h

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
	#include <stdlib.h>
	#include <stdbool.h>
	#include <stdint.h>
	typedef struct tm struct_tm;
	#include <stdio.h>

	typedef void*     Obj;
	typedef void*     pointer;
	typedef long long huge;
	typedef int32_t   int4;
	typedef char*     Char;
	typedef char*     ConstChar;

	#define $CLEANUP(fun) __attribute__((cleanup (fun)))
	#define $CONSUMED
	#define $FORMAT12     __attribute__((format (coda_printf, 1, 2)))

	#if WIN32
		#define coda_printf gnu_printf
	#else
		#define coda_printf printf
	#endif

	#define OBind1_2(class,fun) class##_##fun
	#define OBind1_2M(a,b) OBind1_2(a,b)
	#define OClass_(x) Class_ ## x
	#define OClass(x) OClass_(x)
	#define CodaClassDef(clas,stor,...) typedef stor* clas; \
		extern clas OBind1_2(Class,clas);
	#define Array_add(aaa,ooo)  Array_addObject(aaa,ooo)
	#define Array_take(aaa,ooo) Array_takeObject(aaa,ooo)
	#define Dict Dictionary
	#define Dict_set Dictionary_setKey
	#define Dict_take Dictionary_takeKey
	#define Os_(str) "\0\0" str
	#define Os(str)			((Os_(str))+2)
CodaClassDef(Array,struct Array_,Root);
CodaClassDef(Char,char,Root);
CodaClassDef(ConstChar,char,Char);
CodaClassDef(Dictionary,struct Dictionary_,Root);
CodaClassDef(Keyword,struct Keyword_,Root);
CodaClassDef(Pointer,pointer,Root);
CodaClassDef(Root,void,0);
CodaClassDef(Void,void,0);
	#define Array_sub Array_subInt
	#define isa_(obj,CLASS) OisaClass(obj,OClass(CLASS))
	#define Dict_sub Dictionary_subKey
	struct Keyword_ { Obj item; char word[0]; };
	#define cleanO 				const $CLEANUP(Memory_clean)
	#define	newO(class)			Memory_newO(OClass(class),1)
	#define	newOC(class,cnt)	Memory_newO(OClass(class),cnt)
	#define alocO(size)			Memory_alocO(size)
	#define keepO               Memory_keep
	#define freeO               Memory_free
	#define countO(obj)			Memory_count(obj)
	#define sizeO(obj)			Memory_size(obj)
	#define kindO(obj)			Memory_kind(obj)
	#define Pointer_count Root_get_count
void Array_removeBlock(Array self,int dix,int count);
void Array_takeBlock(Array self,int index,int count,pointer block);
int Array_get_count(Array self);
#define Array_count Array_get_count
Obj Array_subInt(Array self,int ix);
pointer Array_rawAddress(Array self);
Array Array_NewBlock(Array proto,int count,pointer block);
void Array_insertBlock(Array self,int index,int count,pointer block);
Obj Array_insertAt(Array self,int index,Obj obj);
void Array_removeAt(Array self,int dix);
Obj Array_addObject(Array self,Obj obj);
void Array_takeObject(Array self,Obj $CONSUMED obj);
void Array_removeLast(Array self);
Obj Array_replaceAt(Array self,int ix,Obj obj);
int Array_removeObject(Array self,Obj object);
void Array_removeAll(Array self);
void Array_toaSet(Array self);
bool Array_isaSet(Array self);
Char Char_Value(const char *string);
Char $FORMAT12 Char_F(char *cs,...);
bool OisaClass(Obj obj,Obj classobj);
int Dictionary_get_count(Dictionary self);
#define Dictionary_count Dictionary_get_count
void Dictionary_setKey(Dictionary self,char *key,Obj obj);
bool Dictionary_removeKey(Dictionary self,char *key);
int Dictionary_removeObject(Dictionary self,Obj obj);
Obj Dictionary_subKey(Dictionary self,char *key);
Pointer Dictionary_AllKeys(Dictionary self);
void Dictionary_takeKey(Dictionary self,char *key,Obj $CONSUMED obj);
Keyword Dictionary_scan(Dictionary dict);
Keyword Dictionary_next(Keyword element);
Keyword Keyword_Value(char *word,Obj item);
Obj Memory_alocO(int size);
extern _Atomic huge Object_Counter;
void Memory_free(Obj obj);
Obj Memory_keep(Obj obj);
void Memory_clean(const void *vp);
char* Memory_kind(Obj obj);
int4 Memory_size(Obj obj);
int4 Memory_count(Obj obj);
Obj Memory_newO(Obj obj,int nel);
Pointer Pointer_Value(pointer value);
int Root_get_count(Root self);
#define Root_count Root_get_count
Char ToContainer(Obj container);
void codac_versions(void);
CodaClassDef(Bool,_Bool,Root);
CodaClassDef(Data,void,Root);
CodaClassDef(DateString,char,Char);
CodaClassDef(Huge,huge,Root);
CodaClassDef(HugeUID,huge,Huge);
CodaClassDef(Real,double,Root);
 enum {
	PLIST_UnsortedDict=1,
	PLIST_NL4Leafs=2,
	PLIST_AddComputer=4,
	PLIST_Amp38=8,
	PLIST_Apple=16,
	PLIST_NoEncoding=32,
	PLIST_NoDoctype=64,
	PLIST_NoPVersion=128,
	PLIST_Binary=2048,
	};
Bool Bool_Value(bool value);
Data Data_NewBlock(Data self,int count,pointer address);
Data Data_FromFile(char *path);
DateString DateString_Value(char *string);
DateString DateString_FromGmtime(struct_tm *when);
struct_tm* DateString_toGmtime(DateString self);
pointer Gmtime_check(struct_tm *self);
Huge Huge_Value(huge value);
HugeUID HugeUID_Value(huge value);
Char OError(void);
int4 PList_lastLoadType(void);
Obj PList_FromBlock(int count,pointer block,int flags);
Obj PList_toStream(Obj stream,Obj container,int flags);
Obj PList_save(char *file,Obj container,int flags);
Obj PList_Load(char *file,int flags);
void PList_Binary(void);
void pointer_sort(pointer base,int nel,void *IfunVVC,void *context);
Real Real_Value(double value);
	#define defer_call(fun,ptr) $CLEANUP(defer_call_cleanup) \
		pointer OBind1_2M(defer_,__LINE__)[2]={fun,ptr}; \
		Object_leaks_NOP(OBind1_2M(defer_,__LINE__))
 #define Object_Leaks() $CLEANUP(Object_leaks_cleaner) huge OBind1_2M(leaks,__LINE__)[3]= \
	  {Object_Counter,__LINE__,(huge)__func__}; Object_leaks_NOP( OBind1_2M(leaks,__LINE__) )
void defer_call_cleanup(void *vp);
Char obj_xmlTag(Obj obj);
Char obj_ToString(Obj obj);
Char obj_FromString(Obj obj,char *string);
void Object_leaks_cleaner(void *vp);
void Object_leaks_NOP(pointer junk);

