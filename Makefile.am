gameFilesdir = $(prefix)/games/@PACKAGE@/
gameFiles_DATA= preferences.ini

iconFiledir = $(prefix)/games/@PACKAGE@/data/img/
iconFile_DATA = data/img/icon.png


desktopdir = $(datadir)/applications
desktop_DATA = data/@PACKAGE@.desktop

SUBDIRS = data/img data/levels src po
DIST_SUBDIRS = $(SUBDIRS)

install-data-hook:
	sed -i "/^Exec=/s#exe_path#${gameFilesdir}@PACKAGE@#" ${desktopdir}/@PACKAGE@.desktop
	sed -i "/^Icon=/s#icon_path#${iconFiledir}icon.png#" ${desktopdir}/@PACKAGE@.desktop

uninstall-hook:
	-rmdir ${gameFilesdir}data/levels
	-rmdir ${gameFilesdir}data/img/original
	-rmdir ${gameFilesdir}data/img
	-rmdir ${gameFilesdir}data
	-rmdir ${gameFilesdir}

