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

}


void ControlRoom::check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click,
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
			case lightSwitch:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_lights); }
				else if (cabin_state.light_switch_descr == 1) { prepare_message_box(Light_switch_descr1); }
				break;

			case toolbox:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_object); }
				else if (cabin_state.tool_box_descr == 1) {
					prepare_message_box(Tool_box_descr1);
					cabin_state.tool_box_descr += 1;
				}
				else { prepare_message_box(Tool_box_descr2); }
				break;

			case commanderBody:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_float); }
				else if (cabin_state.commander_descr == 1) { prepare_message_box(Commander_body_descr1); }
				break;

			case genericBody:
				if (!cabin_state.light_on) {}
				else if (cabin_state.commander_descr == 1) { prepare_message_box(Generic_body_descr1); }
				break;

			case brokenGlass:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_float); }
				else if (cabin_state.broken_glass_descr == 1) {
					prepare_message_box(Broken_glass_descr1);
					cabin_state.broken_glass_descr += 1;
				}
				else { prepare_message_box(Broken_glass_descr2); }
				break;

			case cryoToHallway:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_leave); }
				else if (cabin_state.cabin_door_descr == 1) { prepare_message_box(Cabin_door_descr1); }
				break;

			case playerPod:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_self); }
				else if (cabin_state.protag_pod_descr == 1) { prepare_message_box(Protag_pod_descr1); }
				break;

			case emptyPod:
				if (!cabin_state.light_on) {}
				else if (cabin_state.empty_pod_descr == 1) {
					prepare_message_box(Empty_pod_descr1);
					cabin_state.empty_pod_descr += 1;
				}
				else { prepare_message_box(Empty_pod_descr2); }
				break;

			case shipRail:
				if (!cabin_state.light_on) {}
				else if (cabin_state.ship_rail_descr == 1) { prepare_message_box(Ship_rail_descr1); }
				break;
		}

	} else if (right_click) {

		switch (item) {
			case lightSwitch:
				if (!cabin_state.light_on) { cabin_state.light_on = true; }
				else { prepare_message_box(Light_switch_use_descr1); }
				break;

			case toolbox:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_use_fail); }
				else { prepare_message_box(Tool_box_use_descr1); }
				break;

			case commanderBody:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_use_fail); }
				else { prepare_message_box(Commander_body_use_descr1); }
				break;

			case genericBody:
				if (!cabin_state.light_on) {}
				else { prepare_message_box(Generic_body_use_descr1); }
				break;

			case brokenGlass:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_use_fail); }
				else if (!in_inventory()) {
					inventory.interactables.push_back(Broken_Glass);
					prepare_message_box(Broken_glass_use_descr1);
				} else { prepare_message_box(Broken_glass_use_descr2); }
				break;

			case cryoToHallway:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_leave); }
				else { location = Hallway1; }
				break;

			case playerPod:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_use_self); }
				else { prepare_message_box(Protag_pod_use_descr1); }
				break;

			case emptyPod:
				if (!cabin_state.light_on) {}
				else { prepare_message_box(Empty_pod_use_descr1); }
				break;

			case shipRail:
				if (!cabin_state.light_on) {}
				else{ prepare_message_box(Ship_rail_use_descr1); }
				break;
		}
	}
}

void ControlRoom::check_story(std::vector<std::string>& message_box) {

	if (!cabin_state.intro_text) {

		//Sounds and shit later
		message_box.push_back(cryo_dark_intro1);
		message_box.push_back(cryo_dark_intro2);
		message_box.push_back(cryo_dark_intro3);
		//More sounds and shit
		message_box.push_back(cryo_dark_intro4);
		message_box.push_back(cryo_dark_intro5);
		message_box.push_back(cryo_dark_intro6);
		message_box.push_back(cryo_dark_intro7);
		message_box.push_back(cryo_dark_intro8);
		message_box.push_back(cryo_dark_intro9);
		cabin_state.intro_text = true;

	}
	else if (cabin_state.light_on && !cabin_state.light_on_text) {

		cryo_interactables.push_back(Empty_pod);
		cryo_interactables.push_back(Ship_rail);
		cryo_interactables.push_back(Generic_body);
		cryo_interactables.push_back(Generic_body2);

		//Scene goes to black
		message_box.push_back(cryo_light_intro1);
		//Scene cuts in; corpses and shit
		message_box.push_back(cryo_light_intro2);
		message_box.push_back(cryo_light_intro3);
		message_box.push_back(cryo_light_intro4);
		cabin_state.light_on_text = true;

	}
}
