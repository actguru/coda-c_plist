# codalist & plist lib tester.  08/03/2024

build=0
# build=1 # make ans files

codalist="codalist"
if test -f "../codalistdos"
	then codalist="../codalistdos"
	fi

echo "codalist is $codalist"

if test -f new.clist
	then rm new.clist
	fi

testfile() {
	local f="$1"
	local file="$1"
	echo "---file: $f"
	cat "$f"
	case "$f" in
		*.args)
			args=$(cat $f)
			file="args.temp" # XXX create temp file
			if $codalist $args -ej -w:- -n > $file 2>&1
				then : # echo good; exit
				else echo bad see: args.temp 1>&2; exit 3
				fi
			;;
		esac
	echo "---json"
	$codalist "$file" -ej -w:- -n 2>&1
	echo "---xml"
	$codalist "$file" -xhead -ea -w:- -n 2>&1
	# $codalist "$file" -xhead -ec -w:- -n 2>&1
	echo "---json-pretty"
	$codalist "$file" -noes -pretty -ej -w:- 2>&1
	echo "---end"
	case "$file" in
		args.temp) rm "$file";; ## XXX comment to prevent delete
		esac
	}

checkfile() { # just use "$f", no args
	testfile $f > $f.out
	let ++ntest
	if test -f $f.ans
		then 
			if cmp -s $f.out $f.ans
				then echo "--- Passed Testing: $f"; let ++npass
					rm "$f.out"; # exact match
				else echo "*** FAILED TESTING: $f"; let ++nfail
				fi
		else
			case "$build" in
				1) echo "Storing answer for: $f ..."
					echo cp $f.out $f.ans
					cp $f.out $f.ans
					let ++nnew
					;;
				*) echo "No correct result stored for: $f *** QUIT ***"; exit 2 ;;
				esac
		fi
	}

ntest=0; npass=0; nfail=0; nnew=0;

for f in *.args *.json *.json5 *.xml
	do
	checkfile # $f
	done

echo "# of tests: $ntest (new=$nnew)"
echo "# FAILED..: $nfail"
echo "# passed..: $npass"
## exit 1 # fail testing
case "$nfail" in
	0) exit 0;;
	*) exit 1;;
	esac

