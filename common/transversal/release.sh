#!/bin/sh
if [ ! -f target/main.pdf ]; then
	echo 'Run build.sh first.'
fi
mv target/main.pdf target/projet_transversal.pdf

