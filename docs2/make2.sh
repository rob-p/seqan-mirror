#!/bin/sh
rm -f dddoc_cache.bin
rm -rf html/*
../util/bin/dddoc.py -d pages doc2 --out-dir html2 --generator=html2 $@
exit $?
