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

aclocal \
&& autoheader \
&& automake --add-missing \
&& autoconf \
&& autoreconf --force --install || exit 1
