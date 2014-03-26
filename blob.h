/*
Kevin Epp  CSE 20212
Lab9  blob.h
This program contains the functions that are used in main.cpp, the Blob class
definition, and the Blob class implementation.
It displays a dot on an SDL screen and performs various functions so that
Mortal Kombat can be implemented using this class and functions
*/

#include <iostream>
#include "SDL/SDL.h"
#include <string>

using namespace std;

//GLOBAL VARIABLES

//Screen dimensions and bits-per-pixel
const int screen_height = 849;
const int screen_width = 1200;
const int screen_bpp = 32;

SDL_Surface *background; //Background image, a Mortal Kombat landscape
SDL_Surface *screen; //The blank screen

SDL_Event event; //Used to check what events happen at the keyboard

//FUNCTIONS

//Load the image inputted to the function so it can be displayed later
SDL_Surface *load_image(string filename){
	SDL_Surface *loaded; //Loaded image
	SDL_Surface *optimized; //Pixel-optimized image
	
	loaded = SDL_LoadBMP(filename.c_str()); //Load the image

	optimized = SDL_DisplayFormat(loaded); //Optimize the image
	SDL_FreeSurface(loaded); //Free the loaded image to avoid memory leak

	//Change the background color for the loaded image so it doesn't show up
	Uint32 colorkey = SDL_MapRGB( optimized->format, 255, 255, 255 );
	SDL_SetColorKey( optimized, SDL_SRCCOLORKEY, colorkey );

	//Return optimized image
	return optimized;
}

//Apply a loaded image to the screen at a certain place
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination){
	SDL_Rect offset;

	//Coordinates of where to place loaded image
	offset.x = x; 
	offset.y = y;
	
	//Display image to desired destination (most likely screen) at coordinates
	SDL_BlitSurface(source, NULL, destination, &offset);
}

//Initialize the screen for SDL use
bool init(){
	//Initialize everything, return false if failed
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		return false;
	}

	//Open up the screen with desired dimensions
	screen = SDL_SetVideoMode(screen_width, screen_height, screen_bpp, SDL_SWSURFACE);

	//Return if screen cannot open
	if(screen == NULL){
		return false;
	}

	SDL_WM_SetCaption("Blob", NULL); //Set screen's title

	return true;
}

//Draws a filled-in rectangle on the screen
//This rectangle will be used to keep track of character's health
//Green bar = available health
//Dark red bar = health lost
//Code drawn from http://pbeblog.wordpress.com/2009/06/24/drawing-rectangles-sdl/
void FillRect(int x, int y, int w, int h, int color) {
	SDL_Rect health = {x,y,w,h}; // make a rectangle at (x,y) of length l and width w
	//Blit that rectangle onto the background filled in with color 
	SDL_FillRect(background, &health, color); 
}

//BLOB CLASS DEFINITION

class Blob{
	public:
		Blob(int = 400, int = 450); //Constructor
		void move1(Blob &); //Make the desired move according to key pressed (Character 1)
		void move2(Blob &); //Make the desired move according to key pressed (Character 2)	
		void show(SDL_Surface*); //Show the dot on desired surface
		void free(); //Free the dot's space in memory
		void show_health(int, int); //Display the current character's health bar using FillRect
		void punch(Blob &); //Punch the opposing character, if close enough
		void kick(Blob &); //Kick the opposing character, if close enough
		int isDone(); //Check to see if the game is over

	private:
		int x; //x-coordinate
		int y; //y-coordinate
		int health; //Current health remaining of the character (out of 100)
		SDL_Surface *image; //Image of the dot
};

//BLOB CLASS IMPLEMENTATION

//Initialize dot
Blob::Blob(int xcoord, int ycoord){
	x = xcoord; //x-coordinate
	y = ycoord; //y-coordinate
	image = load_image("dot.bmp"); //Load image of the dot
	health = 100; //Initialize character health to 100
}

//Make a desired move for Character 1
void Blob::move1(Blob &otherDot){
	if(event.type == SDL_KEYDOWN){ //If key was pressed
		switch(event.key.keysym.sym){
			case SDLK_s: //If s was pressed
				if(x < screen_width - 20)
					x = x + 10; //move dot to right
				break;
			case SDLK_a: //If a was pressed
				if(x > 0)
					x = x - 10; //move dot to left
				break;
			case SDLK_q: //If q was pressed
				punch(otherDot); //Invoke punch function to punch other character
				break;
			case SDLK_w: //If w was pressed
				kick(otherDot); //Invoke kick function to kick other character
				break;
		}
	}
}

//Make a desired move for Character 2
void Blob::move2(Blob &otherDot){
	if(event.type == SDL_KEYDOWN){ //If key was pressed
		switch(event.key.keysym.sym){
			case SDLK_l: //If l was pressed
				if(x < screen_width - 20)
					x = x + 10; //move dot to right
				break;
			case SDLK_k: //If k was pressed
				if(x > 0)
					x = x - 10; //move dot to left
				break;
			case SDLK_i: //If i was pressed
				punch(otherDot); //Invoke punch function to punch other character
				break;
			case SDLK_o: //If o was pressed
				kick(otherDot); //Invoke kick function to kick other character
				break;
		}
	}
}

//Display the health bar for the character
//Avaible health is displayed as green while depleted health is dark red
void Blob::show_health(int x, int y){
	//Draw a rectangle whose length is 4 times the character's health (for better visability)
	//and width is 30 at (x,y) with a light green color (HEX: 1FEB11)
	FillRect(x, y, health*4, 30, 0x1FEB11); 
	
	if(health < 100){ //If character is not at full health

		//Draw the remainder of the rectangle with a dark red color (HEX: 660000)
		FillRect(x + health*4, y, (400 - (health*4)), 30, 0x660000);
	}
}

//Punch the other character and deplete their health if no more than 50 pixels away
void Blob::punch(Blob &otherDot){
	if((x >= (otherDot.x - 50)) && (x <= (otherDot.x + 50))){ //If the character is within 50 pixels

		if(otherDot.health > 0){ //If the other character still has health left

			if(otherDot.health <= 10){ //If the other character cannot take a full punch
				otherDot.health = 0; //Set other character's health to 0
			}else{ //Other character can take a full punch
				otherDot.health = otherDot.health - 10; //Take 10 health away from other character
			}
		}
	}
}

//Kick the other character and deplete their health if no more than 30 pixels away
void Blob::kick(Blob &otherDot){
	if((x >= (otherDot.x - 30)) && (x <= (otherDot.x + 30))){ //If the character is within 30 pixels

		if(otherDot.health > 0){ //If the other character still has health left

			if(otherDot.health <= 20){ //If the other character cannot take a full kick
				otherDot.health = 0; //Set the other character's health to 0
			}else{ //Other character can take a full kick
				otherDot.health = otherDot.health - 20; //Take 20 health away from other character
			}
		}
	}
}

//Display the dot
void Blob::show(SDL_Surface* destination){
	apply_surface(x,y,image, destination);
}

//Free the dot in memory
void Blob::free(){
	SDL_FreeSurface(image);
}

//Check if game is over (i.e someone has run out of health)
int Blob::isDone(){
	if(health <= 0){ //If character's health is depleted
		return 1; //Game is over
	}
	return 0; //Otherwise, game is not over
}
