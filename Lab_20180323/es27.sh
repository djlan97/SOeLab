#!/bin/sh

for filename in *
do
	case $filename in
		*.sh) echo $filename 1>&2 ;;
		*) echo $filename ;;
	esac
done

exit 0
