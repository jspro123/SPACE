#include "ControlRoom.hpp"


ControlRoom::ControlRoom() {

	Crowbar.position_min = glm::vec2(1200, 600);
	Crowbar.position_max = glm::vec2(1360, 720);

	Door.position_min = glm::vec2(40, 470);
	Door.position_max = glm::vec2(305 ,1000);

	Light.position_min = glm::vec2(318, 606);
	Light.position_max = glm::vec2(360, 720);

	Screen.position_min = glm::vec2(430, 160);
	Screen.position_max = glm::vec2(1770, 575);

	Terminal.position_min = glm::vec2(940, 645);
	Terminal.position_max = glm::vec2(1157, 895);

	Body.position_min = glm::vec2(1465, 600);
	Body.position_max = glm::vec2(1660, 900);

	Controls.position_min = glm::vec2(435, 795);
	Controls.position_max = glm::vec2(920, 900);

	Light_descr1.push_back(Light_descr1_1);
	Light_use_descr1.push_back(Light_use_descr1_1);

	Crowbar_descr1.push_back(Crowbar_descr1_1);
	Crowbar_descr1.push_back(Crowbar_descr1_2);
	Crowbar_descr2.push_back(Crowbar_descr2_1);
	Crowbar_use_descr1.push_back(Crowbar_use_descr1_1);

	Door_descr1.push_back(Door_descr1_1);

	Screen_descr1.push_back(Screen_descr1_1);
	Screen_descr1.push_back(Screen_descr1_2);
	Screen_descr1.push_back(Screen_descr1_3);
	Screen_descr1.push_back(Screen_descr1_4);
	Screen_descr2.push_back(Screen_descr2_1);
	Screen_use_descr1.push_back(Screen_use_descr1_1);

	Controls_descr1.push_back(Controls_descr1_1);
	Controls_descr1.push_back(Controls_descr1_2);
	Controls_descr1.push_back(Controls_descr1_3);
	Controls_descr2.push_back(Controls_descr2_1);
	Controls_use_descr1.push_back(Controls_use_descr1_1);

	Terminal_descr1.push_back(Terminal_descr1_1);
	Terminal_descr1.push_back(Terminal_descr1_2);
	Terminal_descr2.push_back(Terminal_descr2_1);
	Terminal_use_descr1.push_back(Terminal_use_descr1_1);

	Body_descr1.push_back(Body_descr1_1);
	Body_descr1.push_back(Body_descr1_2);
	Body_descr1.push_back(Body_descr1_3);
	Body_descr2.push_back(Body_descr2_1);
	Body_use_descr1.push_back(Body_use_descr1_1);
	Body_use_descr2.push_back(Body_use_descr2_1);

	control_interactables.push_back(Light);
	control_interactables.push_back(Crowbar);
	control_interactables.push_back(Door);
	control_interactables.push_back(Screen);
	control_interactables.push_back(Controls);
	control_interactables.push_back(Body);
	control_interactables.push_back(Terminal);
}


bool ControlRoom::check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click,
	itemID item, Inventory& inventory, locationID& location) {

	auto prepare_message_box = [this, &message_box](std::vector<std::string> to_add) {
		for (int i = 0; i < to_add.size(); i++) {
			message_box.push_back(to_add[i]);
		}
	};

	auto in_inventory = [&inventory, &item]() {
		for (int i = 0; i < inventory.interactables.size(); i++) {
			if (inventory.interactables[i].id == item) return true;
		}
		return false;
	};

	if (left_click) {

		switch (item) {
			case controlLight:
				prepare_message_box(Light_descr1);
				break;

			case crowbar:
				if (control_state.crowbar_descr == 1) {
					prepare_message_box(Crowbar_descr1);
					control_state.crowbar_descr += 1;
				} else { prepare_message_box(Crowbar_descr2); }
				break;

			case controlToHallway:
				prepare_message_box(Door_descr1); 
				break;

			case controlScreen:
				if (control_state.screen_descr == 1) {
					prepare_message_box(Screen_descr1);
					control_state.screen_descr += 1;
				} else { prepare_message_box(Screen_descr2); }
				break;

			case controlTerminal:
				if (control_state.terminal_descr == 1) {
					prepare_message_box(Terminal_descr1);
					control_state.terminal_descr += 1;
				} else { prepare_message_box(Terminal_descr2); }
				break;

			case controlBody:
				if (control_state.body_descr == 1) {
					prepare_message_box(Body_descr1);
					control_state.body_descr += 1;
				} else { prepare_message_box(Body_descr2); }
				break;

			case controlControls:
				if (control_state.controls_descr == 1) {
					prepare_message_box(Controls_descr1);
					control_state.controls_descr += 1;
				} else { prepare_message_box(Controls_descr2); }
				break;
		}

	} else if (right_click) {

		switch (item) {
			case controlLight:
				prepare_message_box(Light_use_descr1);
				break;

			case crowbar:
				if (!control_state.picked_up_crowbar) {
					control_state.picked_up_crowbar = true;
					prepare_message_box(Crowbar_use_descr1);
					inventory.interactables.push_back(Crowbar);
				}
				break;

			case controlToHallway:
				location = Hallway1;
				break;

			case controlScreen:
				prepare_message_box(Screen_use_descr1);
				break;

			case controlTerminal:
				if (control_state.used_key_card) {
					control_state.accessed_terminal = true;
					return true;
				} else {
					prepare_message_box(Terminal_use_descr1);
				}
				break;

			case controlBody:
				if (control_state.interact_body_descr == 1) {
					prepare_message_box(Body_use_descr1);
					inventory.interactables.push_back(Key_card);
				} else {
					prepare_message_box(Body_use_descr2);
				}

				break;

			case controlControls:
				prepare_message_box(Controls_use_descr1);
				break;
		}
	}

	return false;
}

void ControlRoom::check_story(std::vector<std::string>& message_box) {

	if (!control_state.intro_text) {
		message_box.push_back(control_intro1);
		message_box.push_back(control_intro2);
		control_state.intro_text = true;
	}

}
