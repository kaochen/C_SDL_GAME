#! /bin/sh
#!/bin/bash
file="README"
if [ -f "$file" ]
then
	echo "$file found."
else
	echo "$file not found. Create a symbolic link between README.md and README "
	ln  -s README.md README
fi
file2="config.rpath"
if [ -f "$file2" ]
then
	echo "$file2 found."
else
	echo "$file2 not found. Create an empty one"
	touch config.rpath
fi

aclocal \
&& autoheader \
&& automake --add-missing \
&& autoconf \
&& autoreconf --force --install || exit 1
