gameFiles_dir = $(prefix)/@PACKAGE@/
gameFiles__DATA= preferences.ini

iconFile_dir = $(prefix)/@PACKAGE@/data/img/
iconFile__DATA = data/img/icon.png

SUBDIRS = data/img data/levels src po
DIST_SUBDIRS = $(SUBDIRS)

uninstall-hook:
	-rmdir ${gameFiles_dir}data/levels
	-rmdir ${gameFiles_dir}data/img/original
	-rmdir ${gameFiles_dir}data/img
	-rmdir ${gameFiles_dir}data
	-rmdir ${gameFiles_dir}

