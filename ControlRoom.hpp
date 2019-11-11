#include "Interactable.hpp"

struct ControlRoom {

	ControlRoom();

	//See descriptions in the story section

	Interactable Light = Interactable(controlLight, "A light switch", false, " . ");
	Interactable Crowbar = Interactable(crowbar, "A crowbar", true, "Covered in blood. ");
	Interactable Door = Interactable(controlToHallway, "A door", false, " . ");
	Interactable Screen = Interactable(controlScreen, "The screen.", false, " . ");
	Interactable Terminal = Interactable(controlTerminal, "A terminal", false, " . ");
	Interactable Body = Interactable(controlBody, "A body", false, " . ");
	Interactable Controls = Interactable(controlControls, "Controls", false, " . ");

	std::vector<Interactable> control_interactables;

	void check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click, itemID item,
		Inventory& inventory, locationID& location);
	void check_story(std::vector<std::string>& message_box);

	//------ story state -------

	// Cabin Interactables
	// light
	struct {

		bool intro_text = false;
		bool light_on = false;
		bool light_on_text = false;
		bool tool_open = false;
		bool tool_open_text = false;

		int broken_glass_descr = 1;
		int commander_descr = 1;
		int body_descr = 1;
		int light_switch_descr = 1;
		int tool_box_descr = 1;
		int cabin_door_descr = 1;
		int protag_pod_descr = 1;
		int empty_pod_descr = 1;
		int ship_rail_descr = 1;

	} control_state;
	

	/* ============= STORY TEXT ============= */



};
