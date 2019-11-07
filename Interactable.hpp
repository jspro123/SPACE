#include <string>
#include <vector>
#include "Sprite.hpp"


enum itemID {lightSwitch, toolbox, brokenGlass, commanderBody, genericBody, cryoDoor};

struct Interactable {

	itemID id;
	glm::vec2 position_min;
	glm::vec2 position_max;
	std::string name; 
	std::vector<std::string> descriptions;
	int current_description = 0;
	bool carryable = false;
	bool in_inventory = false; 

	std::vector<std::string> interacts_with;
	std::string default_message = "Failure.";
	std::string description;


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

	void update_inventory();
};