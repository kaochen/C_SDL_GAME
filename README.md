# SokoRobot
SokoRobot is a Sokoban game. It's written in langage C using SDL2.

This is an early draft, use it carefully.

### Dependencies
```
apt install libsdl2-2.0-0 libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev libfreetype6-dev libxml2-dev

```

### Download
```
git clone https://github.com/kaochen/SokoRobot
```

### Compile
```
cd
make
./SokoRobot
```

clean objects *.o:
```
make clean
```
Clean binaries:
```
make mrproper
```

### Dev stuff

Git :
```
git remote set-url origin git@github.com:kaochen/SokoRobot.git
git status // see changes
git add -u //update all existing files
git add new_files.c //add a new file or update en existing one
git mv file.c newfile.c
git rm file.c
git commit -m"new changes"
git push //push last commit
git pull //pull last changes
```

Good pratice:
 * [FR] http://emmanuel-delahaye.developpez.com/tutoriels/c/bonnes-pratiques-codage-c/

I used the "Kernighan & Ritchie style" to indent my code.
```
apt get install indent
indent -kr file.c
```

### Links
 * http://wiki.libsdl.org/Installation#Linux.2FUnix
 * http://jeux.developpez.com/tutoriels/sdl-2/guide-migration/
 * http://www.meruvia.fr/index.php/big-tuto-sdl-1/179-big-tuto-sdl-1-2-chapitre-31-conversion-de-projet-vers-la-sdl-2
 * http://alexandre-laurent.developpez.com/tutoriels/sdl-2/afficher-sprites-sdl-2/
 * http://www.sokobano.de

