	#include "coda-c_plist.h"
	#include "coda-c_strings.h"

	void tester(Dictionary,bool) ;

int main(int argc,char **argv) {

	char *file="./testpack1.plist";
	bool extract=0;
	if (argc>=2 && cs_exact("-x",argv[1])) { extract=1; ++argv; --argc; }
	if (argc>=2) file=argv[1];
	printf("test pack file: %s\n",file);
	Dictionary dict=PList_Load(file,PLIST_Apple);
	if (!dict) {
		printf("PList Tester format: pltester [testpack.plist]  # default=testpack1.plist\n");
		printf("Error loading[%s]: %s   Test-Pack *STOP*\n",file,OError());
		exit(2);
		}
	if (!isa_(dict,Dictionary)) { printf("bad pack(%s)--top not dict? :[ %s ]\n",file,OError()); exit(3); }

	tester(dict,extract);
	return(0);
	}

void tester(Dictionary dict,bool extract) {
	int ntest=0,ngood=0,nbad=0;

	for(Keyword key=Dictionary_scan(dict);key;key=Dictionary_next(key)) {
		char *file=key->word;
		Char string=key->item;
		if (!isa_(string,Char)) { printf("test(%s) is not a Char(string)?\n",file); exit(5); }
		if (extract) {
			FILE *os=fopen(file,"wb");
			if (!os) { printf("can't write(%s)?\n",file); exit(4); }
			fprintf(os,"%s",string);
			fclose(os);
			}
		cleanO Obj obj=PList_FromBlock(sizeO(string)-1,string,PLIST_Apple);

		int shouldpass=cs_suffix(".pass",file);
		char *msg="*FAIL*"; if (obj) msg="passed";
		int correct=(shouldpass && obj) || (!shouldpass && !obj);
		++ntest;
		if (correct) ++ngood; else ++nbad;
		char *cormsg=(correct ? "Correct!" : "Oops?");
		printf("Test: %20.20s  %s  %s\n",file,msg,cormsg);
		}
	printf("\n");
	printf("Totals: %d tests, %d correct results, %d bad results\n",ntest,ngood,nbad);
	if (nbad>0) printf("*** %d ERRORS, STOP ***\n",nbad),exit(1);
	}

