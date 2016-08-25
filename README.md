# SokoRobot
SokoRobot is a Sokoban game. It's written in langage C using SDL2.

![SokoRobor_ScreenShot](./src/img/SokoRobot-ScreenShot.png)

This is an early draft, use it carefully.
### Shortcuts
* m: menu
* n: next level
* p: previous level
* r: reset current level
* q: quit

### Dependencies
```
apt install build-essential git
apt install libsdl2-2.0-0 libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev libfreetype6-dev libxml2-dev libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-gfx-dev fonts-roboto

```

### Download
```
git clone https://github.com/kaochen/SokoRobot
cd downloadfolder/SokoRobot
```
If you have already the project and you want to synchonize from the server :
```
git pull
```

### Compile
```
make
```

### Play
```
./SokoRobot
```

### Clean install
clean objects *.o:
```
make clean
```
Clean binaries:
```
make mrproper
```

### Dev stuff

![X and Y](https://upload.wikimedia.org/wikipedia/commons/8/86/Rep%C3%A8re_SDL_Window.png)

Git :
```
git remote set-url origin git@github.com:kaochen/SokoRobot.git
git status // see changes
git diff //see changes as a patch
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

I used the "GNU Style" to indent my code.
```
apt get install indent
indent -gnu file.c
```

### Links
 * http://wiki.libsdl.org/Installation#Linux.2FUnix
 * http://jeux.developpez.com/tutoriels/sdl-2/guide-migration/
 * http://www.meruvia.fr/index.php/big-tuto-sdl-1/179-big-tuto-sdl-1-2-chapitre-31-conversion-de-projet-vers-la-sdl-2
 * http://alexandre-laurent.developpez.com/tutoriels/sdl-2/afficher-sprites-sdl-2/
 * http://www.sokobano.de

