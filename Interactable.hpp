#include <string>
#include <vector>
#include "Sprite.hpp"

struct Interactable {

	std::string name; //Unique, please
	Sprite const* associated_sprite; //Contains position info
	std::vector<std::string> descriptions;
	int current_description = 0;
	bool carryable = false;
	bool in_inventory = false; 

	std::vector<std::string> interacts_with;
	std::string default_message = "Failure.";
	std::string description;


	Interactable(std::string name_, Sprite const* associated_sprite_, bool carryable_, std::string description_) {
		name = name_;
		associated_sprite = associated_sprite_;
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

	void update_inventory();
};