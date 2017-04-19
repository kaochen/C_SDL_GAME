gameFilesdir = $(prefix)/games/@PACKAGE@/
gameFiles_DATA= preferences.ini

iconFiledir = $(prefix)/games/@PACKAGE@/data/img/
iconFile_DATA = data/img/icon.png

desktopdir = $(datadir)/applications
desktop_in_files = sokorobot.desktop.in
desktop_DATA = data/$(desktop_in_files:.desktop.in=.desktop)

SUBDIRS = data/img data/levels src po
DIST_SUBDIRS = $(SUBDIRS)

uninstall-hook:
	-rmdir ${gameFiles_dir}data/levels
	-rmdir ${gameFiles_dir}data/img/original
	-rmdir ${gameFiles_dir}data/img
	-rmdir ${gameFiles_dir}data
	-rmdir ${gameFiles_dir}

