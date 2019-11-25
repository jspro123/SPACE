#pragma once
#include <string>
#include <vector>
#include "Sprite.hpp"
#include "Sound.hpp"


enum itemID {lightSwitch, toolbox, brokenGlass, commanderBody, genericBody, cryoToHallway, playerPod, emptyPod, shipRail,
			DoorOneToCryo, DoorTwoToControl, DoorThreeToHallwayTwo, DoorThreePanel, controlToHallway, controlScreen, 
			controlBody, controlTerminal, controlControls, crowbar, controlLight, commanderFinger, killerFinger,
			keyCard, redStone, hammer, spaceTape};
enum locationID {Cabin, Hallway1, Control, Hallway2};

enum soundID {none, SfingerOne, SfingerTwo, Semergency, Sambience, Sdoor_air, Sdoor_open, ScrowbarOne, ScrowbarTwo, SshipCrash, Sforced};

#define NUM_SOUNDS 10

int sid_to_i(soundID);

struct Interactable {

	itemID id;
	glm::vec2 position_min;
	glm::vec2 position_max;
	std::string name; 
	bool carryable = false;
	bool in_inventory = false; 

	std::vector<std::string> interacts_with;
	std::string default_message = "Failure.";
	std::string description; //Inventory


	Interactable(itemID id_, std::string name_, bool carryable_, std::string description_) {
		id = id_;
		name = name_;
		carryable = carryable_;
		description = description_;
	} 
};

struct Inventory { 

	int max_display = 3;
	int current_page = 1;
	enum {DISPLAY, FOCUS} state = DISPLAY;
	int focused_on = 0;
	std::vector<Interactable> interactables;
	std::string default_message = "I'm not carrying anything on me right now.";
	std::vector<std::string> to_output;

	void erase_item(itemID id);
	int return_index(itemID id);
	void update_inventory();
};