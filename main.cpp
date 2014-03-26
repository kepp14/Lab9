/*
Kevin Epp  CSE 20212
Lab 9  main.cpp
Main function that implements two instances of the Blob class as characters and plays a mock Mortal Kombat game
The two Blobs can move independently and punch and kick each other to deplete health
*/

#include <iostream>
#include "SDL/SDL.h"
#include "blob.h"
#include <string>

int main(int argc, char* args[]){
	int choice; //Which background the user chooses to play on
	string backChoice; //String that contains the image location of the desired background
	bool quit = false; //Checks if user quit

	//Check which background the user would like to play on
	cout << "Enter 1 to play on MK ground, 2 to play at ND stadium, or 3 to play at the Golden Dome." << endl;
	cin >> choice;

	//Assign appropriate image location to backChoice depending on user input
	switch(choice){
		case 1: //Mortal Kombat background
			backChoice = "images.bmp"; 
			break;
		case 2: //Notre Dame Stadium background
			backChoice = "stadium.bmp";
			break;
		case 3: //Golden Dome background
			backChoice = "dome.bmp";
			break;
		default: //Invalid input assumes Mortal Kombat background
			cout << "Wrong input. Assuming you want to play on MK ground." << endl;
			backChoice = "images.bmp";
			sleep(2);
			break;
	}

	//If SDL couldn't intialize, quit
	if(init() == false){
		return 1;
	}

	Blob myDot1(400,800); //Initialize character 1
	Blob myDot2(800,800); //Inititalize character 2

	while(quit == false){ //While user has not exited window
		background = load_image(backChoice); //load background image
		while(SDL_PollEvent(&event)){ //While there is an event happening
			myDot1.move1(myDot2); //Check if appropriate key was pressed and make desired move accordingly
			myDot2.move2(myDot1); //Check if appropriate key was pressed and make desired move accordingly
			if(event.type == SDL_QUIT){ //If user exited window
				quit = true; //Quit
			}
		}
		myDot1.show_health(50, 50); //Display health bar for character 1
		myDot2.show_health(750, 50); //Display health bar for character 2
		apply_surface(0,0,background,screen); //Display background on the screen
		myDot1.show(screen); //Display character 1 on the screen over background
		myDot2.show(screen); //Display character 2 on the screen over background

		//If screen could not be shown, quit
		if(SDL_Flip(screen) == -1){
			return 1;
		}

		SDL_FreeSurface(background); //Free the background

		//If Character 1 is out of health
		if(myDot1.isDone() == 1){
			quit = true; //Exit game after loop
			cout << "Player 2 wins!!!" << endl; //Display Character 2 winning message
		}

		//If Character 2 is out of health
		if(myDot2.isDone() == 1){
			quit = true; //Exit game after loop
			cout << "Player 1 wins!!!" << endl; //Display Character 1 winning message
		}	
	}
		
	myDot1.free(); //Free character 1
	myDot2.free(); //Free character 2
	SDL_Quit(); //Free screen and quit SDL
	
	return 0;
}
