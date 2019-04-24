#!/bin/sh

case $# in
	1) cat >>$1 ;;
	2) cat $1 >>$2 ;;
	*) echo "Usage: $0 [sorg] dest"
		exit 1
		;;
esac

exit 0
