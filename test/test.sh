#!/bin/bash
state=true
for filename in *.ob0; do
	echo "[TEST     ]"$filename
	../occ -t $filename>.test_result
	if ! test -f $filename.etalon;
	then
		cp .test_result $filename.etalon;
	fi;
	if cmp -s .test_result $filename.etalon; 
	then
		echo '[    GOOD ]'; 
	else 
		diff .test_result $filename.etalon; 
	 	echo '[     BAD ]'; 
		state=false
	fi;
done
rm .test_result || true
if [ "$state" = false ] ; then
	exit 1;
fi

