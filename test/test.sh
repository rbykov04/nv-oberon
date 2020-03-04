#!/bin/bash
for filename in *.ob0; do
	echo "[TEST     ]"$filename
	../occ $filename>.test_result
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
	fi;
done
rm .test_result || true

