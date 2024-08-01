/* www.coda-c.com  plist10gen.c

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
	#define QWebsite "www.coda-c.com"

	#define _GNU_SOURCE 1

	#if     WIN32
		#define malloc_size(xxx)	_msize(xxx)
		#define timegm _mkgmtime
	#elif __linux__
		#include <malloc.h>
		#define malloc_size(xxx)	malloc_usable_size(xxx)
	#else
		#include <malloc/malloc.h>
	#endif

	#include <stdatomic.h>
	#include <stdarg.h>
	#include <stddef.h>
	#include <stdio.h>
	#include <math.h>
	#include <time.h>
	#include <sys/stat.h>
	#include <limits.h>
	#include <errno.h>
	#include <assert.h>
		#undef  assert
		#define assert Assert_

	static_assert(EOF==(-1),"EOF has a bad value.");

	#define ei else if
	#define $(sig,...) OBind1_2M(class,sig)(class self,##__VA_ARGS__)
	#define _$(sig,...) OBind1_2M(class,sig)(self,##__VA_ARGS__)
	#define _$2(sig,...) OBind1_2M(class,sig)(self,__VA_ARGS__)
	#define _ self->

	#include <string.h>
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

	#define $CONSTRUCTOR  __attribute__((constructor))
	#define $CLEANUP(fun) __attribute__((cleanup (fun)))
	#define $CONSUMED
	#define $FORMAT12     __attribute__((format (coda_printf, 1, 2)))
	#define $FORMAT23     __attribute__((format (coda_printf, 2, 3)))
	#define $NORETURN     __attribute__((__noreturn__))

	#if     WIN32
		#define coda_printf gnu_printf
	#elif __linux__
		#define LINUX 1
		#define coda_printf printf
	#else
		#define APPLE 1
		#define coda_printf printf
	#endif

	#define OBind1_2(class,fun) class##_##fun
	#define OBind1_2M(a,b) OBind1_2(a,b)
	#define OClass_(x) Class_ ## x
	#define OClass(x) OClass_(x)

	#define CodaSig(fun)       codac_sig(OClass(class),OBind1_2(sig,fun),OBind1_2M(class,fun))
	#define CodaAka(fun,clas2) codac_sig(OClass(class),OBind1_2(sig,fun),OBind1_2(clas2,fun))
	#define OSig(sig)			(*sig)
	#define sig_(name) ConstChar OBind1_2(sig,name)=Os(#name)

	#define $class(name) OBind1_2M(class,name)
	#define $boot(name) static $CONSTRUCTOR void $class(name)()

	typedef void (*DTOR)(pointer,pointer);

	#define CodaClassDef(clas,stor,...) typedef stor* clas; \
		extern clas OBind1_2(Class,clas); \
		extern CodaCLASS OBind1_2(Isa,clas)

	typedef struct CodaCLASS_ CodaCLASS;

	struct CodaCLASS_ {
		DTOR dtor;
		char *kClass;
		CodaCLASS *superClass;
		DTOR etor;
		int  size;
		short superNel;
		short bits;
		DTOR itor;
		int  kize,spare;
		pointer classObject;
		pointer custom;
		DTOR ekeep;
		struct Dictionary_* properties;
		pointer spare2,spare3,spare4;
		};

	#define	CodaMetaSize	(sizeof(void *)+8)
	#define CodaCOSize 64
	struct CodaPublicMeta_ {
		CodaCLASS *clas;
		char padding[CodaMetaSize-sizeof(pointer)-sizeof(unsigned short)];
		unsigned short metasize;
		};
	struct CodaPublicCORaw_ {
		struct CodaPublicMeta_ meta;
		char COData[CodaCOSize];
		};
	enum { CodaClass_SUPERNEL=1, CodaClass_CUSTOM=0 };
	#define ORegisterClass(CLASS) static $CONSTRUCTOR void OBind1_2(com_coda_c_www,CLASS)() { \
			Class_register(OClass(CLASS)); }
	#define CodaClass(CLASS,CodaType,super,...) Coda_Class(CLASS,CodaType,&Isa_##super)
	#define Coda_Class(CLASS,CodaType,SUPER,...) \
		static struct CodaPublicCORaw_ OBind1_2(Meta,CLASS); \
		CodaCLASS OBind1_2(Isa,CLASS)={ \
			(DTOR)OBind1_2(CLASS,dtor), \
			#CLASS,\
			SUPER, \
			(DTOR)OBind1_2(CLASS,etor), \
			sizeof(CodaType), \
			CodaClass_SUPERNEL, \
			OBind1_2(CLASS,bits), \
			(DTOR)OBind1_2(CLASS,itor), \
			OBind1_2(CLASS,kize), \
			0, \
			&OBind1_2(Meta,CLASS).COData, \
			(pointer)CodaClass_CUSTOM, \
			(DTOR)OBind1_2(CLASS,ekeep), \
			}; \
		static struct CodaPublicCORaw_ OBind1_2(Meta,CLASS)={{&OBind1_2(Isa,CLASS),"",CodaMetaSize},""}; \
		CLASS OClass(CLASS)=(pointer)(&OBind1_2(Meta,CLASS).COData); \
		ORegisterClass(CLASS)

	enum { bits_Root = -1, bits_Trans =	-2 };

	#define	sizeat(type) sizeof(*((type)0))

	#define cs_length(string)	((int)strlen(string))
	#define cs_exact(str1,str2)	(0==strcmp(str1,str2))
	#define cs_strcopy(str1,str2)	strcpy(str1,str2)
	#define cs_blockCopy(dest,sour,nel) memcpy(dest,sour,nel)
	#define cs_blockFill(dest,chr,nel) memset(dest,chr,nel)
	#define cs_blockCmp(a,b,nel) memcmp(a,b,nel)
	#define cs_blockZero(addr,nel) cs_blockFill(addr,0,nel)
	#define alocS(size)  cs_blockZero(alloca(size),size);
#define CODA_shift(                          _1,_2,_3,_4,_5,_6,_7,_8,_9, N ,...) N
#define CODA_pad(...) CODA_shift(__VA_ARGS__,_9,_8,_7,_6,_5,_4,_3,_2,_1,_0)
#define OBind123_(one,two,three) one##two##three
#define OBind123(one,two,three) OBind123_(one,two,three)

#define CODA_1_Z(fun,eva,a)                 fun(#a,eva(a))
#define CODA_2_Z(fun,eva,a,b)               fun(#a,eva(a)), fun(#b,eva(b))
#define CODA_3_Z(fun,eva,a,b,c)             fun(#a,eva(a)), fun(#b,eva(b)), fun(#c,eva(c))
#define CODA_4_Z(fun,eva,a,b,c,d)           fun(#a,eva(a)), fun(#b,eva(b)), fun(#c,eva(c)), fun(#d,eva(d))
#define CODA_5_Z(fun,eva,a,b,c,d,e)         fun(#a,eva(a)), fun(#b,eva(b)), fun(#c,eva(c)), fun(#d,eva(d)), \
	fun(#e,eva(e))
#define CODA_6_Z(fun,eva,a,b,c,d,e,f)       fun(#a,eva(a)), fun(#b,eva(b)), fun(#c,eva(c)), fun(#d,eva(d)), \
	fun(#e,eva(e)), fun(#f,eva(f))
#define CODA_7_Z(fun,eva,a,b,c,d,e,f,g)     fun(#a,eva(a)), fun(#b,eva(b)), fun(#c,eva(c)), fun(#d,eva(d)), \
	fun(#e,eva(e)), fun(#f,eva(f)), fun(#g,eva(g))
#define CODA_8_Z(fun,eva,a,b,c,d,e,f,g,h)   fun(#a,eva(a)), fun(#b,eva(b)), fun(#c,eva(c)), fun(#d,eva(d)), \
	fun(#e,eva(e)), fun(#f,eva(f)), fun(#g,eva(g)), fun(#h,eva(h))
#define CODA_9_Z(fun,eva,a,b,c,d,e,f,g,h,i) fun(#a,eva(a)), fun(#b,eva(b)), fun(#c,eva(c)), fun(#d,eva(d)), \
	fun(#e,eva(e)), fun(#f,eva(f)), fun(#g,eva(g)), fun(#h,eva(h)), fun(#i,eva(i))
#define CODA_macro(macro, ...) macro(__VA_ARGS__)
#define CODA_Zx(fun,eva,...) CODA_macro(OBind123(CODA,CODA_pad(__VA_ARGS__),_Z),fun,eva,__VA_ARGS__)
#define CODA_1_F(fun,a)                 fun(a)
#define CODA_2_F(fun,a,b)               fun(a), fun(b)
#define CODA_3_F(fun,a,b,c)             fun(a), fun(b), fun(c)
#define CODA_4_F(fun,a,b,c,d)           fun(a), fun(b), fun(c), fun(d)
#define CODA_5_F(fun,a,b,c,d,e)         fun(a), fun(b), fun(c), fun(d), fun(e)
#define CODA_6_F(fun,a,b,c,d,e,f)       fun(a), fun(b), fun(c), fun(d), fun(e), fun(f)
#define CODA_7_F(fun,a,b,c,d,e,f,g)     fun(a), fun(b), fun(c), fun(d), fun(e), fun(f), fun(g)
#define CODA_8_F(fun,a,b,c,d,e,f,g,h)   fun(a), fun(b), fun(c), fun(d), fun(e), fun(f), fun(g), fun(h)
#define CODA_9_F(fun,a,b,c,d,e,f,g,h,i) fun(a), fun(b), fun(c), fun(d), fun(e), fun(f), fun(g), fun(h), fun(i)
#define CODA_ENUM(fun,...) enum { CODA_macro(OBind123(CODA,CODA_pad(__VA_ARGS__),_F),fun,__VA_ARGS__) }
#define CodaClassZero1(x) OBind1_2M(class,x)=0
#define CodaClassZeros(...) CODA_ENUM(CodaClassZero1,__VA_ARGS__)

#define CodaClassZerosC() CodaClassZeros(dtor,itor,kize,etor,ekeep,bits)
#define CodaClass_Trans() enum { OBind1_2M(class,bits)=bits_Trans }
#define CodaClass_Root()  enum { OBind1_2M(class,bits)=bits_Root }
#define CodaClassTransC() CodaClassZeros(dtor,itor,kize,etor,ekeep); CodaClass_Trans()

	#define Array_add(aaa,ooo)  Array_addObject(aaa,ooo)
	#define Array_take(aaa,ooo) Array_takeObject(aaa,ooo)
	#define obj_(SEL,obj,...) ((SEL)ORespond2(obj,sig_##SEL,0,1))(OObject(obj,sig_##SEL),##__VA_ARGS__)
	#define OResponds(obj,SEL) ORespond2(obj,sig_##SEL,0,0)
	#define Dict Dictionary
	#define Dict_set Dictionary_setKey
	#define Dict_take Dictionary_takeKey
	#define Os_(str) "\0\0" str
	#define Os(str)			((Os_(str))+2)
CodaClassDef(CArray,struct Array_,Root);
	extern ConstChar sig_addObject;
	extern ConstChar sig_subInt;
	extern ConstChar sig_removeObject;
	extern ConstChar sig_removeLast;
	extern ConstChar sig_removeAll;
	extern ConstChar sig_ToDelimiter;
	extern ConstChar sig_ToStringSub;
	extern ConstChar sig_NewBlock;
CodaClassDef(Array,struct Array_,Root);
	extern ConstChar sig_ToString;
	extern ConstChar sig_FromString;
CodaClassDef(Char,char,Root);
CodaClassDef(ConstChar,char,Char);
	extern ConstChar sig_setKey;
	extern ConstChar sig_subKey;
	extern ConstChar sig_removeKey;
	extern ConstChar sig_AllKeys;
	extern ConstChar sig_xmlTag;
CodaClassDef(Dictionary,struct Dictionary_,Root);
CodaClassDef(CDictionary,struct Dictionary_,Root);
CodaClassDef(Keyword,struct Keyword_,Root);
CodaClassDef(Pointer,pointer,Root);
	extern ConstChar sig_get_count;
	extern ConstChar sig_Info;
	extern ConstChar sig_set_name;
	extern ConstChar sig_get_name;
CodaClassDef(Root,void,0);
CodaClassDef(Void,void,0);
	typedef Obj  OSig(addObject)(Obj array,Obj obj) ;
	typedef Obj  OSig(subInt)(Obj array,int ix) ;
	typedef int  OSig(removeObject)(Obj array,Obj obj) ;
	typedef void OSig(removeLast)(Obj array) ;
	typedef void OSig(removeAll)(Obj array) ;
	typedef Char OSig(ToDelimiter)(Obj container,int index) ;
	typedef Char OSig(ToStringSub)(Obj container,int index) ;
	typedef Obj  OSig(NewBlock)(Obj proto,int count,pointer data) ;
	typedef Char OSig(ToString)(Obj obj);
	typedef Obj  OSig(FromString)(Obj proto,char *str);
	typedef void OSig(setKey)(Obj dict,char *key,Obj obj) ;
	typedef Obj  OSig(subKey)(Obj dict,char *key) ;
	typedef bool OSig(removeKey)(Obj dict,char *key) ;
	typedef Pointer OSig(AllKeys)(Obj dict) ;
	typedef Char OSig(xmlTag)(Obj obj);
	typedef int   OSig(get_count)(Obj obj) ;
	typedef Char  OSig(Info)(Obj obj) ;
	typedef void  OSig(set_name)(Obj obj,char* name) ;
	typedef Char  OSig(get_name)(Obj obj) ;
	#define cleanC const $CLEANUP(clean_C)
	#define Array_sub Array_subInt
	#define CodaCOSize 64
	#define isa_(obj,CLASS) OisaClass(obj,OClass(CLASS))
	#define Dict_sub Dictionary_subKey
	#define Dictionary_name Dictionary_get_name
	struct Keyword_ { Obj item; char word[0]; };
	#define cleanO 				const $CLEANUP(Memory_clean)
	#define	newO(class)			Memory_newO(OClass(class),1)
	#define	newOC(class,cnt)	Memory_newO(OClass(class),cnt)
	#define alocO(size)			Memory_alocO(size)
	#define keepO               Memory_keep
	#define freeO               Memory_free
	#define countO(obj)			Memory_count(obj)
	#define sizeO(obj)			Memory_size(obj)
	#define classO(obj)			Memory_class(obj)
	#define kindO(obj)			Memory_kind(obj)
	#define	superO(obj)			Memory_superObject(obj)
 #define Msg_(...) Msg_Object(Char_F(__VA_ARGS__),0)
 #define Log_(...) Msg_Object(Char_F(__VA_ARGS__),1)
	#define Quit_(cs,...) Die_Object(Char_F(cs,##__VA_ARGS__),0)
	#define Die_(cs,...) Die_Object(Char_F("Die:%s: " cs,__func__,##__VA_ARGS__),1)
	#define Assert_(e) ((void)((e)?0:Die_Object(Char_F("%s: !True(%s)",__func__,#e),1)))
	#define Pointer_count Root_get_count
void clean_C(const void *vp);
pointer alocC(int4 size);
void freeC(pointer address);
void alocC_accountForMalloc(pointer address);
huge alocC_count(bool allocation,bool bytes);
void alocC_reset(void);
void alocC_counting(bool enable);
extern double Array_FACTOR;
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
void Array_set_name(Array self,char* name);
Char Array_get_name(Array self);
#define Array_name Array_get_name
extern ConstChar Os_Set;
void Array_toaSet(Array self);
bool Array_isaSet(Array self);
Char Array_Join(Array array,char *delimiter);
Char Char_Value(const char *string);
Char $FORMAT12 Char_F(char *cs,...);
Char Char_NewBlock(Char self,int count,pointer address);
CDictionary Class_dictionary(void);
void Class_register(Obj obj);
Char CMux_Diag(pointer cmux,char *name);
Char Dictionary_Diag(Dictionary dict);
Char Array_Diag(Array array);
bool OisaClass(Obj obj,Obj classobj);
void *ORespond2(void *obj,char *sel,CodaCLASS **cpp,int diag);
Obj OObject(Obj obj,char *sel);
int Dictionary_get_count(Dictionary self);
#define Dictionary_count Dictionary_get_count
void Dictionary_setKey(Dictionary self,char *key,Obj obj);
bool Dictionary_removeKey(Dictionary self,char *key);
int Dictionary_removeObject(Dictionary self,Obj obj);
Obj Dictionary_subKey(Dictionary self,char *key);
Pointer Dictionary_AllKeys(Dictionary self);
void Dictionary_set_name(Dictionary self,char* name);
Char Dictionary_get_name(Dictionary self);
#define Dictionary_name Dictionary_get_name
void Dictionary_takeKey(Dictionary self,char *key,Obj $CONSUMED obj);
void Dictionary_auto(Dictionary dict);
int4 Dictionary_autoDisable(Dictionary dict,bool disable);
void Dictionary_hash_default(Dictionary dict);
void Dictionary_hash(Dictionary dict,int4 (*hasher)(const char *string));
void Dictionary_hash_build(Dictionary dict);
bool DictHash_keyobj(Dictionary dict,char *key,pointer obj);
bool DictHash_keydel(Dictionary dict,char *key);
Obj DictHash_obj4key(Dictionary dict,char *key);
Keyword Dictionary_scan(Dictionary dict);
Keyword Dictionary_next(Keyword element);
Keyword Dictionary_keywordSub(Dictionary dict,int index);
void Global_objectAddress(pointer address);
void Global_objectsZero(void);
Keyword Keyword_Value(char *word,Obj item);
Keyword Keyword_TakeItem(char *word,Obj item);
void Keyword_set_item(Keyword self,Obj object);
Obj Keyword_get_item(Keyword self);
#define Keyword_item Keyword_get_item
Obj Memory_alocO(int size);
extern _Atomic huge Object_Counter;
Obj Memory_aloc(int size,CodaCLASS *clas,int extra);
extern bool Memory_SecurityOn;
void Memory_free(Obj obj);
Obj Memory_keep(Obj obj);
void Memory_clean(const void *vp);
CodaCLASS* Memory_class(Obj obj);
DTOR Memory_etor(Obj obj);
DTOR Memory_ekeep(Obj obj);
char* Memory_kind(Obj obj);
int4 Memory_size(Obj obj);
int4 Memory_count(Obj obj);
CodaCLASS* Memory_reclass(Obj obj,CodaCLASS *clas);
Obj Memory_newO(Obj obj,int nel);
Obj Memory_objectToClass(Obj obj,CodaCLASS* newClass);
Obj Memory_superObject(Obj obj);
extern void (*Msg_hook)(Char msg,int log);
void Msg_Object(Char $CONSUMED msg,int log);
extern void (*Die_hook)(Char msg,int trace);
Obj $NORETURN Die_Object(Char $CONSUMED msg,int trace);
Pointer Pointer_Value(pointer value);
Pointer Pointer_NewBlock(Pointer self,int count,pointer block);
int Pointer_length(pointer address);
extern Char (*Root_Diag)(Obj self);
Char Root_get_name(Root self);
#define Root_name Root_get_name
int Root_get_count(Root self);
#define Root_count Root_get_count
CDictionary Signature_dictionary(void);
void Signature_register(Char sig);
Char Signature_fromString(char *str);
void codac_sig(pointer cmux,char *key,pointer data);
Char ToContainer(Obj container);
CDictionary Version_dictionary(void);
void Version_register(char *key,char *text);
void codac_versions(void);

	void Array_dtor(Array self,CodaCLASS *clas);
	void Array_itor(Array self);
	void *CMux_KeyData(pointer cmux,char *key,pointer data) ;

void clean_C(const void *vp) { freeC(*(void **)vp); }

pointer alocC(int4 size) {
	pointer address=calloc(1,size);
	if (!address) Die_Object(Os("alocC; out of memory!"),1);
	return(address);
	}

void freeC(pointer address) {
	free(address);
	}

double Array_FACTOR=1.60;

#define class CArray

	struct Array_ { Char name; Pointer alla; int count,nel,zerox; } ;

	CodaClassZeros(etor,bits,kize,ekeep);
	#define CArray_dtor Array_dtor
	#define CArray_itor Array_itor
CodaClass(CArray,struct Array_,Root);

Char $(Info) {
	char *name=Array_name(self);
	if (!name) name="C Array of raw pointers";
	return Char_F("%s[%d] <%s>",kindO(self),Array_count(self),name);
	}

static int count2nel(int count) {
	if (count<32) count=32;
	int nel=(int)llround(count*Array_FACTOR); assert(nel>count+3);
	return(nel);
	}
	#define eleAddr(ptr,index) (ptr->alla + ptr->zerox + index)

static void C_Array_Release(Array self,DTOR etor,int start,int nel) {
	if (!etor) return;
	Pointer vec=(void *)eleAddr(self,start);
	for(int j=nel-1;j>=0;--j) etor(vec[j],self);
	}

static void CArray_copyBlock(CArray self,int dix,int count,Pointer block) {
	Pointer pdest=eleAddr(self,dix);
	for(int j=0;j<count;++j) *pdest++ = *block++;
	}

static void CArray_copyBlockFromEnd(CArray self,int dix,int count,Pointer block) {
	Pointer pdest=eleAddr(self,dix+count); block+=count;
	for(int j=0;j<count;++j) *(--pdest) = *(--block);
	}

	static void CArray_initCap(CArray self,int capacity) ;
static CArray CArray_NewCap(int capacity) {
	CArray self=newO(CArray); freeC(_ alla);
		CArray_initCap(self,capacity);
	return(self);
	}

	static void CArray_Upgrade(CArray dest,int dix,int count,Pointer block,char *msg) ;
static void CArray_Smaller(CArray dest) {
	if (dest->nel<=32) return;
	int tenth=dest->nel/10;
	if (dest->count>tenth) return;
	CArray_Upgrade(dest,0,0,NULL,"Downsize");
	}

void Array_removeBlock(Array self,int dix,int count) {
	if (count<1) return; assert(dix>=0 && dix+count<=_ count);
	C_Array_Release(self,Memory_etor(self),dix,count);
	if (dix==0)				_ zerox+=count;
	ei (dix+count==_ count)	;
	else CArray_copyBlock(self,dix,_ count-dix-count,eleAddr(self,dix+count));
	_ count-=count;
	CArray_Smaller(self);
	}

static void CArray_Upgrade(CArray dest,int dix,int count,Pointer block,char *msg) {
	int capa=(int)llround((dest->count+count)*Array_FACTOR);
	cleanO CArray grade=CArray_NewCap(capa); assert(grade->nel>dest->count+count);
		grade->count=dest->count+count;
		grade->zerox=(grade->nel - grade->count)/3; assert(grade->zerox>=1);
	CArray_copyBlock(grade,        0,dix,eleAddr(dest,0));
	CArray_copyBlock(grade,      dix,count,block);
	CArray_copyBlock(grade,dix+count,dest->count-dix,eleAddr(dest,dix));
	Char destName=dest->name;
	struct Array_ temp=(*dest); (*dest)=(*grade); (*grade)=temp;
	dest->name=destName; grade->name=0;
	}

void Array_takeBlock(Array self,int index,int count,pointer block) {
	if (count<1) return; assert(index>=0 && index<=_ count);
	Char msg=0; int tailcount=_ count - index;
	if (index==0) { msg="Expand Front";
		if (count<= _ zerox) {
			_ zerox-=count; _ count+=count;
			return CArray_copyBlock(self,index,count,block);
			}
		}
	ei (index==_ count) { msg="Expand End";
		if (_ nel - _ zerox - _ count >= count) {
			_ count+=count;
			return CArray_copyBlock(self,index,count,block);
			}
		}
	ei (index<tailcount) { msg="Insert Front";
		if (count<=_ zerox)	{
			_ zerox-=count; _ count+=count;
			       CArray_copyBlock(self,0,index,eleAddr(self,count));
			return CArray_copyBlock(self,index,count,block);
			}
		}
	else { msg="Insert End";
		if (_ nel - _ zerox - _ count >= count) {
			_ count+=count;
			CArray_copyBlockFromEnd(self,index+count,tailcount,eleAddr(self,index));
			return CArray_copyBlock(self,index,count,block);
			}
	  	}
	CArray_Upgrade(self,index,count,block,msg);
	}

Obj CArray_insertAt(CArray self,int index,Obj obj) {
	Array_takeBlock(self,index,1,&obj);
	return(obj);
	}

Char $(ToString) {
	return Char_F("%s[%d]",kindO(self),_ count);
	}

$boot(StephenMJones) {
	CodaSig(Info);
	CodaSig(ToString);
	}

#undef class

#define class Array

void $(dtor,CodaCLASS *clas) {
	freeO(_ name); _ name=0;
	C_Array_Release(self,clas->etor,0,_ count);
	freeC(_ alla);
	}

static void CArray_initCap(CArray self,int capacity) {
	_ count=0;
	_ nel=count2nel(_ count);
	if (capacity && _ nel<capacity) _ nel=capacity;
	_ zerox=(_ nel - _ count)/3; assert(_ zerox>=1);
	_ alla=alocC(_ nel*sizeof(pointer));
	}

void $(itor) { CArray_initCap(self,0); }

int $(get_count) {
	if (!self) return(0);
	return(_ count);
	}

Obj $(subInt,int ix) {
	if (ix<0 || ix>=_ count) return(0);
	return(*eleAddr(self,ix));
	}

pointer $(rawAddress) {
	return( (pointer)eleAddr(self,0) );
	}

#undef class
typedef Obj  OSig(addObject)(Obj array,Obj obj) ;               sig_(addObject);
typedef Obj  OSig(subInt)(Obj array,int ix) ;                   sig_(subInt);
typedef int  OSig(removeObject)(Obj array,Obj obj) ;            sig_(removeObject);
typedef void OSig(removeLast)(Obj array) ;                      sig_(removeLast);
typedef void OSig(removeAll)(Obj array) ;                       sig_(removeAll);
typedef Char OSig(ToDelimiter)(Obj container,int index) ;       sig_(ToDelimiter);
typedef Char OSig(ToStringSub)(Obj container,int index) ;       sig_(ToStringSub);
typedef Obj  OSig(NewBlock)(Obj proto,int count,pointer data) ; sig_(NewBlock);

#define class Array

	CodaClassZeros(bits,kize);
	#define Array_etor  freeO
	#define Array_ekeep keepO
CodaClass(Array,struct Array_,Root);

Char $(Info) {
	char *name=_$(get_name);
	if (!name) name="strong+CArray";
	return Char_F("%s[%d] <%s>",kindO(self),_$(get_count),name);
	}

Array Array_NewBlock(Array proto,int count,pointer block) {
	Array self=Memory_newO(proto?proto:Class_Array,1);
	_$(insertBlock,0,count,block);
	return(self);
	}

void $(insertBlock,int index,int count,pointer block) {
	Pointer source=block; DTOR keeper=Memory_ekeep(self);
	if (keeper) for(int j=0;j<count;++j) keeper(source[j],self);
	Array_takeBlock(self,index,count,block);
	}

Obj $(insertAt,int index,Obj obj) {
	DTOR keeper=Memory_ekeep(self); if (keeper) keeper(obj,self);
	return CArray_insertAt(self,index,obj);
	}

void $(removeAt,int dix) {
	Array_removeBlock(self,dix,1);
	}

Obj $(addObject,Obj obj) {
	return _$2(insertAt,_$(get_count),obj);
	}

void $(takeObject,Obj $CONSUMED obj) {
	CArray_insertAt(self,_$(get_count),obj);
	}

void $(removeLast) {
	_$2(removeAt,_$(get_count)-1);
	}

Obj $(replaceAt,int ix,Obj obj) {
	Pointer ptr=Array_rawAddress(self); assert(ix>=0 && ix<Array_count(self));
		DTOR keeper=Memory_ekeep(self); if (keeper) keeper(obj,self);
		DTOR etor=Memory_etor(self); if (etor) etor(ptr[ix],self);
		ptr[ix]=obj;
	return(obj);
	}

Char $(xmlTag) { return Os("array"); }

int $(removeObject,Obj object) {
	int removed=0;
	for(int j=0;j<Array_count(self);++j) {
		if (Array_sub(self,j)==object) {
			Array_removeAt(self,j);
			--j; ++removed;
			}
		}
	return(removed);
	}

void $(removeAll) {
	Array_removeBlock(self,0,Array_count(self));
	}

Char $(ToDelimiter,int index) {
	if (index==1 && Array_name(self)) return Char_F("<%s>(",Array_name(self));
	static Char da[]={ Os(", "), Os("("), Os(")"), Os("?") }; return da[index&3];
	}

void $(set_name,char* name) {
	Char old= _ name;
	_ name=(name && *name ? Char_Value(name) : 0);
	freeO(old);
	}

Char $(get_name) { return(_ name); }

Char $(ToStringSub,int index) {
	return ToContainer( Array_sub(self,index) );
	}

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(get_count); CodaSig(subInt);
	CodaSig(NewBlock);
		CodaSig(addObject); CodaSig(removeLast);
		CodaSig(removeObject); CodaSig(removeAll);
	CodaSig(xmlTag);
	CodaSig(ToDelimiter);
	CodaSig(ToStringSub);
	CodaSig(set_name); CodaSig(get_name);
	}

ConstChar Os_Set=Os("<set>");

void $(toaSet) { _$(set_name,0); _ name = Os_Set; }

bool $(isaSet) { return(_ name == Os_Set); }

#undef class

Char Array_Join(Array array,char *delimiter) {
	if (!array) return(0);
	if (!delimiter) delimiter="";
	int j,total=0,nel=Array_count(array),dsize=cs_length(delimiter),nout=0;
	for(j=0;j<nel;++j) {
		Obj obj=Array_sub(array,j);
  		if (isa_(obj,Char)) { ++nout; total+=cs_length(obj); }
		}
	if (nout<1)	return Char_Value("");
	Char blob=newOC(Char,total+1+dsize*nout);
	char *at=blob; nout=0; int check=0;
	for(j=0;j<nel;++j) {
		Obj obj=Array_sub(array,j);
  		if (isa_(obj,Char)) {
			if (nout) { cs_strcopy(at,delimiter); at+=dsize; check+=dsize; }
			int size=cs_length(obj); cs_strcopy(at,obj); at+=size; check+=size;
			++nout;
			}
		}
	++check;
	assert(check == total+1+dsize*(nout-1) );
	return(blob);
	}
typedef Char OSig(ToString)(Obj obj);               sig_(ToString);
typedef Obj  OSig(FromString)(Obj proto,char *str); sig_(FromString);

#define class Char
CodaClassZerosC();
CodaClass(Char,char,Root);

class Char_Value(const char *string) {
	if (!string) return(0);
	char *cp=newOC(Char,cs_length(string)+1); cs_strcopy(cp,string); return(cp);
	}

class $FORMAT12 Char_F(char *cs,...) {
	char buf[4096]; va_list ap; va_start(ap,cs); vsnprintf(buf,sizeof(buf),cs,ap); va_end(ap);
	return Char_Value(buf);
	}

Char $(Info) {
	char b[48]; snprintf(b,40,"%s",self); b[40]=0;
	return Char_F("%s[%d]=\"%s\" aka char*",kindO(self),obj_(get_count,self),b);
	}

Char $(FromString,char *str) { return Char_Value(str); }

Char $(ToString) { return(keepO(self)); }

Char $(xmlTag) { return Os("string"); }

class $(NewBlock,int count,pointer address) {
	if (count<=0) return(0);
	class cp=newOC(class,count); memcpy(cp,address,count); return(cp);
	}

$boot(StephenMJones) {
	CodaSig(Info);
	CodaSig(FromString); CodaSig(ToString); CodaSig(xmlTag);
	CodaSig(NewBlock);
	}
#undef class

#define class ConstChar
CodaClassTransC(); CodaClass(ConstChar,char,Char);
#undef class

CDictionary Class_dictionary_dict=0;

CDictionary Class_dictionary() {
	if (!Class_dictionary_dict) {
		Class_dictionary_dict=newO(CDictionary);
		Dictionary_set_name(Class_dictionary_dict,"The*Class*Dictionary");
		Global_objectAddress(&Class_dictionary_dict);
		}
	return(Class_dictionary_dict);
	}

void Class_register(Obj obj) {
	Dict_set(Class_dictionary(),classO(obj)->kClass,obj);
	}

	typedef struct keydata_ { char *key; pointer data; } keydata;

	typedef struct CMux_ {
		keydata *array;
		int4 nel,mask,count,empty;
		int4 pow,rebuilding,maxHop,flags;
		int4 (*hasher)(const char *key);
		char resv[64-sizeof(pointer)-8*sizeof(int4)-sizeof(pointer)];
		} *CMux;

	#define CMUXMINPOW  4
	#define CMUXMAXPOW 28

 void CMux_itor2(CMux mux,int pow) {
	mux->pow=pow;
	mux->nel=(1<<pow);
	mux->mask=(mux->nel -1);
	mux->array=alocC(sizeof(keydata)*mux->nel);
	mux->empty=0;
	mux->maxHop=0;
	}

static void CMux_itor(CMux mux) {
	CMux_itor2(mux,CMUXMINPOW);
	}

void CMux_dtor(pointer cmux) {
	CMux mux=cmux;
	freeC(mux->array); mux->array=0;
	}

void *CMux_Data4Key(pointer cmux,char *key) {
	CMux mux=cmux; if (!mux->array) return(0);
	int j,h=(intptr_t)key;
	for(j=0;j<mux->nel;++j) {
		int ix= (h+j)&mux->mask;
		keydata *ele=(&mux->array[ix]);
		char *cp=ele->key; if (!cp) break;
		if (cp==key) return ele->data;
		}
	return(0);
	}

static void CMux_Rebuild(CMux mux,int count) {
	Assert_(!mux->rebuilding);
	int pow=0;
	for(pow=CMUXMINPOW;pow<CMUXMAXPOW;++pow)  {
		int nel=(1<<pow);
		if (nel>=count) break;
		}
	CMux grade=alocO(sizeof(struct CMux_)); CMux_itor2(grade,pow);
		grade->rebuilding=1;
		for(int j=0;j<mux->nel;++j) {
			keydata *ele=(&mux->array[j]);
			if (ele->data) CMux_KeyData(grade,ele->key,ele->data);
			}
		grade->rebuilding=0;

		struct CMux_ temp;
		temp=(*mux); (*mux)=(*grade); (*grade)=temp;
	CMux_dtor(grade); freeO(grade);
	}

static void CMux_Check(CMux mux) {
	int eigth=mux->nel/8;
	if (mux->nel - mux->count - mux->empty < eigth) {
		CMux_Rebuild(mux,mux->count*2);
		}
	}

void *CMux_KeyData(pointer cmux,char *key,pointer data) {
	CMux mux=cmux; if (!mux->array) CMux_itor(mux);

	int j,h=(intptr_t)key;
	for(j=0;j<mux->nel;++j) {
		int ix= (h+j)&mux->mask;
		keydata *ele=(&mux->array[ix]);
		if (!ele->key || ele->key==key) {
			if (!data && !ele->data) return(0);
			ei (!ele->key) ++mux->count;
			ei (!data &&  ele->data) { --mux->count; ++mux->empty; }
			ei ( data && !ele->data) { ++mux->count; --mux->empty; }
			ele->key=key;
			ele->data=data;
			if (j>mux->maxHop) mux->maxHop=j;
			CMux_Check(mux);
			return(0);
			}
		}
	Die_("CMux; hash full, dynamic expansion failed.");
	return(data);
	}

	typedef struct csKeyword_ {
		struct csKeyword_ *nxt;
		pointer data;
		char key[0];
		} csKeyword;

	struct Dictionary_ {
		csKeyword *list;
		Char name;
		struct CMux_ mux;
		};

$boot() {
	Version_register( "Coda-C_PList",
	Os(QVersion ", " QWebsite ", Copyright (c) " QCopyYears "  Stephen M. Jones, Affero GPL 3."
	"\000"));
	static_assert(sizeof(huge)==8,"wrong size huge?");
	}

	typedef struct CodaStructMeta_ {
		CodaCLASS *clas;
		unsigned int size:24;
		unsigned int fsegs:8;
		_Atomic unsigned short count;
		unsigned short metasize;
		} CodaStructMeta;

	#define meta2obj(meta)	( (void *)(&meta[1]))
	#define obj2meta(obj)	( &((CodaStructMeta *)obj)[-1] )
	#define obj2meta0(obj)  ( (CodaStructMeta *)( ((void *)obj) - obj2meta(obj)->metasize ) )

	enum { CodaMaxMeta=99, };

bool OisaClass(Obj obj,Obj classobj) {
	CodaCLASS *hunt=classO(classobj);
	CodaCLASS *clas=classO(obj);
	for(int j=0;j<=CodaMaxMeta;++j) {
		if (clas==hunt) return(1);
		clas=clas->superClass; if (!clas) break;
		}
	return(0);
	}

void *ORespond2(void *obj,char *sel,CodaCLASS **cpp,int diag) {
	CodaCLASS *clas=0;
	if (obj) {
		CodaStructMeta *meta=obj2meta(obj);
		if (meta->metasize==0) clas=(&Isa_ConstChar);
		  else {
			meta= obj2meta0(obj);
			clas=meta->clas;
			}
		}
	if (!clas) clas=(&Isa_Void);
	void *data=0; char *diagstr=clas->kClass;
	for(int j=0;1;++j) {
		pointer cmux=clas->classObject; Assert_(cmux);
		data=CMux_Data4Key(cmux,sel); if (data) break;
		clas=clas->superClass;
		if (!clas) { if (diag) Die_("Coda-C Class:%s does not respond to: %s",diagstr,sel); break; }
		if (j>CodaMaxMeta) Die_("Circular Isa-Classes detected for Class: %s",diagstr);
		}
	if (cpp) *cpp=clas;
	return(data);
	}

Obj OObject(Obj obj,char *sel) {
	return(obj);
	}

typedef void OSig(setKey)(Obj dict,char *key,Obj obj) ; sig_(setKey);
typedef Obj  OSig(subKey)(Obj dict,char *key) ;         sig_(subKey);
typedef bool OSig(removeKey)(Obj dict,char *key) ;      sig_(removeKey);
typedef Pointer OSig(AllKeys)(Obj dict) ;               sig_(AllKeys);
typedef Char OSig(xmlTag)(Obj obj);                     sig_(xmlTag);

#define class Dictionary

void $(dtor,CodaCLASS *clas) {
	if (_ mux.hasher) CMux_dtor(& _ mux);
	csKeyword *ptr,*nxt=0; DTOR etor=clas->etor;
	for(ptr=_ list;ptr;ptr=nxt) {
		nxt=ptr->nxt;
		if (etor) etor(ptr->data,self);
		freeC(ptr);
		}
	freeO(_ name); _ name=0; _ list=0; _ mux.count=0;
	}

	CodaClassZeros(itor,bits,kize);
	#define Dictionary_etor  freeO
	#define Dictionary_ekeep keepO
CodaClass(Dictionary,struct Dictionary_,Root);

int $(get_count) {
	if (!self) return(0);
	return(_ mux.count);
	}

void $(setKey,char *key,Obj obj) {
	if (_ mux.hasher) { DictHash_keyobj(self,key,obj); return; }
	if (!obj) { _$(removeKey,key); return; }
	DTOR keeper=Memory_ekeep(self); if (keeper) keeper(obj,self);
	csKeyword *ptr;
	for(ptr=_ list;ptr;ptr=ptr->nxt) { if (cs_exact(key,ptr->key)) break; }
	if (ptr) {
		DTOR etor=Memory_etor(self);
		if (etor) etor(ptr->data,self);
		}
	  else {
		++_ mux.count;
		ptr=alocC(sizeof(csKeyword)+cs_length(key)+1);
			cs_strcopy(ptr->key,key);
		ptr->nxt=_ list; _ list=ptr;
		}
	ptr->data=obj;
	_$(auto);
	}

bool $(removeKey,char *key) {
	if (_ mux.hasher) return DictHash_keydel(self,key);
	csKeyword *ptr,*pre;
	for(pre=0,ptr=_ list;ptr;pre=ptr,ptr=ptr->nxt) { if (cs_exact(key,ptr->key)) break; }
	if (ptr) {
		DTOR etor=Memory_etor(self); if (etor) etor(ptr->data,self);
		if (!pre) _ list=ptr->nxt;
		  else	  pre->nxt=ptr->nxt;
		freeC(ptr); --_ mux.count;
		return(1);
		}
	return(0);
	}

int $(removeObject,Obj obj) {
	int removed=0;
	DTOR etor=Memory_etor(self);
	for(csKeyword *pre=0,*nxt=0,*ptr=_ list;ptr;pre=ptr,ptr=nxt) {
		nxt=ptr->nxt;
		if (ptr->data==obj) {
			if (_ mux.hasher) {
				bool ret=0;
				ret=DictHash_keydel(self,ptr->key);
				assert(ret);
				}
			  else {
				if (etor) etor(ptr->data,self);
				if (!pre) _ list=ptr->nxt;
				  else	  pre->nxt=ptr->nxt;
				freeC(ptr); --_ mux.count;
				}
			++removed;
			ptr=pre;
			}
		}
	return(removed);
	}

Obj $(subKey,char *key) {
	if (_ mux.hasher) return DictHash_obj4key(self,key);
	for(csKeyword *ptr=_ list;ptr;ptr=ptr->nxt) if (cs_exact(key,ptr->key)) return(ptr->data);
	return(0);
	}

Pointer $(AllKeys) {
	int nel=Dictionary_count(self); if (nel<=0) return(0);
	Pointer vec=newOC(Pointer,nel);
	csKeyword *ptr=_ list;
	for(int j=0;ptr;ptr=ptr->nxt,++j)
		vec[(nel-1-j)]=ptr->key;
	return(vec);
	}

Char $(Info) {
	int nel=Dictionary_count(self);
	char *name=Dictionary_name(self);
	if (!name) name="Keyword array";
	return Char_F("%s[%d] <%s>",kindO(self),nel,name);
	}

Char $(xmlTag) { return Os("dict"); }

Char $(ToDelimiter,int index) {
	if (index==1 && Dictionary_name(self)) return Char_F("<%s>{",Dictionary_name(self));
	static Char da[]={ Os(", "), Os("{"), Os("}"), Os("?") }; return da[index&3];
	}

Char $(ToStringSub,int index) {
	Keyword key=Dictionary_keywordSub(self,index);
	cleanO Array temp=newO(Array);
	Array_takeObject(temp,Char_Value(key->word));
	Array_takeObject(temp,ToContainer(key->item));
	return Array_Join(temp," : ");
	}

void $(set_name,char* name) {
	freeO(_ name); _ name=(name && *name ? Char_Value(name) : 0);
	}

Char $(get_name) { return _ name; }

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(get_count); CodaSig(xmlTag); CodaSig(AllKeys);
	CodaSig(set_name); CodaSig(get_name);
	CodaSig(ToDelimiter); CodaSig(ToStringSub);
	CodaSig(setKey); CodaSig(subKey);
	CodaSig(removeKey); CodaSig(removeObject);
	}

void $(takeKey,char *key,Obj $CONSUMED obj) {
	Dict_set(self,key,obj);
	freeO(obj);
	}

#undef class

#define class CDictionary
	CodaClassZeros(etor,itor,bits,kize,ekeep);
	#define CDictionary_dtor    Dictionary_dtor
CodaClass(CDictionary,struct Dictionary_,Root);

Char $(Info) {
	int nel=Dictionary_get_count(self);
	char *name=Dictionary_get_name(self);
	if (!name) name="C Keyword array of raw pointers";
	return Char_F("%s[%d] <%s>",kindO(self),nel,name);
	}

Char $(ToString) { return Char_F("%s[%d]",kindO(self),Dictionary_get_count(self)); }

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(ToString);
	}
#undef class
	void Dictionary_auto(Dictionary dict) { }
	bool DictHash_keyobj(Dictionary dict,char *key,pointer obj) { Quit_("%s; is not defined.",__func__); }
	bool DictHash_keydel(Dictionary dict,char *key) { Quit_("%s; is not defined.",__func__); }
	Obj DictHash_obj4key(Dictionary dict,char *key) {  Quit_("%s; is not defined.",__func__); }

Keyword Dictionary_scan(Dictionary dict) {
	if (!dict) return(0);
	csKeyword *head=dict->list;
	if (!head) return(0);
	return((Keyword)&head->data);
	}

Keyword Dictionary_next(Keyword element) {
	Pointer dataptr=(Pointer)element;
	csKeyword *ptr=(csKeyword *)(&dataptr[-1]);
	if (!ptr->nxt) return(0);
	return((Keyword)&ptr->nxt->data);
	}

Keyword Dictionary_keywordSub(Dictionary dict,int index) {
	csKeyword *ptr; int j=0;
	for(ptr=dict->list;ptr;ptr=ptr->nxt,++j) if (index==j) break;
	if (!ptr) return(0);
	return((Keyword)&ptr->data);
	}
CArray Global_objectPointers=0;

void Global_objectAddress(pointer address) {
	if (!Global_objectPointers) Global_objectPointers=newO(CArray);
	Array_addObject(Global_objectPointers,address);
	}

void Global_objectsZero() {
	if (!Global_objectPointers) return;
	int nel=Array_count(Global_objectPointers);
	Pointer vector=Array_rawAddress(Global_objectPointers);
	for(int j=nel-1;j>=0;--j) {
		Pointer address=vector[j];
		freeO(*address); *address=0;
		}
	freeO(Global_objectPointers); Global_objectPointers=0;
	}
#define class Keyword

static void $(dtor) { freeO(_ item); }

	CodaClassZeros(etor,itor,bits,ekeep);
	#define Keyword_kize    sizeof(struct Keyword_)
CodaClass(Keyword,struct Keyword_,Root);

class Keyword_Value(char *word,Obj item) {
	if (!word) return(0);
	class self=newOC(class,cs_length(word)+1);
		cs_strcopy(_ word,word);
		_ item=keepO(item);
	return(self);
	}

class Keyword_TakeItem(char *word,Obj item) {
	if (!word) return(0);
	class self=newOC(class,cs_length(word)+1);
		cs_strcopy(_ word,word);
		_ item=item;
	return(self);
	}

Char $(Info) {
	cleanO Char msg=obj_(Info,_ item);
	return Char_F("Keyword[%d+%d] {%s : %s}",classO(self)->kize,Root_count(self),_ word,msg);
	}

Char $(xmlTag) { return Os("keyword"); }

Char $(ToString) {
	cleanO Char msg=ToContainer(_ item);
	return Char_F("<keyword>{%s : %s}",_ word,msg);
	}

void $(set_item,Obj object) {
	keepO(object);
	freeO(_ item); _ item=object;
	}

Obj $(get_item) { return(_ item); }

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(xmlTag); CodaSig(ToString);
	}

#undef class

	#define alocOCE(sz,cl,ex)	Memory_aloc(sz,cl,ex)

	static_assert(CodaMetaSize==sizeof(CodaStructMeta),"MetaSize Error.");

Obj Memory_alocO(int size) { return Memory_aloc(size,0,0); }

_Atomic huge Object_Counter=0;

Obj Memory_aloc(int size,CodaCLASS *clas,int extra) {
	if (size<=0) Die_("size %d ? class: %s",size,(clas ? clas->kClass : "?ZERO?"));
	CodaStructMeta *first=alocC(extra+CodaMetaSize+size);
	CodaStructMeta *meta=first; if (extra) meta= (CodaStructMeta *) ( ((char *)first) + extra);
	meta->metasize=extra+CodaMetaSize;
	meta->size=size;
	meta->count=1;
	meta->fsegs=0;
	meta->clas=clas;
	++Object_Counter;
	return(meta2obj(meta));
	}

	CodaStructMeta *Memory_metanext( CodaStructMeta *meta) ;
	int Memory_totalMeta0Size(CodaStructMeta *meta) ;

bool Memory_SecurityOn=1;

void Memory_free(Obj obj) {
	if (!obj) return;
	CodaStructMeta *meta=obj2meta(obj); if (meta->metasize==0) return;
	meta=obj2meta0(obj);
	if (!meta->count) return;
	int count= --meta->count;
	if (count==0) {
		CodaStructMeta *ptr=meta;
		for(int j=0;ptr && j<CodaMaxMeta;ptr=Memory_metanext(ptr),++j) {
			if (ptr->clas && ptr->clas->dtor) {
				ptr->clas->dtor(meta2obj(ptr),ptr->clas);
				}
			}
		Assert_(!ptr);
		if (Memory_SecurityOn) cs_blockZero(meta,Memory_totalMeta0Size(meta));
		freeC(meta);
		--Object_Counter;
		}
	}

Obj Memory_keep(Obj obj) {
	if (!obj) return(obj);
	if (!obj2meta(obj)->metasize) return(obj);
	CodaStructMeta *meta=obj2meta0(obj);
	if (meta->count) ++meta->count;
	return(obj);
	}

void Memory_clean(const void *vp) {
	Memory_free(*(Obj *)vp);
	}

CodaCLASS* Memory_class(Obj obj) {
	if (!obj) return(&Isa_Void);
	CodaStructMeta *meta=obj2meta(obj);
	if (meta->metasize==0) return(&Isa_ConstChar);
	if (!meta->clas) return(&Isa_Void);
	return(meta->clas);
	}

DTOR Memory_etor(Obj obj) { return classO(obj)->etor; }

DTOR Memory_ekeep(Obj obj) { return classO(obj)->ekeep; }

char* Memory_kind(Obj obj) { return classO(obj)->kClass; }

int4 Memory_size(Obj obj) {
	if (!obj) return(0);
	CodaStructMeta *meta=obj2meta(obj);
	if (meta->metasize==0) return cs_length(obj)+1;
	return(meta->size);
	}

int4 Memory_count(Obj obj) {
	if (!obj) return(-1);
	CodaStructMeta *meta=obj2meta(obj);
	if (meta->metasize==0) return(0);
	return obj2meta0(obj)->count;
	}

CodaCLASS* Memory_reclass(Obj obj,CodaCLASS *clas) {
	Assert_(obj);
	CodaStructMeta *meta=obj2meta(obj);
	Assert_(meta->metasize);
	void *old=meta->clas; meta->clas=clas; return(old);
	}

	static pointer Memory_pack(void *enclosed,int newsize,void *clas) {
		Die_("Non transparent subclasses are not allowed.");
		}

 static Obj Memory_newOOR(CodaCLASS *clas,int count,int extra,int transAbove) {
	if (!clas) Die_(">>>use alocO() instead of newO() to make VOID class objects!");
	Assert_(clas!=NULL && count>0 && extra>=0);
	Assert_(clas->superNel>=1); Assert_(clas->size>0);
	int size=count*clas->size,trans=0;
	if (clas->kize) size=clas->kize+count;
	int supNel=clas->superNel;
	if (clas->bits<0)	{
		size=0; trans=1; Assert_(count>=1 && supNel>=1);
		if (supNel!=1 && count!=1)
			Die_("You can't create multiple of Arrayed Classes(%d * %d)",supNel,count);
		supNel*=count;
		}
	Assert_(size+extra<0x7FFFFF);
	Obj obj=0;
	if (clas->superClass && clas->superClass->bits!=bits_Root) {
		if (count!=1 && !trans)	{
			Die_("You can only ARRAY Bases classes(super==Root) OR Transparent classes! { %s }",clas->kClass);
			}
		obj=Memory_newOOR(clas->superClass,supNel,extra+size+(size?CodaMetaSize:0),trans);
		if (size)	obj=Memory_pack(obj,size,clas);
		  else		Memory_reclass(obj,clas);
		}
	  else	{
		obj=alocOCE(size,clas,extra);
		}
	if (clas->itor && !transAbove) {
		clas->itor(obj,clas);
		}
	return(obj);
	}

Obj Memory_newOO(CodaCLASS *clas,int count) { return Memory_newOOR(clas,count,0,0); }

Obj Memory_newO(Obj obj,int nel) {
	if (nel<1) Die_("newOC() called with count=%d.",nel);
	return Memory_newOO(Memory_class(obj),nel);
	}

	CodaStructMeta *Memory_metanext( CodaStructMeta *meta) { return(0); }
	int Memory_totalMeta0Size(CodaStructMeta *meta) { return(meta->size+CodaMetaSize); }

void (*Die_hook)(Char msg,int trace)=0;

Obj $NORETURN Die_Object(Char $CONSUMED msg,int trace) {
	if (Die_hook) Die_hook(msg,trace);
	  else	fprintf(stderr,"%s\n",msg);
	freeO(msg);
	abort();
	exit(-2);
	}

#define class Pointer
CodaClassZerosC();
CodaClass(Pointer,pointer,Root);

class Pointer_Value(pointer value) {
	class self=newO(class); *self=value; return(self);
	}

class $(NewBlock,int count,pointer block) {
	if (count<=0) return(0);
	class source=block; self=newOC(class,count);
	for(int j=0;j<count;++j) self[j]=source[j];
	return(self);
	}

Char $(Info) { return Char_F("Pointer[%d] aka void**",Pointer_count(self)); }

Char $(ToDelimiter,int index) {
	static Char da[]={ Os(", "), Os("<pointer>["), Os("]"), Os("?") }; return da[index&3];
	}

Char $(ToStringSub,int index) { return Char_F("%p",self[index]); }

Char $(ToString) { return Char_F("%p",*self); }

$boot(StephenMJones) {
	CodaSig(NewBlock); CodaSig(Info);
	CodaSig(ToDelimiter); CodaSig(ToStringSub); CodaSig(ToString);
	}

#undef class

int Pointer_length(pointer address) {
	Pointer self=address; if (!self) return(0);
	int nel=0; for(;self[nel];++nel) ;
	return(nel);
	}

typedef int   OSig(get_count)(Obj obj) ;           sig_(get_count);
typedef Char  OSig(Info)(Obj obj) ;                sig_(Info);
typedef void  OSig(set_name)(Obj obj,char* name) ; sig_(set_name);
typedef Char  OSig(get_name)(Obj obj) ;            sig_(get_name);

#define class Root

CodaClassZeros(dtor,etor,itor,kize,ekeep); CodaClass_Root();
Coda_Class(Root,void,0);

Char (*Root_Diag)(Obj self)=0;

Char $(Info) {
	char *class=kindO(self);
	int nel=sizeO(self);
	cleanO Char msg=(Root_Diag?Root_Diag(self):Os(""));
	char* sep=(Root_Diag?"\n":"");
	int count=Root_count(self);
	int kize=classO(self)->kize;
	if (kize)	return Char_F("Class:%s[%d+%d][size=%d]%s%s",class,kize,count,nel,sep,msg);
	  			return Char_F("Class:%s[%d][size=%d]%s%s",   class,     count,nel,sep,msg);
	}

Char $(get_name) { return(0); }

int $(get_count) {
	CodaCLASS* clas=classO(self);
	if (clas->kize) return(sizeO(self) - clas->kize);
	return(sizeO(self)/clas->size);
	}

$boot(StephenMJones) {
	CodaSig(Info);
	CodaSig(get_count);
	CodaSig(get_name);
	}

#undef class

CDictionary Signature_dictionary_dict=0;

CDictionary Signature_dictionary() {
	if (!Signature_dictionary_dict) {
		Signature_dictionary_dict=newO(CDictionary);
		Dictionary_set_name(Signature_dictionary_dict,"The*Signature*Dictionary");
		Global_objectAddress(&Signature_dictionary_dict);
		}
	return(Signature_dictionary_dict);
	}

void Signature_register(Char sig) {
	Dict_set(Signature_dictionary(),sig,sig);
	}

Char Signature_fromString(char *str) {
	return Dict_sub(Signature_dictionary(),str);
	}

void codac_sig(pointer cmux,char *key,pointer data) {
	CMux_KeyData(cmux,key,data);
	Signature_register(key);
	}
Char ToContainer(Obj container) {
	if (!container) return Os("(Null)");
	if (classO(container)==(&Isa_Void)) return Os("Obj?");
	void* hasele=OResponds(container,ToStringSub);
	void* hastos=OResponds(container,ToString);
	void* hasdel=OResponds(container,ToDelimiter);
	int j,nel=obj_(get_count,container);
	if (hastos && nel==1) ;
	ei (hasele && hasdel) {
		cleanO Array temp=newO(Array);
		Array_take(temp, obj_(ToDelimiter,container,1) );
		for(j=0;j<nel;++j) {
			if (j) Array_take(temp, obj_(ToDelimiter,container,0) );
			Obj str=obj_(ToStringSub,container,j);
			Array_take(temp,str);
			}
		Array_take(temp, obj_(ToDelimiter,container,2) );
		return Array_Join(temp,0);
		}
	if (hasele) return obj_(ToStringSub,container,0);
	if (hastos) return obj_(ToString,container);
	return Os("?");
	}
CDictionary Version_dictionary_dict=0;

CDictionary Version_dictionary() {
	if (!Version_dictionary_dict) {
		Version_dictionary_dict=newO(CDictionary);
		Dictionary_set_name(Version_dictionary_dict,"The*Version*Dictionary");
		Global_objectAddress(&Version_dictionary_dict);
		}
	return(Version_dictionary_dict);
	}

void Version_register(char *key,char *text) {
	Dict_set(Version_dictionary(),key,text);
	}

void codac_versions() {
	CDictionary dict=Version_dictionary();
	for(Keyword key=Dictionary_scan(dict);key;key=Dictionary_next(key)) {
		fprintf(stderr,"%s: %s\n",key->word,(char *)key->item);
		}
	}

#define class Void

CodaClassZeros(dtor,etor,itor,kize,ekeep); CodaClass_Root();
Coda_Class(Void,void,0);

Char $(Info) {
	if (!self) return Os("(0)");
	return Char_F("Void_[%d]=%p void*",Root_count(self),self);
	}

int $(get_count) { return sizeO(self); }

$boot(StephenMJones) {
	CodaSig(Info);
	CodaSig(get_count);
	}

#undef class

CodaClassDef(Int4,int4,Root);
CodaClassDef(Short,short,Root);
	#define Int4_count Root_get_count
	#define Short_count Root_get_count
	typedef pointer (*UcEncoder)(pointer ptr,int4 value,pointer context);
Int4 Int4_Value(int4 value);
Int4 Int4_NewBlock(Int4 self,int count,pointer address);
int Int4_length(Int4 self);
Short Short_Value(short value);
Short Short_NewBlock(Short self,int count,pointer address);
int Short_length(Short self);
int u2_codes(pointer u2str);
int u2_count(pointer u2str,int codes);
int u2_scan(pointer u2str,int countOrNeg1,int codes,int *counter,int *faults);
pointer u2_encode(pointer ptr,int4 value,pointer context);
pointer u4_encode(pointer ptr,int4 value,pointer context);
int uc_codes(char* ustr);
int uc_count(char *ustr,int codes);
int uc_scan(char *ustr,int lenOrNeg1,int codes,int *counter,int *faults);
pointer uc_encode(pointer ptr,int4 value,pointer context);
int uc_faults(char *ustr);
Obj uN_Convert(int isizeN,UcEncoder encoder,pointer context,pointer uNstr,int countOrNeg1,int *faults);
Short uc_ToU2(char *u1str,int countOrNeg1,int *faults);
Int4 uc_ToU4(char *u1str,int countOrNeg1,int *faults);
Char u2_ToUc(pointer u2str,int countOrNeg1,int *faults);
Int4 u2_ToU4(pointer u2fstr,int countOrNeg1,int *faults);
Char u4_ToUc(pointer u4str,int countOrNeg1,int *faults);
Short u4_ToU2(pointer u4str,int countOrNeg1,int *faults);

int uc_codes(char* ustr) {
	return uc_scan(ustr,-1,-1,NULL,NULL);
	}

int uc_count(char *ustr,int codes) {
	int bytes=0;
	uc_scan(ustr,-1,codes,&bytes,NULL);
	return(bytes);
	}

int uc_scan(char *ustr,int lenOrNeg1,int codes,int *counter,int *faults) {
	unsigned char *str=(unsigned char *)ustr;
	int haslimit=(codes>=0);
	codes&=0x7FFFFFFF;
	int len,extra=0,utfch=0,fault=0;
	for(len=0;1;++len) {
		if (lenOrNeg1>=0 && len>=lenOrNeg1) break;
		int cc=str[len]; if (lenOrNeg1<0 && !cc) break;
		int oldutflen=utfch;
		if (0x80==(cc&0xC0)) {
			if (extra==1) { extra=0; ++utfch; }
			ei (extra>0)  --extra;
			else       	  ++fault;
			}
		else {
			if (extra)  { extra=0; ++fault;
				if (utfch+fault>codes) { --fault; break; }
				}
			if (0x00==(cc&0x80)) { ++utfch; }
			ei (0xC0==(cc&0xE0)) { extra=1; }
			ei (0xE0==(cc&0xF0)) { extra=2; }
			ei (0xF0==(cc&0xF8)) { extra=3; }
			else 				 { ++fault; }
			}
		if (haslimit) {
			int parto=(extra ? 1:0);
			if (utfch+fault+parto>codes) {
				if (oldutflen!=utfch) --utfch; else --fault;
				break;
				}
			}
		}
	if (extra) ++fault;
	if (counter) *counter=len;
	if (faults) *faults=(fault);
	return(utfch+fault);
	}

pointer uc_encode(pointer ptr,int4 value,pointer context) {
	if (!ptr) {
		if (!value) return Class_Char;
		return(context);
		}
	unsigned char* cp=ptr;
	value&=0x1FFFFF;
	if (value<0x80)    { *cp++ = value; }
	ei (value<0x800)   { *cp++ = 0xC0|((value>> 6)&0x1F);
		*cp++ = 0x80|(value&0x3F);
		}
	ei (value<0x10000)  { *cp++ = 0xE0|((value>>12)&0x0F);
		*cp++ = 0x80|((value>>6)&0x3F); *cp++ = 0x80|(value&0x3F);
		}
	else 			   { *cp++ = 0xF0|((value>>18)&0x07);
		*cp++ = 0x80|((value>>12)&0x3F); *cp++ = 0x80|((value>>6)&0x3F); *cp++ = 0x80|(value&0x3F);
		}
	return(cp);
	}

int uc_faults(char *ustr) {
	int faults=0;
	uc_scan(ustr,-1,-1,NULL,&faults);
	return(faults);
	}

#define class Int4
CodaClassZerosC();
CodaClass(Int4,int4,Root);

class Int4_Value(int4 value) {
	class self=newO(class); *self=value; return(self);
	}

class $(NewBlock,int count,pointer address) {
	if (count<=0) return(0);
	self=newOC(class,count); cs_blockCopy(self,address,count*sizeat(class)); return(self);
	}

Char $(Info) { return Char_F("%s[%d]=(%d,...), aka int4*",kindO(self),Int4_count(self),*self); }

Char $(ToDelimiter,int index) {
	static Char da[]={ Os(", "), Os("<int4>["), Os("]"), Os("?") }; return da[index&3];
	}

Char $(ToStringSub,int index) { return Char_F("%d",self[index]); }

Char $(ToString) { return Char_F("%d",*self); }

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(NewBlock);
	CodaSig(ToString); CodaSig(ToDelimiter); CodaSig(ToStringSub);
	}

int $(length) {
	if (!self) return(0);
	int len=0; for(;self[len];++len) ;
	return(len);
	}
#undef class

#define class Short
CodaClassZerosC(); CodaClass(Short,short,Root);

class Short_Value(short value) {
	class self=newO(class); *self=value; return(self);
	}

class $(NewBlock,int count,pointer address) {
	if (count<=0) return(0);
	self=newOC(class,count); cs_blockCopy(self,address,count*sizeat(class)); return(self);
	}

Char $(Info) { return Char_F("%s[%d]=(%d,...) aka short*",kindO(self),Short_count(self),*self); }

Char $(ToDelimiter,int index) {
	static Char da[]={ Os(", "), Os("<short>["), Os("]"), Os("?") }; return da[index&3];
	}

Char $(ToStringSub,int index) { return Char_F("%d",self[index]); }

Char $(ToString) { return Char_F("%d",*self); }

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(NewBlock);
	CodaSig(ToString); CodaSig(ToDelimiter); CodaSig(ToStringSub);
	}

int $(length) {
	if (!self) return(0);
	int len=0; for(;self[len];++len) ;
	return(len);
	}
#undef class

int u2_codes(pointer u2str) {
	return u2_scan(u2str,-1,-1,NULL,NULL);
	}

int u2_count(pointer u2str,int codes) {
	int shorts=0;
	u2_scan(u2str,-1,codes,&shorts,NULL);
	return(shorts);
	}

int u2_scan(pointer u2str,int countOrNeg1,int codes,int *counter,int *faults) {
	unsigned short *str=u2str;
	int haslimit=(codes>=0);
	codes&=0x7FFFFFFF;
	int len,extra=0,utfch=0,fault=0;
	for(len=0;1;++len) {
		if (countOrNeg1>=0 && len>=countOrNeg1) break;
		int cc=str[len]; if (countOrNeg1<0 && !cc) break;
		int oldutflen=utfch;
		if (0xDC00==(cc&0xFC00)) {
			if (extra==1) { extra=0; ++utfch; }
			else       	  ++fault;
			}
		else {
			if (extra)  { extra=0; ++fault;
				if (utfch+fault>codes) { --fault; break; }
				}
			if (0xD800==(cc&0xFC00)) extra=1;
			else					 ++utfch;
			}
		if (haslimit) {
			if (utfch+fault+extra>codes) {
				if (oldutflen!=utfch) --utfch; else --fault;
				break;
				}
			}
		}
	if (extra) ++fault;
	if (counter) *counter=len;
	if (faults) *faults=(fault);
	return(utfch+fault);
	}

pointer u2_encode(pointer ptr,int4 value,pointer context) {
	if (!ptr) {
		if (!value) return Class_Short;
		return(context);
		}
	value&=0x1FFFFF;
	short* sp=ptr;
	if (value<0x10000) { *sp++ =value; }
	else {
		value-=0x10000;
		*sp++ = 0xD800|((value>>10)&0x3FF);
		*sp++ = 0xDC00|( value     &0x3FF);
		}
	return(sp);
	}

pointer u4_encode(pointer ptr,int4 value,pointer context) {
	if (!ptr) {
		if (!value) return Class_Int4;
		return(context);
		}
	value&=0x1FFFFF;
	int4* ip=ptr;
	*ip++ =value;
	return(ip);
	}

Obj uN_Convert(int isizeN,UcEncoder encoder,pointer context,pointer uNstr, int countOrNeg1,int *faults) {

	if (countOrNeg1<0) {
		if (isizeN==1) countOrNeg1=cs_length(uNstr);
		ei (isizeN==2) countOrNeg1=Short_length(uNstr);
		else		   countOrNeg1=Int4_length(uNstr);
		}
	int maxBytes=4*countOrNeg1+4;
	Char buffer=encoder(0,maxBytes,context);
	cleanO Char temp=(buffer?0:newOC(Char,maxBytes));
	if (!buffer) buffer=temp;
	char* cp=buffer; int fault=0,value=0;
	if (isizeN==1) {
		int extra=0;
		for(int len=0;len<countOrNeg1;++len) {
			int cc=((unsigned char*)uNstr)[len];
			if (0x80==(cc&0xC0)) {
				if (extra) { --extra;
					value=(value<<6)|(cc&0x3F);
					if (extra==0) { cp=encoder(cp,value,context); }
					}
				else { ++fault; cp=encoder(cp,cc,context); }
				}
			else {
				if (extra) { ++fault; cp=encoder(cp,value<<(6*extra),context); extra=0; }
				if (0x00==(cc&0x80)) { cp=encoder(cp,cc,context); }
				ei (0xC0==(cc&0xE0)) { extra=1; value=cc&0x1F; }
				ei (0xE0==(cc&0xF0)) { extra=2; value=cc&0x0F; }
				ei (0xF0==(cc&0xF8)) { extra=3; value=cc&0x07; }
				else 		{ ++fault; cp=encoder(cp,cc,context); }
				}
			}
		if (extra)  { ++fault; cp=encoder(cp,value<<(6*extra),context); };
		}
	ei (isizeN==2) {
		int extra=0;
		for(int len=0;len<countOrNeg1;++len) {
			int cc=((unsigned short*)uNstr)[len];
			if (0xDC00==(cc&0xFC00)) {
				if (extra) { cp=encoder(cp,((extra&0x3FF)<<10|(cc&0x3FF))+0x10000,context); extra=0; }
				else       { ++fault; cp=encoder(cp,cc,context); }
				}
			else {
				if (extra)  { ++fault; cp=encoder(cp,extra,context); extra=0; }
				if (0xD800==(cc&0xFC00)) { extra=cc; }
				else		{ cp=encoder(cp,cc,context); }
				}
			}
		if (extra)  { ++fault; cp=encoder(cp,extra,context); }
		}
	else {
		for(int len=0;len<countOrNeg1;++len) {
			cp=encoder(cp,((int4*)uNstr)[len],context);
			}
		}
	if (faults) *faults=(fault);
	cp=encoder(cp,0,context);
	Obj proto=encoder(0,0,context);
	if (proto) { int4 size=(cp-buffer); CodaCLASS *clas=classO(proto);
		int nel=(size/clas->size);
		return obj_(NewBlock,proto,nel,buffer);
		}
	return(NULL);
	}

Short uc_ToU2(char *u1str,int countOrNeg1,int *faults) {
	return uN_Convert(1,u2_encode,NULL,u1str,countOrNeg1,faults);
	}

Int4 uc_ToU4(char *u1str,int countOrNeg1,int *faults) {
	return uN_Convert(1,u4_encode,NULL,u1str,countOrNeg1,faults);
	}

Char u2_ToUc(pointer u2str,int countOrNeg1,int *faults) {
	return uN_Convert(2,uc_encode,NULL,u2str,countOrNeg1,faults);
	}

Int4 u2_ToU4(pointer u2fstr,int countOrNeg1,int *faults) {
	return uN_Convert(2,u4_encode,NULL,u2fstr,countOrNeg1,faults);
	}

Char u4_ToUc(pointer u4str,int countOrNeg1,int *faults) {
	return uN_Convert(4,uc_encode,NULL,u4str,countOrNeg1,faults);
	}

Short u4_ToU2(pointer u4str,int countOrNeg1,int *faults) {
	return uN_Convert(4,u2_encode,NULL,u4str,countOrNeg1,faults);
	}

CodaClassDef(Bool,_Bool,Root);
CodaClassDef(Data,void,Root);
CodaClassDef(DateString,char,Char);
	extern ConstChar sig_oPrintf;
	extern ConstChar sig_oWrite;
	extern ConstChar sig_oPutc;
	extern ConstChar sig_oGetc;
	extern ConstChar sig_oRead;
	extern ConstChar sig_oTell;
CodaClassDef(FileMem,struct FileMem_,Root);
CodaClassDef(Huge,huge,Root);
CodaClassDef(HugeUID,huge,Huge);
CodaClassDef(Real,double,Root);
	typedef int  $FORMAT23 OSig(oPrintf)(Obj obj,char *format,...) ;
	typedef int  OSig(oWrite)(Obj obj,pointer buffer,int length) ;
	typedef int  OSig(oPutc)(Obj obj,int cc) ;
	typedef int  OSig(oGetc)(Obj obj) ;
	typedef int  OSig(oRead)(Obj obj,pointer buffer,int length) ;
	typedef huge OSig(oTell)(Obj obj) ;
	#define Bool_count Root_get_count
	#define cc_inString(c,str) ((-1)!=cc_pos(c,str))
	#define cs_inString(key,str) ((-1)!=cs_pos(key,str))
	#define Data_count Root_get_count
	#define file_lsType(path) file_typeCode(path,1)
	#define file_type(path)   file_typeCode(path,0)
	#define file_putc(os,cc)  fputc(cc,os)
	#define file_read(is,buf,nel)  fread( buf,1,nel,is)
	#define file_write(os,buf,nel) fwrite(buf,1,nel,os)
	#define Huge_count Root_get_count
	#define Error_F(...) OErrorSet(Char_F(__VA_ARGS__))
	#define OAbort(...) return(Error_F(__VA_ARGS__))
 enum {
	PLIST_UnsortedDict=1,
	PLIST_NL4Leafs=2,
	PLIST_AddComputer=4,
	PLIST_Amp38=8,
	PLIST_Apple=16,
	PLIST_NoEncoding=32,
	PLIST_NoDoctype=64,
	PLIST_NoPVersion=128,
	PLIST_Coda_C     =1<<10,
	PLIST_Binary     =1<<11,
	PLIST_ObjectStream=4096,
	PLIST_Json       =1<<13,
	JSON_Pretty      =1<<14,
	PLIST_Strict     =1<<15,
	PLIST_ITUNES= ( PLIST_UnsortedDict | PLIST_AddComputer | PLIST_Amp38 ),
	};
	#define Real_count Root_get_count
Data Array_ToData(Array array);
extern char Data_base64s[];
int Data_base64decode(char *dp,int dlen,char *sp,int slen);
int Data_base64encode(char *dp,int dlen,char *sp,int slen,int addpad);
Bool Bool_Value(bool value);
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
Data Data_NewBlock(Data self,int count,pointer address);
Data Data_FromFile(char *path);
DateString DateString_Value(char *string);
DateString DateString_FromGmtime(struct_tm *when);
struct_tm* DateString_toGmtime(DateString self);
pointer Gmtime_check(struct_tm *self);
huge file_size(char *path);
huge file_time(char *path);
int file_typeCode(char *path,bool reportlinks);
bool File_protocol(Obj obj);
FileMem FileMem_Open(Data optional);
Array FileMem_array(FileMem self);
void FileMem_flush(FileMem self);
int FileMem_oPutc(FileMem self,int cc);
int $FORMAT23 FileMem_oPrintf(FileMem self,char *format,...);
Data FileMem_ToData(FileMem self);
int FileMem_oGetc(FileMem self);
int FileMem_oRead(FileMem self,char *buffer,int length);
Huge Huge_Value(huge value);
Huge Huge_NewBlock(Huge self,int count,pointer address);
HugeUID HugeUID_Value(huge value);
Void JsonNull_Value(void);
bool isa_JsonNull(Obj obj);
Obj OErrorSet(Obj $CONSUMED obj);
Obj OErrorObject(void);
Char OError(void);
void CList_addLeaf(char *xmltag,Obj cobj);
int4 PList_lastLoadType(void);
Obj PList_FromBlock(int count,pointer block,int flags);
Obj PList_toStream(Obj stream,Obj container,int flags);
Obj PList_save(char *file,Obj container,int flags);
Obj PList_Load(char *file,int flags);
CDictionary Plugin_dictionary(void);
void Plugin_register(char *proto,pointer address);
pointer Plugin_fromProto(char *proto);
void PList_Binary(void);
void pointer_sort(pointer base,int nel,void *IfunVVC,void *context);
Real Real_Value(double value);
Real Real_NewBlock(Real self,int count,pointer address);

bool cc_isCap(int c)   { return( c>='A' && c<= 'Z' ); }
bool cc_isLow(int c)   { return( c>='a' && c<= 'z' ); }
bool cc_isAlpha(int c) { return( cc_isLow(c) || cc_isCap(c) ); }
bool cc_is09(int c)    { return( c>='0' && c<= '9' ); }
bool cc_isWhite(int c) { return cc_inString(c," \t\n\r\f"); }

int cc_toCap(int c)   { return( c>='a' && c<='z' ? c-32 : c); }
int cc_toLow(int c)   { return( c>='A' && c<='Z' ? c+32 : c); }

int cc_pos(char c,const char *str) {
	for(const char *cp=str;*cp;++cp) if (*cp==c) return(cp-str);
	return(EOF);
	}

int cc_posCap(char c,const char *str)  {
	c=cc_toCap(c);
	for(const char *cp=str;*cp;++cp) if (cc_toCap(*cp)==c) return(cp-str);
	return(EOF);
	}

int cc_revPos(char c,const char *str) {
	const char *cp=str+cs_length(str);
	while(--cp>=str) if (*cp==c) break;
	return(cp-str);
	}

bool cs_prefix(const char *key,const char *str) {
	while(*key) if (*key++ != *str++)	return(0);
	return(1);
	}

bool cs_prefixCap(const char *key,const char *str) {
	while(*key) if (cc_toCap(*key++)!=cc_toCap(*str++))	return(0);
	return(1);
	}

void cs_copy(char *dest,const char *sour) {
	int cc; while(1) { cc=(*sour++); if (!cc) break; (*dest++)=cc; }
	*dest=0;
	}

bool cs_exactCap(const char *str1,const char *str2) {
	while(*str1) if (cc_toCap(*str1++)!=cc_toCap(*str2++))	return(0);
	return(*str2==0);
	}

int cs_pos(const char *key,const char *str) {
	int pos=0,j,c; if (!*key) return(EOF);
	c= *key++; if (!*key) return cc_pos(c,str);
	while(1) {
		if (!(j=cc_pos(c,str)+1)) return(EOF);
		str+=j; pos+=j;
		if (cs_prefix(key,str)) return(--pos);
		}
	}

int cs_posCap(const char *key,const char *str) {
	int pos=0,j,cc; if (!*key) return(EOF);
	cc= *key++; if (!*key) return cc_posCap(cc,str);
	while(1) {
		if (!(j=cc_posCap(cc,str)+1)) return(EOF);
		str+=j; pos+=j;
		if (cs_prefixCap(key,str)) return(--pos);
		}
	}

int4 cs_toInt4(const char *string) {
	return strtol(string,0,10);
	}

double cs_toReal(const char *string) {
	return strtod(string,0);
	}

char Data_base64s[]=
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	#define base64s	Data_base64s

int Data_base64decode(char *dp,int dlen,char *sp,int slen) {
	int j,px=0,nel=0,out=0;
	for(j=0;j<slen;++j) { int cc=sp[j];
		if (cc_isWhite(cc)) continue;
		int ix=cc_pos(cc,base64s); if (ix==EOF) continue;
		if (out>=dlen)							return(-2);
		switch(nel%4) {
			case 0:  break;
			case 1:  dp[out++]= ((px<<2)&0xfc)|((ix>>4)&0x03); break;
			case 2:  dp[out++]= ((px<<4)&0xf0)|((ix>>2)&0x0f); break;
			case 3:  dp[out++]= ((px<<6)&0xc0)|( ix    &0x3f); break;
			}
		px=ix; ++nel;
		}
	return(out);
	}

int Data_base64encode(char *dp,int dlen,char *sp,int slen,int addpad) {
	int j,ix,px=0,out=0;
	for(j=0;j<slen;++j) { ix=sp[j];
		if (out+1>=dlen)						return(-2);
		switch(j%3) {
			case 0:  dp[out++]=base64s[                (ix>>2)&0x3f ]; break;
			case 1:  dp[out++]=base64s[((px<<4)&0x30)|((ix>>4)&0x0f)]; break;
			case 2:  dp[out++]=base64s[((px<<2)&0x3c)|((ix>>6)&0x03)];
					 dp[out++]=base64s[(                ix    &0x3f)]; break;
			}
		px=ix;
		}
	ix=0;
		if (out+(addpad?2:0)>=dlen)				return(-2);
		switch(j%3) {
			case 1:  dp[out++]=base64s[((px<<4)&0x30)|((ix>>4)&0x0f)];
					 if (addpad) dp[out++]='=';
					 if (addpad) dp[out++]='=';
					 break;
			case 2:  dp[out++]=base64s[((px<<2)&0x3c)|((ix>>6)&0x03)];
					 if (addpad) dp[out++]='=';
					 break;
			}
	return(out);
	}

	#undef base64s

	static __thread Obj lastError=0;

Obj OErrorSet(Obj $CONSUMED obj) {
	freeO(lastError);
	lastError=obj;
	return(0);
	}

Obj OErrorObject() {
	return(lastError);
	}

Char OError() {
	if (!lastError) return(0);
	if (isa_(lastError,Char)) return(lastError);
	if (isa_(lastError,Dictionary)) {
		Char desc=Dict_sub(lastError,"description");
		if (isa_(desc,Char)) return(desc);
		return(Os("Error Object--Dictionary w/o 'description'"));
		}
	return Os("Error Object--unknown.");
	}

#define class Bool
CodaClassZerosC();
CodaClass(Bool,_Bool,Root);

class Bool_Value(bool value) {
	class self=newO(class); *self=value; return(self);
	}

Char $(Info) { return Char_F("%s[%d]=(%d,...) aka _Bool",kindO(self),sizeO(self),*self); }

class $(FromString,char *string) {
	char *str=string;
	while(cc_isWhite(*str)) ++str;
	if (!*str) OAbort("empty <bool> value ''");
	int ival=strtol(str,0,10);
	if (cc_inString(cc_toCap(*str),"TY1")) ival=1;
	return Bool_Value(ival);
	}

Char $(ToString) { return ( *self ? Os("true"):Os("false")); }

Char $(xmlTag) { return Os("bool"); }

Char $(ToDelimiter,int index) {
	static Char da[]={ Os(", "), Os("<bool>["), Os("]"), Os("?") }; return da[index&3];
	}

Char $(ToStringSub,int index) { return Bool_ToString(self); }

$boot(StephenMJones) {
	CodaSig(Info);
	CodaSig(FromString); CodaSig(ToString); CodaSig(xmlTag);
	CodaSig(ToDelimiter); CodaSig(ToStringSub);
	}

#undef class

#define class Data
CodaClassZerosC();
CodaClass(Data,void,Root);

class $(NewBlock,int count,pointer address) {
	if (count<=0) return(0);
	self=newOC(class,count); cs_blockCopy(self,address,count); return(self);
	}

Char $(Info) { return Char_F("%s[%d] aka void*",kindO(self),Data_count(self)); }

class $(FromString,char *string) {
	int len=cs_length(string); char temp[len+16];
	huge hval=Data_base64decode(temp,len+8,string,len);
	if (hval<0) OAbort("base64 data decode error.");
	return Data_NewBlock(0,hval,temp);
	}

Char $(ToString) {
	int len=Data_count(self);
	int maxstr=((len+2)/3)*4+1024;
	Char str=newOC(Char,maxstr);
	int nullat=Data_base64encode(str,maxstr-6,self,len,1);
	str[nullat]=0;
	return(str);
	}

Char $(xmlTag) { return Os("data"); }

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(NewBlock);
	CodaSig(FromString); CodaSig(ToString); CodaSig(xmlTag);
	}
#undef class

Data Data_FromFile(char *path) {
	int type=file_type(path);
	if (type=='?') OAbort("file does not exist.");
	ei (type!='-') OAbort("file is not regular.");
	huge size=file_size(path); if (size<1) OAbort("file is empty.");
	cleanO Data data=newOC(Data,size);
	FILE *is=fopen(path,"rb"); if (!is) OAbort("can't open/read file.");
	huge got=file_read(is,data,size);
	fclose(is);
	if (got!=size) OAbort("did not read entire file?");
	return(keepO(data));
	}

#define class DateString
CodaClassTransC();
CodaClass(DateString,char,Char);

DateString DateString_Value(char *string) {
	int len=cs_length(string)+1; if (len<32) len=32;
	DateString self=newOC(DateString,len); cs_strcopy(self,string); return(self);
	}

Char $(Info) { return Char_F("DateString[%d]=%s aka char*",sizeO(self),(char *)self); }

DateString $(FromString,char *str) {
	if (!DateString_toGmtime(str)) return(0);
	return DateString_Value(str);
	}

Char $(ToString) { return Char_Value(self); }

Char $(xmlTag) { return Os("date"); }

$boot(StephenMJones) {
	CodaSig(Info);
	CodaSig(FromString); CodaSig(ToString); CodaSig(xmlTag);
	}
#undef class

DateString DateString_FromGmtime(struct_tm *when) {
	char bb[128];
	snprintf(bb,sizeof(bb),"%04d-%02d-%02dT%02d:%02d:%02dZ",
		when->tm_year+1900, when->tm_mon+1, when->tm_mday,
		when->tm_hour, when->tm_min, when->tm_sec);
	return DateString_Value(bb);
	}

struct_tm* DateString_toGmtime(DateString self) {
	int len=cs_length(self);
	if (len!=20) OAbort("%s; string length %d, must be %d ?",__func__,len,20);
	char a[32]; cs_copy(a,self);
	static struct_tm when; cs_blockZero(&when,sizeof(when));
	if (a[ 4]!='-' || a[ 7]!='-' || a[10]!='T') OAbort("%s; bad date seperators",__func__);
	if (a[13]!=':' || a[16]!=':' || a[19]!='Z') OAbort("%s; bad time seperators",__func__);
	a[ 4]=0; when.tm_year=cs_toInt4(a+ 0)-1900;
	a[ 7]=0; when.tm_mon =cs_toInt4(a+ 5)-1;
	a[10]=0; when.tm_mday=cs_toInt4(a+ 8);
	a[13]=0; when.tm_hour=cs_toInt4(a+11);
	a[16]=0; when.tm_min =cs_toInt4(a+14);
	a[19]=0; when.tm_sec =cs_toInt4(a+17);
	if (!Gmtime_check(&when)) return(0);
	return(&when);
	}

pointer Gmtime_check(struct_tm *self) {
	if (_ tm_mon <0 || _ tm_mon >11) OAbort("bad date: month=%d", _ tm_mon );
	int dpm[12]={31,29,31,30,31,30,31,31,30,31,30,31};
	int days=dpm[_ tm_mon];
	if (_ tm_mday<1 || _ tm_mday>days) OAbort("bad date: day=%d",_ tm_mday);

	if (_ tm_hour<0 || _ tm_hour>23) OAbort("bad time: hour=%d",_ tm_hour);
	if (_ tm_min <0 || _ tm_min >59) OAbort("bad time: min=%d", _ tm_min );
	if (_ tm_sec <0 || _ tm_sec >60) OAbort("bad time: sec=%d", _ tm_sec );
	return(self);
	}

huge file_size(char *path) {
	struct stat a; if (stat(path,&a)) return(EOF);
	return(a.st_size);
	}

huge file_time(char *path) {
	struct stat a; if (stat(path,&a)) return(EOF);
	return(a.st_mtime);
	}

	#if WIN32
		#define lstat stat
		#define S_IFLNK  0xC300
		#define S_IFSOCK 0xC400
	#endif

int file_typeCode(char *path,bool reportlinks) {
	struct stat a;
	if (reportlinks) { if (lstat(path,&a)) return('?'); }
	  else           { if (stat(path,&a))  return('?'); }
	switch(a.st_mode & S_IFMT)	{
		case S_IFREG:	return('-');
		case S_IFDIR:	return('d');
		case S_IFCHR:	return('c');
		case S_IFBLK:	return('b');
		case S_IFIFO:	return('p');
		case S_IFLNK:	return('l');
		case S_IFSOCK:	return('s');
		default: 		return('u');
		}
	}

typedef int  $FORMAT23 OSig(oPrintf)(Obj obj,char *format,...) ; sig_(oPrintf);
typedef int  OSig(oWrite)(Obj obj,pointer buffer,int length) ;   sig_(oWrite);
typedef int  OSig(oPutc)(Obj obj,int cc) ;                       sig_(oPutc);
typedef int  OSig(oGetc)(Obj obj) ;                              sig_(oGetc);
typedef int  OSig(oRead)(Obj obj,pointer buffer,int length) ;    sig_(oRead);
typedef huge OSig(oTell)(Obj obj) ;                              sig_(oTell);

bool File_protocol(Obj obj) {
	Char sigs[]={sig_oPrintf,sig_oWrite,sig_oPutc,sig_oGetc,sig_oRead,sig_oTell,0};
	for(int j=0;sigs[j];++j) {
		Char sig=sigs[j];
		if (!ORespond2(obj,sig,0,0)) {
			Error_F("%s; obj(type=%s) does not respond to(%s).",__func__,kindO(obj),sig);
			return(0);
			}
		}
	return(1);
	}

Data Array_ToData(Array array) {
	if (!array) OAbort("null array?");
	int4 total=0,nel=Array_count(array);
	for(int j=0;j<nel;++j) total+=sizeO(Array_sub(array,j));
	if (nel<1 || total<1) OAbort("empty array?");
	Data data=newOC(Data,total);
	char *at=data;
	for(int j=0;j<nel;++j) {
		Obj obj=Array_sub(array,j);
		int4 size=sizeO(obj);
		if (size) { cs_blockCopy(at,obj,size); at+=size; }
		}
	return(data);
	}

	struct FileMem_ {
		Array array;
		int4 readIndex,readPos;
		int4 bufPos,tellsize;
		char buffer[BUFSIZ];
		};

#define class FileMem

static void $(dtor) { freeO(_ array); }
static void $(itor) { _ array=newO(Array); }

	CodaClassZeros(etor,bits,kize,ekeep);
CodaClass(FileMem,struct FileMem_,Root);

Char $(Info) { return Char_F("FileMem[Array:%d]",Array_count(_ array)); }

class FileMem_Open(Data optional) {
	class self=newO(class);
	if (optional) { Array_addObject(_ array,optional); _ tellsize+=sizeO(optional); }
	return(self);
	}

Array $(array) { _$(flush); return(_ array); }

void $(flush) {
	if ( _ bufPos<1) return;
	Array_takeObject(_ array,Data_NewBlock(0,_ bufPos,_ buffer));
	_ bufPos=0;
	}

huge $(oTell) { return(_ tellsize); }

int $(oWrite,pointer buf,int length) {
	if (_ bufPos + length >  BUFSIZ) _$(flush);
	if (_ bufPos + length <= BUFSIZ) { cs_blockCopy(_ buffer + _ bufPos,buf,length); _ bufPos+=length; }
	else Array_takeObject(_ array,Data_NewBlock(0,length,buf));
	_ tellsize+=length;
	return(length);
	}

int $(oPutc,int cc) { char aa[]={cc,0}; return _$(oWrite,aa,1); }

int $FORMAT23 $(oPrintf,char *format,...) {
	va_list ap; va_start(ap,format);
		int len=0;
			char buf[4096]; len=vsnprintf(buf,sizeof(buf),format,ap);
			char *cp=0; if (len>=sizeof(buf)) {
				len=vasprintf(&cp,format,ap); Assert_(cp);
				}
			char *ptr=(cp?cp:buf);
			_$(oWrite,ptr,len);
			free(cp);
		va_end(ap);
	return(len);
	}

Data $(ToData) {
	_$(flush);
	return Array_ToData(_ array);
	}

int $(oGetc) {
	_$(flush);
	int nel=Array_count(_ array);
	if (_ readIndex<0 || _ readIndex>=nel) return(EOF);
	Data data=Array_sub(_ array,_ readIndex);
	int size=sizeO(data);
	if (_ readPos<0 || _ readPos>=size) return(EOF);
	unsigned char *buf=data; int cc=buf[_ readPos];
	++ _ readPos;
	if (_ readPos>=size) { _ readPos=0; ++ _ readIndex; }
	return(cc);
	}

int $(oRead,char *buffer,int length) {
	if (length<1) return(0);
	_$(flush);
	int nel=Array_count(_ array);
	int out=0;
	while(out<length) {
		if (_ readIndex<0 || _ readIndex>=nel) break;
		Data data=Array_sub(_ array,_ readIndex);
		int size=sizeO(data);
		if (_ readPos<0 || _ readPos>=size) break;
		int avail=size - _ readPos;
		int want=length - out;
		int xfer=(want<avail ? want : avail);
		cs_blockCopy(buffer+out,data + _ readPos,xfer);
		out += xfer; _ readPos += xfer;
		if (_ readPos>=size) { _ readPos=0; ++ _ readIndex; }
		}
	return(out ? out : EOF);
	}

$boot(StephenMJones) { CodaSig(Info);
	CodaSig(oPrintf); CodaSig(oWrite); CodaSig(oPutc);
	CodaSig(oGetc);   CodaSig(oRead);
	CodaSig(oTell);
	}

#undef class

#define class Huge
CodaClassZerosC();
CodaClass(Huge,huge,Root);

class Huge_Value(huge value) {
	class self=newO(class); *self=value; return(self);
	}

class $(NewBlock,int count,pointer address) {
	if (count<=0) return(0);
	self=newOC(class,count); cs_blockCopy(self,address,count*sizeat(class)); return(self);
	}

Char $(Info) { return Char_F("%s[%d]=(%lld,...) aka huge*",kindO(self),Huge_count(self),*self); }

class $(FromString,char *string) {
	char *str=string;
	while(cc_isWhite(*str)) ++str;
	if (!*str) OAbort("empty <integer> value ''");
	if (str[0]=='0' && (str[1]=='x' || str[1]=='X'))
		return Huge_Value( strtoll(str,0,16) );
	return Huge_Value( strtoll(str,0,10) );
	}

Char $(xmlTag) { return Os("integer"); }

Char $(ToString) { return Char_F("%lld",*self); }

Char $(ToDelimiter,int index) {
	static Char da[]={ Os(", "), Os("<huge>["), Os("]"), Os("?") }; return da[index&3];
	}

Char $(ToStringSub,int index) { return Char_F("%lld",self[index]); }

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(NewBlock);
	CodaSig(FromString); CodaSig(ToString); CodaSig(xmlTag);
	CodaSig(ToDelimiter); CodaSig(ToStringSub);
	}
#undef class

#define class HugeUID
CodaClassTransC();
CodaClass(HugeUID,huge,Huge);

class HugeUID_Value(huge value) {
	huge *hp=newO(class); *hp=value; return(hp);
	}

Char $(Info) {
	return Char_F("HugeUID[%d]=(%lld,...) aka UID", Root_count(self), *self);
	}

	Huge Huge_FromString(Huge self,char *string);

class $(FromString,char *string) {
	self=Huge_FromString(0,string); if (!self) return(0);
	Memory_reclass(self,&Isa_HugeUID);
	return(self);
	}

Char $(xmlTag) { return Os("uid"); }

Char $(ToDelimiter,int index) {
	static Char da[]={ Os(", "), Os("<uid>["), Os("]"), Os("?") }; return da[index&3];
	}

Char $(ToStringSub,int index) { return Char_F("%lld<uid>",self[index]); }

$boot(StephenMJones) {
	CodaSig(Info);
	CodaSig(FromString); CodaSig(xmlTag);
	CodaSig(ToDelimiter); CodaSig(ToStringSub);
	CList_addLeaf("uid",Class_HugeUID);
	}
#undef class

Void JsonNull_Value() { return alocO(1); }

bool isa_JsonNull(Obj obj) { return( isa_(obj,Void) && sizeO(obj)==1 );  }

	static char *xmlkeya[]={"amp;","lt;","gt;","quot;","apos;",NULL};
	static char  xmlchra[]="&<>\"\'";

	static int decode1amp(char *string,int j,int *kp) {
		char *str=string+j+1;
		int x=0; for(x=0;xmlkeya[x];++x) {
			if (cs_prefix(xmlkeya[x],str)) break;
			}
		if (xmlkeya[x]) {
			string[(*kp)++]=xmlchra[x];
			return cs_length(xmlkeya[x]);
			}
		if (*str=='#') {
			++str;
			int pos=cc_pos(';',str);
			if (pos!=EOF) {
				int utf=0;
				if (*str=='x') utf=strtoll(str+1,0,16);
				   else		   utf=strtoll(str,0,10);
				if (utf<=0) return(0);
				char *buff=string+(*kp);
				char *endcp=uc_encode(buff,utf,0);
				int len=endcp-buff; if (len<1 || len>4) return(0);
				(*kp)+=len;
				return(pos+2);
				}
			}
		return(0);
		}

bool PList_stringDecode(char *string,int *errPos) {
	int j,k,len=cs_length(string);
	for(j=0,k=0;j<len;++j) {
		int cc=string[j];
		if (cc=='&') {
			int len=decode1amp(string,j,&k);
			if (!len) {
				if (errPos) *errPos=j;
				return(0);
				}
			j+=len;
			}
		  else { string[k++]=cc; }
		}
	string[k]=0;
	return(1);
	}

static int encodedLength(char *a) {
	int len=0;
	for(int j=0;a[j];++j) {
		int cc=a[j];
		if (cc=='&') len+=5;
		ei (cc=='<' || cc=='>') len+=4;
		else ++len;
		}
	return(len);
	}

	static char *enckeya[]={"amp;","lt;","gt;","#38;"};
	static char  encchra[]="&<>&";
	static char *ituneka[]={"#38;","lt;","gt;","amp;"};

char* PList_stringEncode(int bufsize,char *buffer,char *a,Char *extra,bool amp38) {
	bufsize-=6;
	if (extra) {
		int size=encodedLength(a)+1;
		if (size>bufsize) { bufsize=size; buffer=alocO(size); *extra=buffer; }
		}
	int k=0;
	for(int j=0;a[j];++j) {
		int cc=a[j];
		if (k>=bufsize) OAbort("%s; overflow %d.",__func__,k);
		int p=cc_pos(cc,encchra);
		if (p==EOF)	{ buffer[k++]=cc; }
		  else	{
			char *key=(!amp38 ? enckeya[p] : ituneka[p]);
			sprintf(buffer+k,"&%s",key);
			k+=cs_length(key)+1;
			}
		}
	buffer[k]=0;
	return(buffer);
	}

	static CDictionary CListClassDict=0;

static CDictionary CList_leafs() {
	if (!CListClassDict) {
		static bool once=1;
		if (once) { once=0; Global_objectAddress(&CListClassDict); }

		CListClassDict=newO(CDictionary);
		}
	return CListClassDict;
	}

void CList_addLeaf(char *xmltag,Obj cobj) {
	Dict_set(CList_leafs(),xmltag,cobj);
	}

Obj CLIst_leafToClass(char *xmltag) {
	return Dict_sub(CList_leafs(),xmltag);
	}

bool CList_leafClass(Obj obj) {
	CodaCLASS *clas=classO(obj);
	for(Keyword key=Dictionary_scan(CList_leafs());key;key=Dictionary_next(key)) {
		if (clas==classO(key->item)) return(1);
		}
	return(0);
	}

$boot(CList) { CList_leafs(); }

	static pointer table[]={
		&OClass(Char), &OClass(Huge), &OClass(Bool),
		&OClass(Real), &OClass(Data), &OClass(DateString),
		0};

	static CDictionary PListClassDict=0;

	static void PList_leafs_init() {
		if (!PListClassDict) {
			static bool once=1;
			if (once) { once=0; Global_objectAddress(&PListClassDict); }

			PListClassDict=newO(CDictionary);
			}
		}

	static CDictionary PList_leafs() {
		PList_leafs_init();
		if (Dictionary_count(PListClassDict)==0) {
			void **vpp=0;
			for(int j=0;0!=(vpp=table[j]);++j) {
				Obj cobj=(*vpp); Assert_(cobj);
				Char str=obj_(xmlTag,cobj); Assert_(str);
				Dict_set(PListClassDict,str,cobj);
				}
			}
		return PListClassDict;
		}

Obj PLIst_leafToClass(char *xmlkind,bool apple) {
	Obj ret=Dict_sub(PList_leafs(),xmlkind);
	if (apple || ret) return(ret);
	return CLIst_leafToClass(xmlkind);
	}

bool PList_leafClass(Obj obj,bool apple) {
	CodaCLASS *clas=classO(obj);
	void **vpp=0;
	for(int j=0;0!=(vpp=table[j]);++j) {
		Obj cobj=(*vpp);
		if (clas==classO(cobj)) return(1);
		}
	if (clas== &Isa_ConstChar) return(1);
	if (apple) return(0);
	return CList_leafClass(obj);
	}

$boot(PList) { PList_leafs_init(); }

	#define ALEN 4096

	typedef struct PLPTR1_ {
		unsigned char* blob;
		int lno,pos,blobNel,iLevel;
		char a[ALEN+16];
		bool apple;
		} *PLPTR1;
	#undef  Self
	#define Self PLPTR1

	enum { MAXioLevel=100, };

	static pointer ptrAbort(Self self,Char msg) {
		Error_F("line %d: %s",_ lno+1,msg);
		freeO(msg); msg=0;
		return(0);
		}

  	#define abortP(...)	return(ptrAbort(self,Char_F(__VA_ARGS__)))

	static int ptr_getCc(Self self) {
		if (_ pos >= _ blobNel) return(EOF);
		int cc=_ blob[_ pos++];
		if (cc=='\n') ++_ lno;
		return(cc);
		}
	static void ptr_ungetCc(Self self,int count) {
		_ pos-=count;
		if (_ pos<0) _ pos=0;
		}
	static pointer slurpComment(Self self) {
		if ('-' != ptr_getCc(self)) abortP("Bad comment starting '-'");
		ei ('-' != ptr_getCc(self)) abortP("Bad comment starting '--'");
		else for(int dashes=0;1;) {
			int cc=ptr_getCc(self); if (cc==EOF) abortP("EOF in comment?");
			if (cc=='-') ++dashes;
			ei (cc=='>') {
				if (dashes>=2) return(self);
				}
			else dashes=0;
			}
		}

	static pointer ptrToken(Self self,int flags) {
		int cc,started=flags&1,killspace= !(flags&2);
		if (!started) while(1)	{
			cc=ptr_getCc(self); if (cc==EOF) abortP("EOF starting Token.");
			if (cc=='<') break;
			if (cc=='/') {
				int ncc=ptr_getCc(self);
				if (ncc=='/') {
					while(1) { ncc=ptr_getCc(self); if (ncc=='\n' || ncc==EOF) break; }
					cc='\n';
					}
				}
			if (!cc_isWhite(cc)) abortP("Token start bad cc %d.",cc);
			}
		int p=0;
		while(1)	{
			cc=ptr_getCc(self); if (cc==EOF) abortP("EOF reading Token.");
			if (cc=='>') break;
			if (cc==32 && killspace) continue;
			if (p>=ALEN)					abortP("Token too large. %d",p);
			if (p==0 && cc=='!') {
				int c1=ptr_getCc(self);
				int c2=ptr_getCc(self);
				ptr_ungetCc(self,2);
				if (c1=='-' && c2=='-') {
					if (!slurpComment(self)) return(0);
					return ptrToken(self,flags&2);
					}
				}
			_ a[p++]=cc;
			}
		_ a[p]=0;
		return(self);
		}
	static char *plistParts4[]={
		"?xml version=\"1.0\" encoding=\"UTF-8\"?",
		"!DOCTYPE plist PUBLIC",
		"!DOCTYPE codalist SYSTEM",
		NULL};

	static pointer ptrLoadTop(Self self,int4 *typeptr) {
		int type=0;
		if (!ptrToken(self,2)) return(0);
		if (cs_prefix("?xml",_ a)) {
			if (!cs_exact(plistParts4[0],_ a)) 	abortP("wrong 1st token: xml version.");
			if (!ptrToken(self,2)) return(0);
			}
		if (cs_prefix("!DOC",_ a)) {
			if (cs_prefix(plistParts4[1],_ a)) 	;
			ei (cs_prefix(plistParts4[2],_ a)) 	;
			else abortP("wrong 2nd token lhs: DOCTYPE plist | DOCTYPE codalist.");
			if (!ptrToken(self,2)) return(0);
			}
		if (cs_prefix("plist",_ a)) 	{ type=PLIST_Apple; _ apple=1; }
		ei (cs_exact("codalist",_ a)) 	{ type=0; _ apple=0; }
		else abortP("wrong 3rd token: plist | codalist.(%s)",_ a);

		if (typeptr) *typeptr=type;
		return(self);
		}

	static pointer ptrStringDecode(Self self,char *buffer) {
		int errpos=0;
		if (!PList_stringDecode(buffer,&errpos)) {
			abortP("StringDecode(%-8.8s) error.",_ a+errpos);
			}
		return(self);
		}
	static int ptr_ltSize(Self self) {
		for(int j=_ pos; j< _ blobNel;++j) {
			if (_ blob[j]=='<') return(j - _ pos);
			}
		return(EOF);
		}
	static pointer ptrLoadKey(Self self,Char *extra) {
		int cc,p=0;
		int size=ptr_ltSize(self); if (size==EOF) abortP("EOF loading KEY.");
		cleanO Char temp=(size<=ALEN ? 0 : newOC(Char,size+8));
		if (temp && !extra) abortP("Item too large: %d w/o extra",size);
		char *buffer=(temp ? temp : _ a);
		while(1)	{
			cc=ptr_getCc(self); if (cc==EOF) abortP("EOF in LoadKey.(NEVER)");
			if (cc=='<') break;
			if (p>=size) abortP("Key|Item too large. %d (extra %p) NEVER",p,extra);
			buffer[p++]=cc;
			}
		buffer[p]=0;
		if (!ptrStringDecode(self,buffer)) return(0);
		if (temp) *extra=keepO(temp);
		return(self);
		}
	static Char aNull=Os("<null>");
				static pointer ptrLoadItem(Self self,char *ender) ;
	static pointer ptrLoadDict(Self self,Dictionary dict) {
		while(1)	{
			if (!ptrToken(self,0)) return(0);
			if (cs_exact(_ a,"/dict")) break;
			if (!cs_exact(_ a,"key")) abortP("DictLoad: entries must start with <key> got <%s>",_ a);
			char key[ALEN+16];
				if (!ptrLoadKey(self,0)) return(0);
				if (_ apple && uc_faults(_ a)) abortP("Non-UTF8 dict:key(%s)",_ a);
				cs_strcopy(key,_ a);
			if (!ptrToken(self,1)) return(0);
			if (!cs_exact(_ a,"/key")) abortP("DictLoad: key ending token error </key> got <%s>",_ a);
			Obj obj=ptrLoadItem(self,0); if (!obj) return(0);
			if (obj==aNull) obj=JsonNull_Value();
			Dict_take(dict,key,obj);
			}
		return("OK");
		}
	#undef ALEN

	static pointer ptrLoadArray(Self self,Array array) {
		for(int j=0;1;++j)	{
			static Char ender=Os("/array");
			Obj obj=ptrLoadItem(self,ender); if (!obj) return(0);
			if (obj==ender) break;
			if (obj==aNull) obj=0;
			Array_take(array,obj);
			}
		return("OK");
		}
	static void decLevel(const void *vp) {
		Self self=(*(Self*)vp);
		--_ iLevel;
		}
	static void SelfNOP(Self junk) { }

	static Obj ptrLoadItem(Self self,char *ender) {
		if (++_ iLevel>MAXioLevel) OAbort("too many levels(%d). Circular?",_ iLevel);
		const $CLEANUP(decLevel) Self selfcopy= self;
			SelfNOP(selfcopy);

		if (!ptrToken(self,0)) return(0);
		char *kind=_ a;
		if (ender && cs_exact(kind,ender)) return(ender);
		if (cs_exact("dict",kind)) {
			Dictionary dict=newO(Dictionary);
			if (!ptrLoadDict(self,dict)) { freeO(dict); return(0); }
			if (Dictionary_count(dict)==1) {
				Keyword key=Dictionary_scan(dict);
				if (cs_exact("CF$UID",key->word) && isa_(key->item,Huge)) {
					Huge hhh=key->item;
					HugeUID uuu=HugeUID_Value(*hhh);
					freeO(dict);
					return(uuu);
					}
				ei (cs_exact("CF$Null",key->word) && isa_(key->item,Bool)) {
					freeO(dict);
					return aNull;
					}
				}
			return dict;
			}
		if (cs_exact("array",kind)) {
			Array array=newO(Array);
			if (!ptrLoadArray(self,array)) { freeO(array); return(0); }
			return array;
			}
		if (cs_exact("keyword",kind)) {
			if (!ptrLoadKey(self,0)) return(0);
			cleanO Keyword kkk=Keyword_Value(_ a,0);
			if (!ptrToken(self,1)) return(0);
			if (*_ a!='/' || !cs_exact(_ a+1,"keyword"))
				abortP("Keyword: end token </%s> got <%s>","keyword",_ a);
			Obj obj=ptrLoadItem(self,0); if (!obj) return(0);
			if (OResponds(obj,set_name)) {
				obj_(set_name,obj,kkk->word);
				if (isa_(obj,Array) && cs_exact(kkk->word,Os_Set)) Array_toaSet(obj);
				return(obj);
				}
			kkk->item=obj;
			return keepO(kkk);
			}
		int endslash=(*kind && kind[cs_length(kind)-1]=='/');
		if (endslash) {
			if (cs_exact("dict/", kind)) return newO(Dictionary);
			if (cs_exact("array/",kind)) return newO(Array);
			if (cs_exact("true/", kind)) return Bool_Value(1);
			if (cs_exact("false/",kind)) return Bool_Value(0);
			if (cs_exact("string/",kind)) return Char_Value("");
			if (cs_exact("null/",kind)) return aNull;
			}
			int toklen=cs_length(kind); if (toklen>60) abortP("token too large(%-20.20s...)",kind);
			char token[toklen+1]; cs_strcopy(token,kind);
			kind=token;
		Char extra=0;
		if (!ptrLoadKey(self,&extra)) return(0);
		cleanO Char temp=extra;
		char *input=(temp?temp:_ a);

		Obj cobj=PLIst_leafToClass(kind,_ apple);
		if (!cobj) abortP("unknown xml token: <%s>",kind);
		Obj obj=obj_(FromString,cobj,input); if (!obj) abortP("<%s> %s",kind,OError());

		if (cobj==OClass(Char)) {
			if (_ apple && uc_faults(obj))
				abortP("Non-UTF8 string(%s)",(char*)obj);
			}

		if (!ptrToken(self,1)) return(0);
		if (*_ a!='/' || !cs_exact(_ a+1,token)) abortP("Item: end token </%s> got <%s>",token,_ a);

		return(obj);
		}

	static __thread int4 PList_lastLoadTypeThread=0;
int4 PList_lastLoadType() { return(PList_lastLoadTypeThread); }

Obj PList_FromBlock(int count,pointer block,int flags) {
	if (!block) OAbort("Null data!");
	Self self=alocS(sizeat(Self));
		_ blob=block;
		_ blobNel=count;
	if (_ blobNel<16) abortP("Not enough data!");
	if (0==cs_blockCmp(block,"bplist",6)) {
		PList_lastLoadTypeThread=PLIST_Binary;

		char *proto="Obj PList_BinaryLoad(int,pointer);";
		Obj (*loader)(int,pointer)=Plugin_fromProto(proto);
		if (!loader) OAbort("%s; no plugin: %s.",__func__,proto);
		return loader(count,block);
		}
	if (!ptrLoadTop(self,&PList_lastLoadTypeThread)) return(0);

	cleanO Obj obj=ptrLoadItem(self,0); if (!obj) return(0);
		if (!ptrToken(self,0)) return(0);
		if (cs_exact(_ a,"/plist"))    ;
		ei (cs_exact(_ a,"/codalist")) ;
		else abortP("last token must be </plist|codalist> got <%s>",_ a);

	return(keepO(obj));
	}

	#define BLEN (4096+4096)
	typedef struct PLPTR2_ {
		int  iLevel,flags;
		char b[BLEN+16];
		oPrintf oprintf;
		Obj stream;
		} *PLPTR2;
	#undef  Self
	#define Self PLPTR2

	enum { MAXiLevel=100, };

	#define ePrintf(...) self->oprintf(self->stream,__VA_ARGS__)

	#define hasFlag(bitflag) (self->flags & bitflag)

	static pointer saveItem(Self self,Obj obj,int isArray,int indent) ;

Obj PList_toStream(Obj stream,Obj container,int flags) {
	if (flags & PLIST_Binary) {
		char *proto="Obj PList_BinaryWrite(Obj,Obj,int);";
		Obj (*writer)(Obj,Obj,int)=Plugin_fromProto(proto);
		if (!writer) OAbort("%s; no plugin: %s.",__func__,proto);
		return writer(stream,container,flags);
		}
	oPrintf oprintf=(pointer)fprintf;
	if (flags & PLIST_ObjectStream) {
		oprintf=OResponds(stream,oPrintf);
		if (!oprintf) OAbort("stream does not respond to: %s",sig_oPrintf);
		}
	if (!stream)  OAbort("Void stream?");
	Self self=alocS(sizeat(Self));
		_ flags=flags;
		_ oprintf=oprintf;
		_ stream=stream;

	if (!(flags & PLIST_NoEncoding))
		ePrintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

	if (hasFlag(PLIST_Apple)) {
		if (!(flags & PLIST_NoDoctype))
			ePrintf("<!DOCTYPE plist PUBLIC \"-/" "/Apple%s/" "/DTD PLIST 1.0/" "/EN\" "
				"\"http:/" "/www.apple.com/DTDs/PropertyList-1.0.dtd\">\n",
				(hasFlag(PLIST_AddComputer)?" Computer":""));
		if (!(flags & PLIST_NoPVersion)) ePrintf("<plist version=\"1.0\">\n");
		  else						   ePrintf("<plist>\n");
		if (!saveItem(self,container,2,-1)) return(0);
		ePrintf("</plist>\n");
		}
	  else	{
		if (!(flags & PLIST_NoDoctype))
		ePrintf("<!DOCTYPE codalist SYSTEM \"http:/" "/www.coda-c.com/dtds/codalist.dtd\">\n");
		ePrintf("<codalist>\n");
		if (!saveItem(self,container,2,-1)) return(0);
		ePrintf("</codalist>\n");
	  	}
	return(container);
	}

	static void indentEqn(Self self,int indent) {
		char a[32];
		int j; for(j=0;j<indent && j<20;++j) a[j]='\t';
		a[j]=0;
		if (j>0) ePrintf("%s",a);
		}

	static void Data2_PLData2os(Data obj,Self self,int chunk,int indent,Char str) ;

	static pointer saveLeaf(Self self,Obj obj,int isArray,int indent) {
		if (!PList_leafClass(obj, hasFlag(PLIST_Apple)) ) {
			if (isa_(obj,HugeUID)) {
				HugeUID hhh=obj;
				cleanO Dictionary cfuid=newO(Dictionary);
				Dict_take(cfuid,"CF$UID",Huge_Value(*hhh));
				return saveItem(self,cfuid,isArray,indent);
				}
			if (obj==0 || isa_JsonNull(obj)) {
				if(hasFlag(PLIST_Apple)) {
					if (hasFlag(PLIST_Strict))
						OAbort("Strict: <null> is not allowed in Apple XML.");
					cleanO Dictionary cfnull=newO(Dictionary);
					Dict_take(cfnull,"CF$Null",Bool_Value(1));
					return saveItem(self,cfnull,isArray,indent);
					}
				  else {
					int leaf=(isArray==2);
					int dent=(isArray &1);
					if (dent && !leaf) { indentEqn(self,indent); ePrintf("\t"); }

					ePrintf("<null/>\n");
					return(self);
					}
				}
			OAbort("class[%s] is not a supported leaf type.",kindO(obj));
			}
		Char kind=obj_(xmlTag,obj); if (!kind) OAbort("[%s] not is supported a leaf type?",kindO(obj));
		cleanO Char aa=obj_(ToString,obj);
		if (!aa) OAbort("class[%s] did not produce a string with 'ToString'.",kindO(obj));

		Char extra=0;
		if (!isa_(obj,Data)) {
			if (!PList_stringEncode(BLEN,_ b,aa,&extra,hasFlag(PLIST_Amp38))) return(0);
			}
		cleanO Char temp=extra;
		char *buffer=(temp?temp:_ b);

		int leaf=(isArray==2);
		int dent=(isArray &1);
		if (!isArray && (hasFlag(PLIST_NL4Leafs) || isa_(obj,Data))) {
			ePrintf("\n");
			dent=1;
			}
		if (dent && !leaf) { indentEqn(self,indent); ePrintf("\t"); }

		if (isa_(obj,Bool)) ePrintf("<%s/>\n",buffer);
		ei (isa_(obj,Data)) {
			Data2_PLData2os(obj,self,(_ flags>>16)&0xFFFF,indent+1-leaf,aa);
			}
		else ePrintf("<%s>%s</%s>\n",kind,buffer,kind);

		if (hasFlag(PLIST_Apple) && isa_(obj,Char) && uc_faults(buffer))
			OAbort("Non-UTF8 string(%s)",buffer);

		if (isa_(obj,DateString)) {
			if (!DateString_toGmtime(buffer)) return(0);
			}

		return(self);
		}

	static pointer autoName(Self self,Obj container) {
		if (!hasFlag(PLIST_Apple) && OResponds(container,get_name)) {
			Char name=obj_(get_name,container);
			if (name && *name) {
				if (!PList_stringEncode(BLEN,_ b,name,0,0)) return(0);
				ePrintf("<keyword>%s</keyword>",_ b);
				}
			}
		return(self);
		}

	static pointer plist3Array(Self self,Array container,int indent) {
		if (Array_isaSet(container) && hasFlag(PLIST_Strict) && hasFlag(PLIST_Apple)) {
			OAbort("Strict: <Set> not allowed in Apple PList as <array>.");
			}
		indentEqn(self,indent);
		autoName(self,container);
		int j,nel=Array_get_count(container);
		if (nel==0) { ePrintf("<%s/>\n","array"); return(self); }
		ePrintf("<%s>\n","array");
		for(j=0;j<nel;++j) {
			Obj obj=Array_subInt(container,j);
			if (!saveItem(self,obj,1,indent)) return(0);
			}
		indentEqn(self,indent);
		ePrintf("</%s>\n","array");
		return(self);
		}

	static pointer plist3Dict(Self self,Dictionary container,int indent) {
		indentEqn(self,indent);
		autoName(self,container);
		int j,nel=Dictionary_get_count(container);
		if (nel==0) { ePrintf("<%s/>\n","dict"); return(self); }
		ePrintf("<%s>\n","dict");
		cleanO Pointer vector=Dictionary_AllKeys(container);
		if (!hasFlag(PLIST_UnsortedDict)) pointer_sort(vector,nel,strcmp,0);
		for(j=0;j<nel;++j) {
			char *cp=vector[j];
			Obj obj=Dictionary_subKey(container,cp);
			if (!obj) OAbort("can't find obj for(%s) internal error? NEVER",cp);
			if (!PList_stringEncode(BLEN,_ b,cp,0,0)) return(0);
			indentEqn(self,indent);
			ePrintf("\t<key>%s</key>",_ b);
			if (hasFlag(PLIST_Apple) && uc_faults(_ b))
				OAbort("Non-UTF8 Dict Key(%s)",_ b);
			if (!saveItem(self,obj,0,indent)) return(0);
			}
		indentEqn(self,indent);
		ePrintf("</%s>\n","dict");
		return(self);
		}

	static pointer saveItem(Self self,Obj obj,int isArray,int indent) {
		if (++_ iLevel>MAXiLevel) OAbort("too many levels(%d). Circular?",_ iLevel);
		if (isa_(obj,Dictionary)) {
			if (!isArray) ePrintf("\n");
			if (!plist3Dict(self,obj,indent+1)) return(0);
			}
		ei (isa_(obj,Array)) {
			if (!isArray) ePrintf("\n");
			if (!plist3Array(self,obj,indent+1)) return(0);
			}
		ei (isa_(obj,Keyword)) {
			if (hasFlag(PLIST_Apple)) OAbort("Keyword is not supported for Apple PLists");
			Keyword kkk=obj;
			if (!PList_stringEncode(BLEN,_ b,kkk->word,0,0)) return(0);
			  else {
				ePrintf("\t<keyword>%s</keyword>",_ b);
				if (!saveItem(self,kkk->item,0,indent)) return(0);
				}
			}
		else {
			if (!saveLeaf(self,obj,isArray,indent)) return(0);
			}
		--_ iLevel;
		return(self);
		}

	static char* data2White(int ix,int chunk,int indent,int atend) {
		if (chunk<=0) return(0);
		if (ix%chunk && !atend) return(0);
		static char buf[32]="";
		char *cp=buf; *cp++ ='\n';
		int j; for(j=0;j<indent && j<20;++j) *cp++ = '\t';
		*cp=0;
		return(buf);
		}

	static void Data2_PLData2os(Data obj,Self self,int chunk,int indent,Char str) {
		if (chunk && (chunk<4 || chunk>1000)) chunk=72;
		ePrintf("<%s>","data");
			int len=cs_length(str);
		cleanO Char work=newOC(Char,2*len+1024);
		int j,cc,pos=0;
		for(j=0;0!=(cc=str[j]);++j) {
			char *temp=data2White(j,chunk,indent,0);
			if (temp) { int lll=cs_length(temp); cs_strcopy(work+pos,temp); pos+=lll; }
			work[pos++]=cc;
			}
			char *temp=data2White(j,chunk,indent,1);
			if (temp) { int lll=cs_length(temp); cs_strcopy(work+pos,temp); pos+=lll; }
		work[pos]=0;
		ePrintf("%s",work);
		ePrintf("</%s>\n","data");
		}

Obj PList_save(char *file,Obj container,int flags) {
	cleanO FileMem fff=FileMem_Open(0); if (!fff) return(0);
	if (!PList_toStream(fff,container,flags|PLIST_ObjectStream)) return(0);
	cleanO Data data=FileMem_ToData(fff); if (!data) OAbort("NEVER empty?");
	FILE *os=fopen(file,"wb"); if (!os) OAbort("can't write file.");
		huge size=Data_count(data);
		huge nout=file_write(os,data,size);
		if (nout!=size) OAbort("WRITE error %d vs %d?",(int)nout,(int)size);
		if (fclose(os)) OAbort("fclose Error?");
	return(container);
	}

Obj PList_Load(char *file,int flags) {
	cleanO Data data=Data_FromFile(file); if (!data) return(0);
	return PList_FromBlock(Data_count(data),data,flags);
	}

	typedef struct { void *context; int (*fun)(const void *aa, const void *bb, void *context); } ZsortData;

#if LINUX
	static int zsortSwap(const void *aa, const void *bb,void *context) {
		ZsortData *data=(ZsortData*)context;
		return data->fun(*((void **)aa),*((void **)bb),data->context);
		}
#else
	static int zsortSwap(void *context, const void *aa, const void *bb) {
		ZsortData *data=(ZsortData*)context;
		return data->fun(*((void **)aa),*((void **)bb),data->context);
		}
#endif

void pointer_sort(pointer base,int nel,void *IfunVVC,void *context) {
	ZsortData tmp; tmp.context=context; tmp.fun=IfunVVC;
	#if LINUX
		qsort_r(base, nel, sizeof(void *), zsortSwap, &tmp);
	#elif WIN32
		qsort_s(base, nel, sizeof(void *), zsortSwap, &tmp);
	#elif APPLE
		qsort_r(base, nel, sizeof(void *), &tmp, zsortSwap);
	#else
		#error "pointer_sort: unknown operating system";
	#endif
	}

#define class Real
CodaClassZerosC();
CodaClass(Real,double,Root);

class Real_Value(double value) {
	class self=newO(class); *self=value; return(self);
	}

class $(NewBlock,int count,pointer address) {
	if (count<=0) return(0);
	self=newOC(class,count); cs_blockCopy(self,address,count*sizeat(class)); return(self);
	}

Char $(Info) { return Char_F("%s[%d]=(%3.2f,...) aka double*",kindO(self),Real_count(self),*self); }

class $(FromString,char *string) {
	char *str=string;
	while(cc_isWhite(*str)) ++str;
	if (!*str) OAbort("empty <real> value ''");
	return Real_Value( cs_toReal(str) );
	}

Char $(ToString) { return Char_F("%.16e",*self); }

Char $(xmlTag) { return Os("real"); }

Char $(ToDelimiter,int index) {
	static Char da[]={ Os(", "), Os("<real>["), Os("]"), Os("?") }; return da[index&3];
	}

Char $(ToStringSub,int index) { return Char_F("%4.3f",self[index]); }

$boot(StephenMJones) {
	CodaSig(Info); CodaSig(NewBlock);
	CodaSig(FromString); CodaSig(ToString); CodaSig(xmlTag);
	CodaSig(ToDelimiter); CodaSig(ToStringSub);
	}
#undef class

static CDictionary Plugin_dictionary_dict=0;

CDictionary Plugin_dictionary() {
	if (!Plugin_dictionary_dict) {
		Plugin_dictionary_dict=newO(CDictionary);
		Global_objectAddress(&Plugin_dictionary_dict);
		}
	return(Plugin_dictionary_dict);
	}

void Plugin_register(char *proto,pointer address) {
	Dict_set(Plugin_dictionary(),proto,address);
	}

pointer Plugin_fromProto(char *proto) {
    return Dict_sub(Plugin_dictionary(),proto);
	}

#if 1==0
void PList_Binary() {
	}
#endif

Array Char_Split(char* string,char* delimiter) {
	Array array=newO(Array);
	while(1) {
		int p=cs_pos(delimiter,string); if (p==EOF) break;
		Char hunk=Char_F("%.*s",p,string);
		Array_takeObject(array,hunk);
		string+=(p+cs_length(delimiter));
		}
	Array_takeObject(array,Char_Value(string));
	return(array);
	}

void cs_rightTrim(char *str) {
	for(int i=cs_length(str)-1; i>=0 && cc_isWhite(str[i]); --i) str[i]=0;
	}

void cs_leftTrim(char *str) {
	int i; for(i=0;cc_isWhite(str[i]);++i) ;
	if (i) cs_copy(str,str+i);
	}

void cs_trim(char *str) {
	cs_leftTrim(str);
	cs_rightTrim(str);
	}

huge cs_toHuge(const char* str) {
	if (!str) return(0);
	cleanO Char ccc=Char_Value(str);
	cs_trim(ccc);
	if (cs_prefix("0x",ccc)) return strtoll(ccc+2,0,16);
	return strtoll(ccc,0,10);
	}

void Object_leaks_cleaner(void *vp) {
	Huge hhh=(Huge)vp;
	Char msg=(char*)(intptr_t)hhh[2];
	fprintf(stderr,">>> Leaks: %lld (end scope %s:%lld)\n",Object_Counter-hhh[0],msg,hhh[1]);
	}

void Object_leaks_NOP(pointer junk) { }

Char obj_xmlTag(Obj obj) {
	if (!OResponds(obj,xmlTag)) OAbort("object does not respond to 'xmlTag'.");
	return obj_(xmlTag,obj);
	}

Char obj_ToString(Obj obj) {
	if (!OResponds(obj,ToString)) OAbort("object does not respond to 'ToString'.");
	return obj_(ToString,obj);
	}

Char obj_FromString(Obj obj,char *string) {
	if (!OResponds(obj,FromString)) OAbort("object does not respond to 'FromString'.");
	return obj_(FromString,obj,string);
	}

int file_getCs(FILE *f,char *s,int mx) {
	int c=0,i;
	for(i=0;--mx>0 && (c=fgetc(f))!=EOF && c!='\n';++i) s[i]=c;
	s[i]=0;
	return( (c==EOF && i==0) ? EOF : i );
	}

void defer_call_cleanup(void *vp) {
	if (vp) {
		Pointer self=(Pointer)vp;
		void (*fun)(pointer)=self[0];
		if (fun) fun(self[1]);
		}
	}

	enum { UTF8max=0x10FFFF, };
int4 Json_lastLoadType(void);
Obj Json_FromBlock(int count,pointer block,int flags);
Obj Json_toStream(Obj stream,Obj container,int flags);
Obj Json_save(char *file,Obj container,int flags);
Obj Json_Load(char *file,int flags);

	#define ALEN 4096

	typedef struct PLPTR5_ {
		unsigned char* blob;
		int lno,pos,blobNel,iLevel,flags;
		char a[ALEN+16];
		} *PLPTR5;
	#undef  Self
	#define Self PLPTR5

	enum { MAXjiLevel=100, };

	#define hasFlag(bitflag) (self->flags & bitflag)

	static pointer jsonAbort(Self self,Char msg) {
		Error_F("line %d: %s",_ lno+1,msg);
		freeO(msg); msg=0;
		return(0);
		}

	#undef  abortP
  	#define abortP(...)	return(jsonAbort(self,Char_F(__VA_ARGS__)))

	static int jsn_getCc(Self self) {
		if (_ pos >= _ blobNel) return(EOF);
		int cc=_ blob[_ pos++];
		if (cc=='\n') ++_ lno;
		return(cc);
		}
	static void jsn_ungetCc(Self self,int count) {
		_ pos-=count;
		if (_ pos<0) _ pos=0;
		}

	static ConstChar kColon=Os(":");
	static ConstChar kComma=Os(",");

	static int jsn_ltSize(Self self,int thequote) {
		int backs=0;
		for(int j=_ pos; j< _ blobNel;++j) {
			int cc= _ blob[j];
			if (cc==thequote && (backs&1)==0) return(j - _ pos);
			if (cc=='\\') ++backs;
			  else        backs=0;
			}
		return(EOF);
		}
	static pointer gethex4(Self self,int *cpointptr) {
		char buf[8];
		for(int j=0;j<4;++j) {
			int cc=jsn_getCc(self); if (cc==EOF) abortP("EOF Hex[%d]",j);
			if ( (cc>='0' && cc<='9') || (cc>='a' && cc<='z') || (cc>='A' && cc<='Z') ) ;
			else abortP("Bad Hex[%d]= #%d",j,cc);
			buf[j]=cc;
			}
		buf[4]=0;
		*cpointptr=(int)strtoll(buf,0,16);
		return(self);
		}

	static pointer jsnLoadKey(Self self,Char *extra,int thequote) {
		int cc,p=0;
		int size=jsn_ltSize(self,thequote); if (size==EOF) abortP("EOF loading KEY.");
		cleanO Char temp=(size<=ALEN ? 0 : newOC(Char,size+8));
		if (temp && !extra) abortP("Item too large: %d w/o extra",size);
		char *buffer=(temp ? temp : _ a);
		int backs=0;
		while(1) {
			cc=jsn_getCc(self); if (cc==EOF) abortP("EOF in LoadKey.(NEVER)");
			if (cc==thequote && (backs&1)==0) break;
			if (cc=='\\' && !backs) { ++backs; continue; }
			if (backs) { backs=0;
				if (cc=='b') cc='\b';
				ei (cc=='f') cc='\f';
				ei (cc=='t') cc='\t';
				ei (cc=='r') cc='\r';
				ei (cc=='n') cc='\n';
				ei (cc=='u') {
					int cpoint=0; if (!gethex4(self,&cpoint)) return(0);
					if (0xD800==(cpoint &0xFC00)) {
						cc=jsn_getCc(self);
						if (cc!='\\') {
							if (cc!=EOF) jsn_ungetCc(self,1);
							}
						  else {
							int c2=jsn_getCc(self);
							if (c2!='u') {
								jsn_ungetCc(self,c2!=EOF ? 2 : 1);
								}
							  else {
								int lowsur=0; if (!gethex4(self,&lowsur)) return(0);
								if (0xDC00!=(lowsur &0xFC00)) abortP("Bad low surrogate 0x%04x",lowsur);
								cpoint=(((cpoint &0x3FF)<<10 | (lowsur &0x3FF))+0x10000);
								}
							}
						}
					ei (0xDC00==(cpoint &0xFC00)) {
						}
					ei (cpoint==0) {
						cpoint=UTF8max;
						}
					if (p+4>=size) abortP("String too large. %d (extra %p) Never?",p,extra);
					char *buff=buffer+p;
					char *endcp=uc_encode(buff,cpoint,0);
					int len=endcp-buff; if (len<1 || len>4) len=0;
					p+=len;
					continue;
					}
				}
			if (p>=size) abortP("Key|Item too large. %d (extra %p) NEVER",p,extra);
			buffer[p++]=cc;
			}
		buffer[p]=0;
		if (uc_faults(buffer)) abortP("Non-UTF8 dict:key(%s)",buffer);
		if (temp) *extra=keepO(temp);
		return(self);
		}

	#undef ALEN

				static pointer jsnLoadItem(Self self,char *ender,bool top) ;
	static pointer jsnLoadDict(Self self,Dictionary dict) {
		while(1)	{
			static Char ender=Os("}");
			cleanO Obj key=jsnLoadItem(self,ender,0); if (!key) return(0);
			if (key==ender) break;
			if (!isa_(key,Char)) abortP("Bad Dictionary key?");

			cleanO Obj colon=jsnLoadItem(self,0,0); if (!colon) return(0);
			if (colon!=kColon) {
				cleanO Char msg=ToContainer(colon);
				abortP("Dict: missing colon? ( %s )",msg);
				}

			cleanO Obj obj=jsnLoadItem(self,0,0); if (!obj) return(0);
			Dict_set(dict,key,obj);
			cleanO Obj comma=jsnLoadItem(self,ender,0); if (!comma) return(0);
			if (comma==ender) break;
			if (comma!=kComma) {
				cleanO Char msg=ToContainer(comma);
				abortP("Dict: missing comma? ( %s )",msg);
				}
			}
		return("OK");
		}

	static pointer jsnLoadArray(Self self,Array array) {
		for(int j=0;1;++j)	{
			static Char ender=Os("]");
			Obj obj=jsnLoadItem(self,ender,0); if (!obj) return(0);
			if (obj==ender) break;
			if (obj==kComma) continue;
			if (obj==kColon) OAbort("Colon in array?");
			if (isa_JsonNull(obj)) obj=0;
			Array_take(array,obj);
			}
		return("OK");
		}
	static pointer ptrHex(Self self) {
		int j=0;
		for(j=0;j+_ pos < _ blobNel && j<63;++j) {
			int cc= cc_toCap(_ blob[j+_ pos]);
			if (cc>='0' && cc<='9') ;
			ei (cc>='A' && cc<='F') ;
			else break;
			}
		Char string=alocS(j+1); cs_blockCopy(string,_ blob+ _ pos,j);
		_ pos +=j;
		huge ival=strtoll(string,0,16);
		return Huge_Value(ival);
		}

	static pointer ptrNumber(Self self) {
		int c1=jsn_getCc(self);
		int c2=jsn_getCc(self);
		if (c1=='0' && c2=='x') return ptrHex(self);
		jsn_ungetCc(self,c2==EOF ? 1 : 2);

		int j=0;
		for(j=0;j+_ pos < _ blobNel && j<127;++j) {
			int cc= _ blob[j+_ pos];
			if (cc>='0' && cc<='9') ;
			ei (cc=='-' || cc=='.' || cc=='e' || cc=='E' || cc=='+') ;
			else break;
			}
		Char string=alocS(j+1); cs_blockCopy(string,_ blob+ _ pos,j);
		_ pos +=j;
		double dval=strtod(string,0);

		char buf[64]; snprintf(buf,sizeof(buf),"%.16g",dval);
		bool isreal=cc_inString('.',buf);
		if (isreal) return Real_Value(dval);
		return Huge_Value(llround(dval));
		}
		Data Data_FromString(Data self,char *string);
		DateString DateString_FromString(DateString self,char *str);

	static bool nameCc(int cc,bool notfirst) {
		cc&=0xFF;
		if (cc>127) return(1);
		if ( (cc>='a' && cc<='z') || (cc>='A' && cc<='Z') || cc=='_' || cc=='$') return(1);
		if (notfirst && (cc>='0' && cc<='9') ) return(1);
		return(0);
		}

	static void jsndecLevel(const void *vp) {
		Self self=(*(Self*)vp);
		--_ iLevel;
		}
	static void jsnSelfNOP(Self junk) { }

	static Obj jsnLoadItem(Self self,char *ender,bool top) {
		if (++_ iLevel>MAXjiLevel) OAbort("too many levels(%d). Circular?",_ iLevel);
		const $CLEANUP(jsndecLevel) Self selfcopy= self;
			jsnSelfNOP(selfcopy);

		int cc=jsn_getCc(self);
		while(cc==32 || cc=='\t' || cc=='\r' || cc=='\n') cc=jsn_getCc(self);

		if (ender && *ender==cc) return(ender);

		if (cc=='{') {
			Dictionary dict=newO(Dictionary);
			if (!jsnLoadDict(self,dict)) { freeO(dict); return(0); }

			if (Dictionary_count(dict)==1 && !hasFlag(PLIST_Strict)) {
				Keyword key=Dictionary_scan(dict);
				if (cs_exact("CF$UID",key->word) && isa_(key->item,Huge)) {
					Huge rrr=key->item;
					HugeUID uuu=HugeUID_Value( *rrr );
					freeO(dict);
					return(uuu);
					}
				ei (cs_exact("CF$Date",key->word) && isa_(key->item,Char)) {
					Obj ret=DateString_FromString(0,key->item);
					freeO(dict);
					return(ret);
					}
				ei (cs_exact("CF$Data",key->word) && isa_(key->item,Char)) {
					Obj ret=Data_FromString(0,key->item);
					freeO(dict);
					return(ret);
					}
				}
			return dict;
			}
		if (cc=='[') {
			Array array=newO(Array);
			if (!jsnLoadArray(self,array)) { freeO(array); return(0); }
			return array;
			}

		if (cc==':') return kColon;
		if (cc==',') return kComma;

		if (cc=='\"' || cc=='\'') {
			Char extra=0;
			if (!jsnLoadKey(self,&extra,cc)) return(0);
			return(extra ? extra : Char_Value(_ a));
			}

		if (cc_inString(cc,"0123456789-.+")) {
			jsn_ungetCc(self,1);
			return ptrNumber(self);
			}

		if (nameCc(cc,0)) {
			char name[128]={cc};
			int j=0;
			for(j=1;j<sizeof(name)-1;++j) {
				cc=jsn_getCc(self); if (cc==EOF) break;
				if (!nameCc(cc,1)) { jsn_ungetCc(self,1); break; }
				name[j]=cc;
				}
			name[j]=0;
			if (cs_exact(name,"true"))  return Bool_Value(1);
			if (cs_exact(name,"false")) return Bool_Value(0);
			if (cs_exact(name,"null"))  return JsonNull_Value();
			if (top) OAbort("unquoted string as only/top object? (%s)",name);
			return Char_Value(name);
			}

		if (cc=='/') {
			cc=jsn_getCc(self);
			bool toeol=(cc=='/');
			if (!toeol && cc!='*') abortP("Comment start? Slash with out Slash or asterisk.");
			int stars=0;
			while(1) {
				cc=jsn_getCc(self); if (cc==EOF) break;
				if (toeol && cc=='\n') break;
				if (!toeol) {
					if (stars && cc=='/') break;
					if (cc=='*') ++stars;
					  else         stars=0;
					}
				}
			return jsnLoadItem(self,ender,top);
			}

		abortP("Bad char: 0x%02x?",cc);
		}

	static __thread int4 Json_lastLoadTypeThread=0;
int4 Json_lastLoadType() { return(Json_lastLoadTypeThread); }

Obj Json_FromBlock(int count,pointer block,int flags) {
	if (!block) OAbort("Null data!");

	bool redirect=(count>8 && 0==cs_blockCmp(block,"bplist00",8));

	if (!redirect) {
		for(int j=0;j<count;++j) {
			int cc= *(Char)(block+j);
			if (!cc) OAbort("null char found at pos %d?",j);
			}
		}

	if (!redirect) {
		for(int j=0;j<count;++j) {
			int cc= *(Char)(block+j);
			if (cc_isWhite(cc)) ;
			ei (cc=='<') { redirect=1; break; }
			else break;
			}
		}
	if (redirect) {
		Obj obj=PList_FromBlock(count,block,flags);
		if (obj) {
			int flags=PList_lastLoadType();
			if (0==(flags & (PLIST_Apple|PLIST_Binary))) flags|=PLIST_Coda_C;
			Json_lastLoadTypeThread=PList_lastLoadType();
			}
		return(obj);
		}

	Self self=alocS(sizeat(Self));
		_ blob=block;
		_ blobNel=count;
		_ flags=flags;
	Obj obj=jsnLoadItem(self,0,1);
	if (obj) Json_lastLoadTypeThread=PLIST_Json;
	return(obj);
	}

static int json_encodedLength(char *a) {
	int len=0;
	for(int j=0;a[j];++j) {
		int cc=a[j]&0xFF;
		if (cc=='/' || cc=='\\' || cc=='"') len+=2;
		ei (cc<32) len+=6;
		else ++len;
		}
	return(len);
	}

char* Json_stringEncode(int bufsize,char *buffer,char *a,Char *extra) {
	bufsize-=4;
	if (extra) {
		int size=json_encodedLength(a)+1;
		if (size>bufsize) { bufsize=size; buffer=alocO(size); *extra=buffer; }
		}
	int k=0;
	for(int j=0;a[j];++j) {
		int cc=a[j]&0xFF;
		if (k>=bufsize) OAbort("%s; overflow %d.",__func__,k);
		if (cc=='/' || cc=='\\' || cc=='"') buffer[k++]='\\';
		ei (cc<32) {
			buffer[k++]='\\';
			if (cc=='\t') cc='t';
			ei (cc=='\n') cc='n';
			ei (cc=='\r') cc='r';
			ei (cc=='\f') cc='f';
			ei (cc=='\b') cc='b';
			else {
				char temp[16]; snprintf(temp,sizeof(temp),"u%04X",cc);
				if (k+6>=bufsize) OAbort("%s; Overflow %d.",__func__,k);
				cs_copy(buffer+k,temp);
				k+=cs_length(temp);
				continue;
				}
			}
		buffer[k++]=cc;
		}
	buffer[k]=0;
	return(buffer);
	}

	#define BLEN (4096+4096)
	typedef struct PLPTR6_ {
		int  iLevel,flags,indent;
		char b[BLEN+16];
		oPrintf oprintf;
		Obj stream;
		} *PLPTR6;
	#undef  Self
	#define Self PLPTR6

	enum { MAXjoLevel=100, };

	#define ePrintf(...) self->oprintf(self->stream,__VA_ARGS__)

	#define hasFlag(bitflag) (self->flags & bitflag)
	#define isPretty()       (self->flags & JSON_Pretty)

	#define pretty(str) prettySelf(self,str)

	static void prettySelf(Self self,char *str) {
		if (isPretty()) ePrintf("%s",str);
		}

	static pointer jsn_saveItem(Self self,Obj obj,int isArray) ;

Obj Json_toStream(Obj stream,Obj container,int flags) {
	if (!stream)    OAbort("Void stream?");
	if (!container) OAbort("Void container?");

	if (flags & (PLIST_Coda_C | PLIST_Apple | PLIST_Binary))
		return PList_toStream(stream,container,flags);

	oPrintf oprintf=(pointer)fprintf;
	if (flags & PLIST_ObjectStream) {
		oprintf=OResponds(stream,oPrintf);
		if (!oprintf) OAbort("stream does not respond to: %s",sig_oPrintf);
		}
	Self self=alocS(sizeat(Self));
		_ flags=flags;
		_ oprintf=oprintf;
		_ stream=stream;

	if (!jsn_saveItem(self,container,2)) return(0);
	pretty("\n");
	return(container);
	}

	static pointer jprints(Self self,char *string) {
		if (uc_faults(string)) OAbort("Non-UTF8 string(%s)",string);
		ePrintf("\"");
		while(1) {
			static char maxutf[]="\xF4\x8F\xBF\xBF";
			int pos=cs_pos(maxutf,string); if (pos==EOF) break;
			if (pos>0) { ePrintf("%.*s",pos,string); string+=pos; }
			ePrintf("\\u0000");
			string+=4;
			}
		ePrintf("%s\"",string);
		return(self);
		}

		enum { json_max_dent=60, };

	static void jsn_indentEqn(Self self) {
		if (!isPretty()) return;
		char a[json_max_dent*2+8]; int nspace= _ indent*2;
		int j; for(j=0;j<nspace && j<json_max_dent;++j) a[j]=32;
		a[j]=0;
		if (j>0) ePrintf("%s",a);
		}

		bool PList_leafClass(Obj obj,bool apple) ;
	static pointer jsn_saveLeaf(Self self,Obj obj,int isArray) {
		if (!PList_leafClass(obj, 1)) {
			if (isa_JsonNull(obj)) { ePrintf("null"); return(self); }
			if (isa_(obj,HugeUID)) {
				HugeUID hhh=obj;
				if (hasFlag(PLIST_Strict)) OAbort("Strict: <uid> is not supported in JSON.");
				ePrintf("{\"CF$UID\":%lld}",*hhh);
				return(self);
				}
			OAbort("class[%s] is not a supported JSON leaf type.",kindO(obj));
			}
		cleanO Char aa=obj_ToString(obj);
		if (!aa) OAbort("class[%s] did not produce a string with 'ToString'.",kindO(obj));

		if (isa_(obj,Bool)) ePrintf("%s",aa);
		ei (isa_(obj,Real)) {
			char aaa[64]; snprintf(aaa,sizeof(aaa),"%.16g",*(Real)obj);
			int cc=aaa[0];
			if (!cc_inString(cc,"0123456789-.+")) OAbort("Bad Real Value(%s)?",aaa);
			ePrintf("%s",aaa);
			}
		ei (isa_(obj,Huge)) ePrintf("%s",aa);
		ei (isa_(obj,Data)) {
			if (hasFlag(PLIST_Strict)) OAbort("Strict: <data> is not supported in JSON.");
			ePrintf("{\"CF$Data\":\"%s\"}",aa);
			}
		ei (isa_(obj,Char)) {
			Char extra=0;
			if (!Json_stringEncode(BLEN,_ b,aa,&extra)) return(0);
			cleanO Char temp=extra;
			char *buffer=(temp?temp:_ b);

			if (isa_(obj,DateString)) {
					if (hasFlag(PLIST_Strict)) OAbort("Strict: <date> is not supported in JSON.");
				   if (!DateString_toGmtime(obj)) return(0);
		 		   ePrintf("{\"CF$Date\":\"%s\"}",buffer);
				   }
			  else {
				if (!jprints(self,buffer)) return(0);
				}
			}
		else OAbort("class[%s] is not a supported leaf type?",kindO(obj));
		return(self);
		}

	static pointer json_3Array(Self self,Array container) {
		int j,nel=Array_get_count(container);
		ePrintf("[");
		++ _ indent;
		for(j=0;j<nel;++j) {
			if (j) ePrintf(",");
			Obj obj=Array_subInt(container,j);
			pretty("\n"); jsn_indentEqn(self);
			if (!obj) { ePrintf("null"); continue; }
			if (!jsn_saveItem(self,obj,1)) return(0);
			}
		pretty("\n"); jsn_indentEqn(self);
		ePrintf("]");
		-- _ indent;
		return(self);
		}

	static pointer json_3Dict(Self self,Dictionary container) {
		ePrintf("{"); pretty("\n");
		++_ indent;
		int j,nel=Dictionary_get_count(container);
		cleanO Pointer vector=Dictionary_AllKeys(container);
		if (!hasFlag(PLIST_UnsortedDict)) pointer_sort(vector,nel,strcmp,0);
		for(j=0;j<nel;++j) {
			if (j) { ePrintf(","); pretty("\n"); }
			char *cp=vector[j];
			Obj obj=Dictionary_subKey(container,cp);
			if (!obj) OAbort("can't find obj for(%s) internal error? NEVER",cp);
			if (!Json_stringEncode(BLEN,_ b,cp,0)) return(0);
			jsn_indentEqn(self);
			if (!jprints(self,_ b)) return(0);
			if (isPretty()) ePrintf(" : "); else ePrintf(":");
			if (!jsn_saveItem(self,obj,0)) return(0);
			}
		if (nel) pretty("\n");
		jsn_indentEqn(self);
		ePrintf("}");
		--_ indent;
		return(self);
		}

	#undef BLEN

	static pointer jsn_saveItem(Self self,Obj obj,int isArray) {
		if (++_ iLevel>MAXjoLevel) OAbort("too many levels(%d). Circular?",_ iLevel);
		if (isa_(obj,Dictionary)) {
			if (!json_3Dict(self,obj)) return(0);
			}
		ei (isa_(obj,Array)) {
			if (Array_isaSet(obj) && hasFlag(PLIST_Strict)) {
				OAbort("Strict: <Set> not allowed in JSON as <array>.");
				}
			if (!json_3Array(self,obj)) return(0);
			}
		else {
			if (!jsn_saveLeaf(self,obj,isArray)) return(0);
			}
		--_ iLevel;
		return(self);
		}

Obj Json_save(char *file,Obj container,int flags) {
	cleanO FileMem fff=FileMem_Open(0); if (!fff) return(0);
	if (!Json_toStream(fff,container,flags|PLIST_ObjectStream)) return(0);
	cleanO Data data=FileMem_ToData(fff); if (!data) OAbort("NEVER empty?");
	FILE *os=fopen(file,"wb"); if (!os) OAbort("can't write file.");
		huge size=sizeO(data);
		huge nout=file_write(os,data,size);
		if (nout!=size) OAbort("WRITE error %d vs %d?",(int)nout,(int)size);
		if (fclose(os)) OAbort("fclose Error?");
	return(container);
	}

Obj Json_Load(char *file,int flags) {
	cleanO Data data=Data_FromFile(file); if (!data) return(0);
	return Json_FromBlock(sizeO(data),data,flags);
	}

