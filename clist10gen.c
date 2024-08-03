/* www.coda-c.com  clist10gen.c

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

	#include "./coda-c_plist.h"
	#include "./coda-c_strings.h"
	#include <assert.h>

	#define _ self->
	#define ei else if
	#define file_read(is,buf,nel)  fread( buf,1,nel,is)
	#define file_write(os,buf,nel) fwrite(buf,1,nel,os)
	#define Msg_(cs,...)  fprintf(stderr,cs "\n",##__VA_ARGS__)
	#define Quit_(cs,...) fprintf(stderr,cs "\n",##__VA_ARGS__),exit(-1)

void vi_about(bool junk);
void about(void);
void aboute(void);
void abouto(void);
pointer dynamo(void);
void PList_Diag2Level(Obj list,FILE *os,int maxlevel);
Data stdin_read(void);

void vi_about(bool junk) {
	Msg_("About: codalist (Version %s)",QVersion);
	Msg_("%s  (%s)","Coda-C Property List Utility",QWebsite);
	Msg_("-------------------------------------------------------");
	}

void about() {
	printf("\r\n\n");
	vi_about(0);
	printf("Format: codalist [options or file] ...\n");
	printf("Options:\n");
	printf("    -h     About codalist   ## Startup: filename=new.clist, Mode=Coda-C XML, /=<dict>\n");
	printf("    -he    About Encodings  ## arguments/commands are executed in order\n");
	printf("    -ho    About Objects    ## Note: + for flags unsets the flag\n");
	printf("\n");
	printf("    -w     Write the current file\n");
	printf("    -w:$   Write a file where '$' is the file name\n");
	printf("    -w:-   Write to <stdout>\n");
	printf("     $     Load a file where '$' is the file name\n");
	printf("     -     Load <stdin> (capture once, use many)\n");
	printf("    -f:$   Set the file name to '$'\n");
	printf("    +/%%=$  Set an object where '%%' is a Keypath and '$' is an Object\n");
	printf("    -/%%    Delete an object where '%%' is a Keypath\n");
	printf("\n");
	printf("    -v     Verbose(on): display processing messages ## +v (off)\n");
	printf("    -d     No display at end of arguments\n");
	printf("    -dn    Display at end of arguments\n");
	printf("    -dw    Display at end of arguments, if not written (default)\n");
	printf("    -da    Display Apple XML at end of arguments\n");
	printf("    -dc    Display Coda-C XML at end of arguments\n");
	printf("    -dj    Display JSON at end of arguments\n");
	printf("\n");
	printf("    -c     Display copyright and version information\n");
	printf("    -n     Output a newline\n");
	printf("    -i     Display info: filename, top object, & mode\n");
	printf("    -t     Test plist for Coda-C XML, Apple XML, Binary, & JSON formats\n");
	printf("    -q     Quit\n");

	printf("\n");
	}

void aboute() {
	printf("\r\n\n");
	vi_about(0);
	printf("Encoding Options:\n");
	printf("\n");
	printf("  Output File Formats\n");
	printf("    -ec  Serialize to Coda-C XML format (default)\n");
	printf("    -ea  Serialize to Apple XML format\n");
	printf("    -eb  Serialize to Apple Binary format\n");
	printf("    -ej  Serialize to JSON format\n");
	printf("    Note: A PList composed of a single data leaf will write raw data\n");
	printf("\n");
	printf("  Output Options\n");
	printf("    -pretty    Add spaces and new lines to JSON\n");
	printf("    +pretty    Compact JSON (default)\n");
	printf("    -xhead     Do not Output XML Headers\n");
	printf("    +xhead     Output XML Headers (default)\n");
	printf("    -xml1      Add new lines for leafs in XML\n");
	printf("    +xml1      Do not add new lines for leafs in XML (default)\n");
	printf("    -unsorted  Unsorted Dictionary keys\n");
	printf("    +unsorted  Sort Dictionary keys (default)\n");
	printf("    -noes      Do not escape slashes in JSON\n");
	printf("    +noes      Escape slashes in JSON (default)\n");
	printf("\n");
	printf("  Input Options\n");
	printf("    -strict    No JSON conversions\n");
	printf("    +strict    JSON conversion for <uid>, <date>, & <data>. (non-standard) (default)\n");
	printf("\n");
	}

void abouto() {
	printf("\r\n\n");
	vi_about(0);
	printf("Keypaths:\n");
	printf("    Reference an existing container by string or number for array\n");
	printf("    /key/0/key2/5\n");
	printf("  Assignments:\n");
	printf("    Use '+' to add array elements\n");
	printf("    /key/+\n");
	printf("\n");
	printf("New Object types:\n");
	printf("    array (a new container)\n");
	printf("    dict  (a new container)\n");
	printf("    true\n");
	printf("    false\n");
	printf("    null (arrays allow nulls, but not for all formats)\n");
	printf("    jsonnull (placeholder for dictionary JSON null)\n");
	printf("    string:Green_Tea\n");
	printf("    int:12345  (or integer:)\n");
	printf("    uid:12345  (CF$UID)\n");
	printf("    date:1971-12-31T23:23:59Z\n");
	printf("    real:12.345\n");
	printf("    data:BASE64.DATA.AAAA\n");
	printf("    datafile:Myfile.bin  (contents of file as Data)\n");
	printf("    ./FILE[:keypath] (component of plist file)\n");
	printf("    /FILE[:keypath]\n");
	printf("    :keypath (in current plist)\n");
	printf("    JSON  (a valid JSON5 string)\n");
	printf("\n");
	}

	enum { PLIST_ITUNES= ( PLIST_UnsortedDict | PLIST_AddComputer | PLIST_Amp38 ), };

	typedef struct Self_ {
		Obj  plist;
		Char filename;
		int  filemode;
		int  savemode;
		int  datamode;
		int  display;
		int  verbose;
		int  nwrites;
		int  printlevel;
		Data stdinData;
		} *Self;

	static void init(Self self) {
		_ plist=newO(Dictionary);
		_ filename=Char_Value("new.clist");
		_ display=2;
		_ printlevel=10;
		_ savemode=0;
		_ filemode=PLIST_Coda_C;
		_ datamode=72;
		}

	static int outflags(Self self) {
		return( ((_ filemode|_ savemode)&0xFFFF) | ((_ datamode<<16)&0xFFFF0000) );
		}

	static void fini(Self self) {
		if (_ display==2) _ display=(_ nwrites?0:1);
		if (_ display) {
			if (_ display==3) {
				if (!Json_toStream(stdout,_ plist,PLIST_Apple|PLIST_NoEncoding|PLIST_NoDoctype))
					Quit_("Error: %s",OError());
				}
			ei (_ display==4) {
				if (!Json_toStream(stdout,_ plist,PLIST_Json|JSON_Pretty|JSON_NoEscapeSlash))
					Quit_("Error: %s",OError());
				}
			ei (_ display==5) {
				if (!Json_toStream(stdout,_ plist,PLIST_Coda_C|PLIST_NoEncoding|PLIST_NoDoctype))
					Quit_("Error: %s",OError());
				}
			else PList_Diag2Level(_ plist,stdout,_ printlevel);
			}
		}

	static void testplist(Self self) {
		int modea[]={ PLIST_Coda_C, PLIST_Apple, PLIST_Binary, PLIST_Json, 0 };
		Char modes[]={"Coda-C XML", "Apple XML", "Apple Binary", "JSON", 0 };
		for(int j=0;j<4;++j) {
			printf("Testing %12.12s: ",modes[j]);
			cleanO FileMem sink=FileMem_Open(0); assert(sink);
			if (!Json_toStream(sink,_ plist,PLIST_ObjectStream | modea[j] | (PLIST_Strict & outflags(self)) ))
				 printf("%s\n",OError());
			else printf("*** OK ***\n");
			}
		}

	static void writeplist(Self self,char *file) {
		if (isa_(_ plist,Data)) {
			if (_ verbose) Msg_("Saving Raw Data to: %s",(file ? file : "<stdout>"));
			FILE *os=( file ? fopen(file,"wb") : stdout);
			if (!os && file) Quit_("*** can't save Data to: (%s).",file);
			defer_call( (file ? fclose : fflush),os);
			file_write(os,_ plist,sizeO(_ plist));
			}
		else {
			if (file) {
				assert(*file);
				if (_ verbose) Msg_("Saving.: %s",file);
				if (!Json_save(file,_ plist,outflags(self)))
					Quit_("*** can't save file(%s) [%s].",file,OError());
				}
			  else {
				if (_ verbose) Msg_("Saving.: %s","<stdout>");
				if (!Json_toStream(stdout,_ plist,outflags(self)))
					Quit_("*** failed write to stdout [%s].",OError());
				}
			}
		++_ nwrites;
		}

	static void setitem(Self self,char *str) ;
	static void delitem(Self self,char *str) ;

	static void loadfile(Self self,char *file) {
		if (_ verbose) Msg_("Loading: %s",file);
		freeO(_ plist);    _ plist=Json_Load(file,outflags(self));
		if (!_ plist) Quit_("*** can't load file(%s) [%s].",file,OError());
						   _ filemode=PList_lastLoadType();
		freeO(_ filename); _ filename=Char_Value(file);
		}

	static void loadstdin(Self self) {
		if (!_ stdinData) {
			if (_ verbose) Msg_("Loading: %s","<stdin>");
			_ stdinData=stdin_read();
			}
		freeO(_ plist); _ plist=Json_FromBlock(sizeO(_ stdinData),_ stdinData,outflags(self));
		if (!_ plist) Quit_("*** can't load file(%s) [%s].","<stdin>",OError());
		                   _ filemode=Json_lastLoadType();
		freeO(_ filename); _ filename=Os("stdin.clist");
		}

 int main(int argc,char **args) {
	dynamo();
	if (argc==1) { about(); return(0); }

	Self self=alocS(sizeat(Self));
	init(self);
	for(int j=1;j<argc;++j) { char *arg=args[j];
		if (cs_exactCap("-h",arg)) about();
		ei (cs_exact("-v",arg))  _ verbose=1;
		ei (cs_exact("+v",arg))  _ verbose=0;

		ei (cs_exact("-he",arg))  aboute();
		ei (cs_exact("-ho",arg))  abouto();
		ei (cs_exact("-ec",arg))  _ filemode=PLIST_Coda_C;
		ei (cs_exact("-ea",arg))  _ filemode=PLIST_Apple;
		ei (cs_exact("-eb",arg))  _ filemode=PLIST_Binary;
		ei (cs_exact("-ej",arg))  _ filemode=PLIST_Json;

		ei (cs_exact("-e:apple",arg))  { _ filemode=PLIST_Apple; _ savemode=PLIST_NL4Leafs; _ datamode=52; }
		ei (cs_exact("-e:itunes",arg)) { _ filemode=PLIST_Apple; _ savemode=PLIST_ITUNES;   _ datamode=52; }
		ei (cs_prefix("-p:",arg)) {
			_ printlevel=cs_toInt4(arg+3);
			if (_ printlevel<0 || _ printlevel>100) _ printlevel=10;
			}
		ei (cs_prefix("-S",arg)) { _ savemode=cs_toHuge(arg+2); }
		ei (cs_prefix("-D",arg)) { _ datamode=cs_toHuge(arg+2); }

		ei (cs_exact("-pretty",arg))    _ savemode |=  JSON_Pretty;
		ei (cs_exact("+pretty",arg))    _ savemode &= ~JSON_Pretty;
		ei (cs_exact("-xhead",arg))     _ savemode |=  (PLIST_NoEncoding|PLIST_NoDoctype);
		ei (cs_exact("+xhead",arg))     _ savemode &= ~(PLIST_NoEncoding|PLIST_NoDoctype);
		ei (cs_exact("-xml1",arg))      _ savemode |=  PLIST_NL4Leafs;
		ei (cs_exact("+xml1",arg))      _ savemode &= ~PLIST_NL4Leafs;
		ei (cs_exact("-unsorted",arg))  _ savemode |=  PLIST_UnsortedDict;
		ei (cs_exact("+unsorted",arg))  _ savemode &= ~PLIST_UnsortedDict;
		ei (cs_exact("-strict",arg))    _ savemode |=  PLIST_Strict;
		ei (cs_exact("+strict",arg))    _ savemode &= ~PLIST_Strict;
		ei (cs_exact("-noes",arg))      _ savemode |=  JSON_NoEscapeSlash;
		ei (cs_exact("+noes",arg))      _ savemode &= ~JSON_NoEscapeSlash;

		ei (cs_exact("-c",arg)) {
			vi_about(0);
			codac_versions();
			printf("* Binary Plugin %sLoaded\n\n",(dynamo() ? "" : "<NOT> "));
			}
		ei (cs_prefix("+/",arg)) setitem(self,arg+1);
		ei (cs_prefix("-/",arg)) delitem(self,arg+1);
		ei (cs_exact("-w",arg))  writeplist(self,_ filename);
		ei (cs_exact("-w:-",arg)) writeplist(self,0);
		ei (cs_prefix("-w:",arg)) writeplist(self,arg+3);
		ei (cs_prefix("-f:",arg)) { freeO(_ filename); _ filename=Char_Value(arg+3); }
		ei (cs_prefix("-i",arg))  {
			printf(">>> file: %s, top: %s, mode: 0x%08x\n",_ filename,kindO(_ plist),outflags(self));
			}

		ei (cs_exact("-d",arg))  _ display=0;
		ei (cs_exact("-dn",arg)) _ display=1;
		ei (cs_exact("-dw",arg)) _ display=2;
		ei (cs_exact("-da",arg)) _ display=3;
		ei (cs_exact("-dj",arg)) _ display=4;
		ei (cs_exact("-dc",arg)) _ display=5;

		ei (cs_exact("-n",arg))  printf("\n");
		ei (cs_exact("-",arg))   loadstdin(self);
		ei (cs_exact("-t",arg))  testplist(self);
		ei (cs_exact("-q",arg))  break;

		ei (cs_prefix("-",arg)) Quit_("*** unknown argument(%s)?",arg);
		ei (cs_prefix("+",arg)) Quit_(">>> unknown argument(%s)?",arg);
		else loadfile(self,arg);
		}
	fini(self);

	freeO(_ plist); _ plist=0;
	freeO(_ filename); _ filename=0;
	freeO(_ stdinData); _ stdinData=0;

	return(0);
	}

		static Obj getref(char *str,Obj root);

	static Obj makeobject(Self self,char *str) {
		Obj obj=0; char *key=0;
		if (cs_exact("array",str)) obj=newO(Array);
		ei (cs_exact("dict",str)) obj=newO(Dictionary);
		ei (cs_exact("true",str)) obj=Bool_Value(1);
		ei (cs_exact("false",str)) obj=Bool_Value(0);
		ei (cs_prefix((key="string:"),str)) obj=Char_Value(str+cs_length(key));
		ei (cs_prefix((key="integer:"),str)) {
			obj=obj_FromString(Class_Huge,str+cs_length(key));
			if (!obj) Quit_("Bad Huge value: %s",OError());
			}
		ei (cs_prefix((key="int:"),str)) {
			obj=obj_FromString(Class_Huge,str+cs_length(key));
			if (!obj) Quit_("bad Huge value: %s",OError());
			}
		ei (cs_prefix((key="uid:"),str)) {
			obj=obj_FromString(Class_HugeUID,str+cs_length(key));
			if (!obj) Quit_("bad HugeUID value: %s",OError());
			}
		ei (cs_prefix((key="date:"),str)) {
			obj=obj_FromString(Class_DateString,str+cs_length(key));
			if (!obj) Quit_("bad DateString value: %s",OError());
			}
		ei (cs_prefix((key="real:"),str)) {
			obj=obj_FromString(Class_Real,str+cs_length(key));
			if (!obj) Quit_("bad Real value: %s",OError());
			}
		ei (cs_prefix((key="data:"),str)) {
			obj=obj_FromString(Class_Data,str+cs_length(key));
			if (!obj) Quit_("bad Data: %s",OError());
			}
		ei (cs_prefix((key="datafile:"),str)) {
			char *path=str+cs_length(key);
			obj=Data_FromFile(path);
			if (!obj) Quit_("can't load file(%s): %s",path,OError());
			}

		ei (cs_exact("null",str)) obj=0;
		ei (cs_exact("jsonnull",str)) obj=JsonNull_Value();

		ei (cs_prefix((key="keyword:"),str)) {
			char *cp=str+cs_length(key);
			int pos=cc_pos(':',cp);
				if (pos==EOF) Quit_("*** object:keyword w/o ':' seperator for 'word'?");
				cleanO Char temp=Char_Value(cp);
				temp[pos]=0; cp=temp+pos+1;
			cleanO Obj ooo=makeobject(self,cp);
			obj=Keyword_Value(temp,ooo);
			}

		ei (cc_inString(*str,"./")) {
			cleanO Char file=Char_Value(str);
			int pos=cc_pos(':',file); char *cp=0;
				if (pos!=EOF) { file[pos]=0; cp=file+pos+1; }
			cleanO Obj pl2=Json_Load(file,outflags(self));
			if (!pl2) Quit_("*** can't load secondary file(%s) [%s].",file,OError());
			if (!cp) obj=keepO(pl2);
			  else {
				obj=getref(cp,pl2);
				if (!obj) Quit_("*** invalid file(%s) ref(%s)?",file,cp);
				keepO(obj);
				}
			}
		ei (*str==':') {
			obj=getref(str+1,_ plist);
			if (!obj) Quit_("*** invalid reference(%s)?",str+1);
			keepO(obj);
			}
		else {
			obj=Json_FromBlock(cs_length(str),str,outflags(self));
			if (!obj) Quit_("*** unknown object(%s) Error: %s",str,OError());
			}
		return(obj);
		}

	static Obj getwhere(Array list,int getobj,Obj root,char *diagpath) {
		int nel=Array_count(list);
		if (nel<=0) Quit_("*** getwhere; with empty array?");
		Obj where=root;
		for(int j=0;j<(getobj?nel:nel-1);++j) {
			Obj next=0; Char item=Array_sub(list,j);
			if (isa_(where,Dictionary)) next=Dict_sub(where,item);
			ei (isa_(where,Array)) {
				int cc=(*item);
				if (cc<'0' || cc>'9') Quit_("*** where; indexing array w/o bad value(%s)",item);
				int inx=cs_toInt4(item);
				int nnn=Array_count(where);
				if (inx<0 || inx>=nnn) Quit_("*** where; bad array index(%d).",inx);
				next=Array_sub(where,inx);
				}
			ei (isa_(where,Keyword)) {
				if (!cs_exact("0",item)) Quit_("*** where; indexing keyword bad value(%s), use: '0'",item);
				next= ((Keyword)where)->item;
				}
			if (getobj && j==nel-1) return(next);
			if (!next) Quit_("*** where; invalid path? (%s)",diagpath);
			ei (isa_(next,Dictionary)) ;
			ei (isa_(next,Array)) ;
			ei (isa_(next,Keyword)) ;
			else Quit_("*** where; path to non-container? (%s)",diagpath);
			where=next;
			}
		return(where);
		}

	static Obj getref(char *str,Obj root) {
		if (cs_exact("/",str)) return(root);
		if (*str!='/') Quit_("*** getref; bad component(%s) must start with slash.",str);
		cleanO Array list=Char_Split(str+1,"/");
		return getwhere(list,1,root,str);
		}

	static void setitem(Self self,char *str) {
		int pos=cc_pos('=',str);
			if (pos==EOF) Quit_("*** '+' command w/o '=' (%s)",str);
		cleanO Char temp=Char_Value(str);
		temp[pos]=0; char *val=temp+pos+1;
		Obj obj=makeobject(self,val);
		if (cs_exact("/",temp)) { freeO(_ plist); _ plist=obj; return; }
		if (*temp!='/') Quit_("*** setitem; bad component(%s) must start with slash.",temp);
		cleanO Array list=Char_Split(temp+1,"/");
		int nel=Array_count(list);
		Obj where=getwhere(list,0,_ plist,temp);
		Char last=Array_sub(list,nel-1);
		if (isa_(where,Dictionary)) {
			Dict_take(where,last,obj);
			}
		ei (isa_(where,Array)) {
			int nnn=Array_count(where);
			int cc=(*last);
			int inx=cs_toInt4(last);
			if (cc=='+') inx=nnn;
			ei (cc<'0' || cc>'9') Quit_("*** indexing array w/o bad value(%s)",last);
			if (inx<0 || inx>nnn) Quit_("*** bad array index(%d).",inx);
			ei (inx==nnn) Array_addObject(where,obj);
			else Array_replaceAt(where,inx,obj);
			freeO(obj);
			}
		ei (isa_(where,Keyword)) {
			if (!cs_exact("0",last)) Quit_("*** Keyword index is not '0'(%s).",last);
			Keyword kkk=where;
				keepO(obj);
				freeO(kkk->item); kkk->item=obj;
			}
		else Quit_("*** assignment ref is not a container: dict,array,keyword(%s).",temp);
		}

	static void delitem(Self self,char *str) {
		if (cs_exact("/",str)) { freeO(_ plist); _ plist=newO(Dictionary); return; }
		if (*str!='/') Quit_("*** delitem; bad component(%s) must start with slash.",str);
		cleanO Array list=Char_Split(str+1,"/");
		int nel=Array_count(list);
		Obj where=getwhere(list,0,_ plist,str);
		Char last=Array_sub(list,nel-1);
		if (isa_(where,Dictionary)) {
			if (_ verbose && !Dict_sub(where,last))
				Msg_("--- can't remove undefined key(%s)? ---",last);
			Dictionary_removeKey(where,last);
			}
		ei (isa_(where,Array)) {
			int nnn=Array_count(where);
			int cc=(*last);
			int inx=cs_toInt4(last);
			if (cc<'0' || cc>'9') Quit_("*** delitem; indexing array w/o bad value(%s)",last);
			if (inx<0 || inx>=nnn) Quit_("*** delitem; bad array index(%d).",inx);
			Array_removeAt(where,inx);
			}
		ei (isa_(where,Keyword)) {
			Quit_("*** you can't remove Keyword's item, just remove the Keyword.");
			}
		else Quit_("*** referencing non-container: dict,array,keyword(%s)?",str);
		}

	void Version_register(char *key,char *text);

static __attribute__((constructor)) void codalist_boot() {
	Version_register( "codalist",
	Os(QVersion ", " QWebsite ", Copyright (c) " QCopyYears "  Stephen M. Jones, Affero GPL 3."
	"\000"));
	}

#if !WIN32

	#include <dlfcn.h>

static pointer dynamo1(char *name,int major) {
	char path[128];
	char dotmaj[64]; sprintf(dotmaj,".%d",major); if (!major) *dotmaj=0;
	#if __APPLE__
	snprintf(path,sizeof(path),"lib%s%s.dylib",name,dotmaj);
	#else
	snprintf(path,sizeof(path),"lib%s.so%s",name,dotmaj);
	#endif
	return dlopen(path,RTLD_NOW|RTLD_GLOBAL);
	}

pointer dynamo() {
	static pointer lib=0;
	if (!lib) {
		static char name[]="coda-c_blist";
				  lib=dynamo1(name,1);
		if (!lib) lib=dynamo1(name,0);
		}
	return(lib);
	}

#else
	pointer dynamo() { return(0); }
#endif

	static void indentos(FILE *os,int indent) {
		int j; for(j=0;j<indent && j<20;++j) { fprintf(os,"\t"); }
		}

	static void printLeaf(void* obj,FILE *os) {
		if (!obj) { fprintf(os,"(Null)\n"); return; }
		char *kind=obj_xmlTag(obj);
		cleanO Char aa=(kind ? ToContainer(obj) : Os("(JsonNull)"));
			if (!kind) kind=Os("Void");
		char *quots=""; if (cs_exact("string",kind)) quots="\"";
		int maxlen=60;
		if (cs_length(aa)<=maxlen)
			 fprintf(os,"%s%s%s\n",quots,aa,quots);
		else fprintf(os,"%s%.*s...%s\n",quots,maxlen,aa,quots);
		}

		 Char Dictionary_get_name(Dictionary self);
		 Char Array_get_name(Array self);
	static void plistPrint1(void* container,FILE *os,int indent,int maxlevel) ;
	static void* printItem(void *obj,FILE *os,int isArray,int indent,int maxlevel) {
		if (isa_(obj,Dictionary)) {
			Char name=Dictionary_get_name(obj); if (!name) name="";
			fprintf(os,"<%s[%d]>%s\n","dict",Dictionary_get_count(obj),name);
			if (indent+1<maxlevel) plistPrint1(obj,os,indent+1,maxlevel);
			}
		ei (isa_(obj,Array)) {
			Char name=Array_get_name(obj); if (!name) name="";
			if (Array_isaSet(obj)) name="CF$Set";
			fprintf(os,"<%s[%d]>%s\n","array",Array_get_count(obj),name);
			if (indent+1<maxlevel) plistPrint1(obj,os,indent+1,maxlevel);
			}
		else printLeaf(obj,os);
		return("OK");
		}

	static void plistPrint1(void* container,FILE *os,int indent,int maxlevel) {
		int isDict =isa_(container,Dictionary);
		int isArray=isa_(container,Array);
		if (!isDict && !isArray) { indentos(os,indent); printLeaf(container,os); return; }

		if (isArray) {
			int nel=Array_count(container);
			for(int j=0;j<nel;++j) {
				indentos(os,indent);
				void *obj=Array_subInt(container,j);
				printItem(obj,os,isArray,indent,maxlevel);
				}
			}
		else	{
			int nel=Dictionary_count(container);
			cleanO Pointer vector=Dictionary_AllKeys(container);
				assert(nel==Pointer_count(vector));
			pointer_sort(vector,nel,strcmp,0);
			for(int j=0;j<nel;++j) { char *cp=vector[j];
				indentos(os,indent);
				fprintf(os,"%s=",cp);
				Obj obj=Dictionary_subKey(container,cp); assert(obj);
				printItem(obj,os,isArray,indent,maxlevel);
				}
			}
		}

void PList_Diag2Level(Obj list,FILE *os,int maxlevel) {
	plistPrint1(list,os,0,maxlevel);
	}

	int FileMem_oWrite(FileMem self,pointer buf,int length);

Data stdin_read() {

	cleanO FileMem fff=FileMem_Open(0); assert(fff);

	while(1) {
		char buffer[4096];
		int ret=file_read(stdin,buffer,sizeof(buffer));
		if (ret<1) break;
		FileMem_oWrite(fff,buffer,ret);
		}

	Data data=FileMem_ToData(fff); assert(data);

	return(data);
	}

