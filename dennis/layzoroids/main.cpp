/*
SAMPLE ASTEROIDS GAME
Developed by Chris DeLeon
November 2009
Created as material for use on http://gamedevlessons.com/
To be included in Free Lessons Vol. 9 - find all past editions at:
  http://gamedevlessons.com/?page=free

This code is written with clarity and understandability to a beginner in mind.
It is not intended to represent the most optimized or most organized way
that the code could possibly be prepared.

Allegro is required to compile this application. Find instructions here:
http://gamedevlessons.com/lessons/letter5.html
(All software needed to compile this, for Mac/PC/Linux, is free!)

Files that are part of this project should be:
  core.cpp
  core.h
  main.cpp

Feel free to make changes, experiment, and distribute. Please leave this
comment at the top of the file, since it points back to my instructional
resources. My mission is to help people - your help in this is appreciated!
*/

/*
CONTROLS:
  SPACEBAR - Fire
  Left/Right Arrow Keys - Turn ship
  Up Arrow Key - Thrust
*/

/*
Truth is, you're free to do whatever you'd like with this. The only
goal is that you learn how to do the sort of things that you want to
do, and this is provided as an example and starting point. The 
following suggested objectives are things which are deliberately
within fairly straightforward reach given the code as it is already.

My objective in offering these ideas certainly isn't to stump, just 
to guide working within this code, and getting used to how things work
in Allegro, including using Allegro's super-awesome API (look for
Allegro.txt in your docs folder) and sample/example code (look for
those within your Allegro folder)

Any questions on how to do any of this? No problem, just let me know
which, and/or share the code as it currently is, and I'm happy to help.

Assignment/Todo:
  1-Get the shot to move (carry player's momentum from moment fired!)
  2-Get the player and shot to "wrap" upon leaving the screen
  3-Add the asteroids
    A-Get asteroids spawning
    B-Get asteroids moving
    C-Get asteroids to "wrap" at screen edges
    D-Get asteroids to destroy when shot by the player
    E-Reset entire level when last asteroid is destroyed
    Bonus: Get asteroids to split/shrink when shot
  4-Add a score, display it on-screen
  5-Add an enemy UFO ship that flies on and fires periodically
  6-Modify the ship/shot drawing code so it renders a long isosceles
  7-Add colored, drifting dot particles for explosions, thrust, trails
  8-Implement lives for the player, with respawn delay between death
  9-Modify the game to work in waves/levels of increasing difficulty
  Bonus: Add a high scores table and accept player initials :)
  After: Go wild. Bitmap graphics, sounds, power-ups, starfield, etc.
*/

#include "core.h"

#define ROCK_NUM 25

struct obj_typ {
  float xv, yv; // component velocity
  float x, y; // position
  int life; // 0 means dead, -1 indefinite, positive counts down
  float ang; // current facing in radians
  float rad; // radius
};

obj_typ shot;
obj_typ ship;
obj_typ ast[ROCK_NUM];

void wipeObj(obj_typ *thisObj) {
  thisObj->life = 0;
  thisObj->x = thisObj->y = 0;
  thisObj->xv = thisObj->yv = 0.0;
  thisObj->ang = 0.0;
  thisObj->rad = 0.0;
}

void moveObj(obj_typ *thisObj) {
  if( thisObj->life == 0) {
    return;
  }
  thisObj->x += thisObj->xv;
  thisObj->y += thisObj->yv;
}

void drawLine(obj_typ *thisObj, int drawColor) {
  if( thisObj->life == 0) {
    return;
  }
  
  float lineOffsetX = cos(thisObj->ang) * thisObj->rad;
  float lineOffsetY = sin(thisObj->ang) * thisObj->rad;
  line(screenBuffer, 
          int(thisObj->x - lineOffsetX), int(thisObj->y - lineOffsetY),
          int(thisObj->x + lineOffsetX), int(thisObj->y + lineOffsetY),
          drawColor);
}

void makeScreenBlack() {
  //clear the screen to black
  //without this next line, the game "smears"
  //comment it out to try it!
  clear_to_color(screenBuffer, makecol(0,0,0));
}

void resetRocks() {
  for(int i=0;i<ROCK_NUM;i++) {
    wipeObj( &(ast[i]) );
  }
}

int numberOfRocksRemaining() {
  int numberOfRocks;
  
  numberOfRocks = 0;
  
  for(int i=0;i<ROCK_NUM;i++) {
    if( ast[i].life != 0 ) {
      numberOfRocks++;
    }
  }
  return numberOfRocks;
}

void drawThings() {
  makeScreenBlack();

  // draw rocks
  for(int i=0;i<ROCK_NUM;i++) {
    if( ast[i].life != 0 ) {
      circlefill(screenBuffer, 
            int(ast[i].x), int(ast[i].y), 
            int(ast[i].rad), 
            makecol(255, 255, 0));
    }
  }

  drawLine( &ship , makecol(255, 255, 0) );
  
  drawLine( &shot , makecol(0, 255, 0) );
}

void resetPlayer() {
  wipeObj( &(ship) );
  
  wipeObj( &(shot) );
  
  shot.rad = 3.0;
  
  ship.x = SCREEN_W/2;
  ship.y = SCREEN_H/2;
  ship.life = -1;
  ship.rad = 5.0;
}

bool doesOverlap(obj_typ *obj_1,
                obj_typ *obj_2) {
  float dx = obj_1->x - obj_2->x;
  float dy = obj_1->y - obj_2->y;
  float dist = sqrt( dx*dx+dy*dy );

  if( dist < (obj_1->rad + obj_2->rad) ) {
    return true;
  }
  return false;
}

void shoot() {
  if( shot.life == 0 ) {
    float lineOffsetX = cos(ship.ang) * ship.rad;
    float lineOffsetY = sin(ship.ang) * ship.rad;

    shot.x = ship.x + lineOffsetX;
    shot.y = ship.y + lineOffsetY;
    shot.ang = ship.ang;
    shot.life = 80;
  }
}

#define PLAYER_TURN_SPEED 0.02
#define PLAYER_THRUST_POWER 0.02

void movePlayer() {
  moveObj( &ship );

  // take input
  
  if( key[KEY_LEFT] ) {
    ship.ang -= PLAYER_TURN_SPEED;
  }

  if( key[KEY_RIGHT] ) {
    ship.ang += PLAYER_TURN_SPEED;
  }

  if( key[KEY_UP] ) {
    float thrustPower = PLAYER_THRUST_POWER;
    
    ship.xv += cos(ship.ang) * thrustPower;
    ship.yv += sin(ship.ang) * thrustPower;
  }
  
  if( key[KEY_SPACE] ) {
    shoot();
  }
  
  // player bound checking
  if(ship.x > SCREEN_W) {
    resetPlayer();
  }
  if(ship.x < 0) {
    resetPlayer();
  }

  if(ship.y > SCREEN_H) {
    resetPlayer();
  }  
  if(ship.y < 0) {
    resetPlayer();
  }
}

void moveShot() {
  if(shot.life > 0) {
    shot.life--;
  }

  // update the ball's position for the next frame
  moveObj( &shot );
  
  // shot screen edge checks
  if(shot.x > SCREEN_W) {
    shot.life = 0;
  }
  if(shot.x < 0) {
    shot.life = 0;
  }

  if(shot.y > SCREEN_H) {
    shot.life = 0;
  }  
  if(shot.y < 0) {
    shot.life = 0;
  }
}

void gameSetup() {
  resetRocks();
  resetPlayer();
}

// program code aways begins at the top of main()
int main(){
  //The next line is a function drom core.cpp
  // it sets up Allegro for graphics and keyboard
  techInit();
    
  gameSetup();
    
  while(key[KEY_ESC] == false){
    movePlayer();
    
    moveShot();
    
    drawThings();
    
    updateScreen();

    // is the game going too fast on your awesome modern hardware?
    // Experiment with numbers 0-15 here to slow down the application:
    rest(1);
    }
  return 0;
} END_OF_MAIN()// putting this line after the main() function
            // helps Allegro make your game work in Windows/MaxOSX
