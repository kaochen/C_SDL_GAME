# SokoRobot
SokoRobot is a Sokoban game. It's written in langage C using SDL2.

This is an early draft, use it carefully.

```
git clone https://github.com/kaochen/SokoRobot
git remote add -m https://github.com/kaochen/SokoRobot.git
git remote set-url origin git@github.com:kaochen/SokoRobot.git
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
Good pratice:
 * [FR] http://emmanuel-delahaye.developpez.com/tutoriels/c/bonnes-pratiques-codage-c/
```
apt get install indent
indent -bli0 -npsl -i3 -ts0 -sob file.c
```


### Links
 * http://wiki.libsdl.org/Installation#Linux.2FUnix
 * http://jeux.developpez.com/tutoriels/sdl-2/guide-migration/
 * http://www.meruvia.fr/index.php/big-tuto-sdl-1/179-big-tuto-sdl-1-2-chapitre-31-conversion-de-projet-vers-la-sdl-2
 * http://alexandre-laurent.developpez.com/tutoriels/sdl-2/afficher-sprites-sdl-2/
 * http://www.sokobano.de

