#include "Interactable.hpp"

struct ControlRoom {

	ControlRoom();

	//See descriptions in the story section

	Interactable Light = Interactable(controlLight, "A light switch", false, " . ");
	Interactable Crowbar = Interactable(crowbar, "A crowbar", true, "It's covered in blood. ");
	Interactable Door = Interactable(controlToHallway, "A door", false, " . ");
	Interactable Screen = Interactable(controlScreen, "The screen", false, " . ");
	Interactable Terminal = Interactable(controlTerminal, "A terminal", false, " . ");
	Interactable Body = Interactable(controlBody, "A body", false, " . ");
	Interactable Controls = Interactable(controlControls, "Controls", false, " . ");
	Interactable Killer_finger = Interactable(killerFinger, "A finger", true, "It looks a bit off.");
	Interactable Key_card = Interactable(keyCard, "A key-card", true, "I can use this to access the terminal.");
	Interactable CCU = Interactable(computerChip, "A CCU chip. ", true, "It's used for communications and navigation. ");

	std::vector<Interactable> control_interactables;

	bool check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click, itemID item,
		Inventory& inventory, locationID& location);
	void check_story(std::vector<std::string>& message_box);

	//------ story state -------

	// Cabin Interactables
	// light
	struct {

		bool intro_text = false;
		bool used_key_card = false;
		bool commander_bio = false;
		bool diary_bio = false;
		bool picked_up_crowbar = false;
		bool accessed_terminal = false;
		bool took_finger = false;

		int crowbar_descr = 1;
		int screen_descr = 1;
		int controls_descr = 1;
		int terminal_descr = 1;
		int use_terminal_descr = 1;
		int body_descr = 1;
		int interact_body_descr = 1;

	} control_state;
	

	/* ============= STORY TEXT ============= */

	/* ============= CONTROL - INTRO ============= */

	std::string control_intro1 = ". . . ";
	std::string control_intro2 = "Just me left. . . ";

	/* ============= CONTROL ROOM =============*/

	std::vector<std::string> Light_descr1;
	std::vector<std::string> Light_use_descr1;
	std::string Light_descr1_1 = "Just a light switch.";
	std::string Light_use_descr1_1 = "I'd rather not. . . ";

	std::vector<std::string> Crowbar_descr1;
	std::vector<std::string> Crowbar_descr2;
	std::vector<std::string> Crowbar_use_descr1;
	std::string Crowbar_descr1_1 = "A crowbar. It's covered in blood. ";
	std::string Crowbar_descr1_2 = "So this is what killed. . .";
	std::string Crowbar_descr2_1 = "A crowbar. It's covered in blood. ";
	std::string Crowbar_use_descr1_1 = ". . . I should take this with me.";

	std::vector<std::string> Door_descr1;
	std::string Door_descr1_1 = "This leads back to the hallway.";

	std::vector<std::string> Screen_descr1;
	std::vector<std::string> Screen_descr2;
	std::vector<std::string> Screen_use_descr1;
	std::string Screen_descr1_1 = "The navigation screen. It's covered in blood. "; 
	std::string Screen_descr1_2 = ". . .  ";
	std::string Screen_descr1_3 = "Looks like some debris smashed into the left side of the ship. Made his head smash against the screen. ";
	std::string Screen_descr1_4 = ". . . where were you going? ";
	std::string Screen_descr2_1 = "The navigation screen. It's covered in blood. ";
	std::string Screen_use_descr1_1 = "It's just a screen. Not sure how I'd use it. ";

	std::vector<std::string> Controls_descr1;
	std::vector<std::string> Controls_descr2;
	std::vector<std::string> Controls_use_descr1;
	std::string Controls_descr1_1 = "The ship's controls. The auto-pilot is off. ";
	std::string Controls_descr1_2 = "Let me see. . . at our current heading, we were going to reach. . . ";
	std::string Controls_descr1_3 = "What? He tried taking us back? ";
	std::string Controls_descr2_1 = "The ship's controls. The auto-pilot is off. ";
	std::string Controls_use_descr1_1 = "With the condition the ship is in, I don't think that'll do anything good. ";

	std::vector<std::string> Terminal_descr1;
	std::vector<std::string> Terminal_descr2;
	std::vector<std::string> Terminal_descr3;
	std::vector<std::string> Terminal_use_descr1;
	std::vector<std::string> Terminal_use_descr2;
	std::string Terminal_descr1_1 = "The ship's computer terminal. Looks like it's still working. ";
	std::string Terminal_descr1_2 = ". . . 2039. So it's been two years since we started heading back. ";
	std::string Terminal_descr2_1 = "The ship's computer terminal. Looks like it's still working. ";
	std::string Terminal_descr3_1 = "Smashed to pieces. . . (even though you can't see it.)";
	std::string Terminal_use_descr1_1 = "I need to use a key-card to gain access. ";
	std::string Terminal_use_descr2_1 = "It's broken beyond repair. No way. ";

	std::vector<std::string> Body_descr1;
	std::vector<std::string> Body_descr2;
	std::vector<std::string> Body_use_descr1;
	std::vector<std::string> Body_use_descr2;
	std::string Body_descr1_1 = "(spits)";
	std::string Body_descr1_2 = ". . . ";
	std::string Body_descr1_3 = "Why didn't you kill me?";
	std::string Body_descr2_1 = ". . . ";
	std::string Body_use_descr1_1 = "He's holding something. . . looks like the terminal key-card. I should take it with me. ";
	std::string Body_use_descr2_1 = ". . . nothing else here. ";
};
