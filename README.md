# C_SDL_GAME
C_SDL_Game is a simple SDL Game write to practice C

This is an early draft, use it carefully.

```
git clone https://github.com/kaochen/C_SDL_GAME
git remote add -m https://github.com/kaochen/C_SDL_GAME.git
git remote set-url origin git@github.com:kaochen/C_SDL_GAME.git
```

### Compile
```
sudo apt install libsdl2-2.0-0 libsdl2-dev libsdl2-image-dev
gcc -o game -g *.c *.h `sdl2-config --cflags --libs`-lSDL2main -lSDL2_image -lSDL2

./game
```

### Links
 * http://wiki.libsdl.org/Installation#Linux.2FUnix
 * http://jeux.developpez.com/tutoriels/sdl-2/guide-migration/
 * http://www.meruvia.fr/index.php/big-tuto-sdl-1/179-big-tuto-sdl-1-2-chapitre-31-conversion-de-projet-vers-la-sdl-2
 * http://alexandre-laurent.developpez.com/tutoriels/sdl-2/afficher-sprites-sdl-2/

### Color code :
```
127,34,0 (7F2200);
255,124,76 (FF7C4C);
255,67,0 (FF4300); background
127,36,4 (7F2404);
204,54,0 (CC3600);
```
