#include "Interactable.hpp"

struct HallwayOne {

	HallwayOne();

	//See descriptions in the story section
	Interactable Door_1 = Interactable(DoorOneToCryo, "A door", false, "It's sharp.");
	Interactable Door_2 = Interactable(DoorTwoToControl, "A door", false, "It's sharp.");
	Interactable Door_3 = Interactable(DoorThreeToHallwayTwo, "A door", false, "It's sharp.");
	Interactable Door_3_panel = Interactable(DoorThreePanel, "A panel", false, "It's sharp.");
	std::vector<Interactable> hallwayone_interactables;

	//called to create menu for current scene:
	void check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click, itemID item,
		Inventory& inventory, locationID& location);
	void check_story(std::vector<std::string>& message_box);

	struct {

		bool door_3_open = false;
		int door_3_panel_descr = 1;

	} hallwayone_state;

	/* ============= STORY TEXT ============= */

	std::vector<std::string> Door_1_descr1;
	std::string Door_1_descr1_1 = "This door leads to the cryosleep room. ";

	std::vector<std::string> Door_2_descr1;
	std::string Door_2_descr1_1 = "This door leads to the control room. ";

	std::vector<std::string> Door_3_descr1;
	std::vector<std::string> Door_3_use_descr1;
	std::string Door_3_descr1_1 = "This door leads to the emergency exit. ";
	std::string Door_3_use_descr1_1 = "It's sealed shut. I can't get it open.";

	std::vector<std::string> Door_3_panel_descr1;
	std::vector<std::string> Door_3_panel_descr2;
	std::vector<std::string> Door_3_panel_descr3;
	std::vector<std::string> Door_3_panel_use_descr1;
	std::string Door_3_panel_descr1_1 = "The door's control panel. Let's take a look. ";
	std::string Door_3_panel_descr1_2 = "Looks like the doors been manually locked at some point. I'll need to use the terminal in the control room to open it.";
	std::string Door_3_panel_descr2_1 = "I'll need to use the terminal in the control room to open it.";
	std::string Door_3_panel_descr3_1 = "Looks like the door is open. ";
	std::string Door_3_panel_use_descr1_1 = "It's just a screen. Nothing I can do from here.";
};
