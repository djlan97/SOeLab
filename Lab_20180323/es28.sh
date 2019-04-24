#!/bin/sh

for fname in *
do
	grep '#!/bin/sh' $fname >/dev/null 2>&1
	if test $? -eq 0
	then
		echo $fname
	fi

done

exit 0
