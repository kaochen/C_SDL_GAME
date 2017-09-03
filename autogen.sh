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

#erase pot file in order to force automake to rebuild it
#potFile="po/sokorobot.pot"
#if [ -f "$potFile" ]
#then
#	echo "$potFile found."
#	rm $potFile
#fi
aclocal \
&& autoheader \
&& automake --add-missing \
&& autoconf \
&& autoreconf --force --install || exit 1
