#include "CabinRoom.hpp"

CabinRoom::CabinRoom() {

	Light_switch.position_min = glm::vec2(1655, 585);
	Light_switch.position_max = glm::vec2(1690, 700);

	Tool_box.position_min = glm::vec2(0, 440);
	Tool_box.position_max = glm::vec2(200, 645);

	Broken_Glass.position_min = glm::vec2(700, 820);
	Broken_Glass.position_max = glm::vec2(760, 940);

	Broken_Glass2.position_min = glm::vec2(430, 940);
	Broken_Glass2.position_max = glm::vec2(490, 1014);

	Broken_Glass3.position_min = glm::vec2(725, 240);
	Broken_Glass3.position_max = glm::vec2(785, 355);

	Commander_body.position_min = glm::vec2(310, 480);
	Commander_body.position_max = glm::vec2(630, 850);

	Generic_body.position_min = glm::vec2(850, 300);
	Generic_body.position_max = glm::vec2(1030, 560);

	Generic_body2.position_min = glm::vec2(526, 150);
	Generic_body2.position_max = glm::vec2(695, 283);

	Cabin_door.position_min = glm::vec2(1700, 300);
	Cabin_door.position_max = glm::vec2(1900, 1140);

	Protag_pod.position_min = glm::vec2(1375, 140);
	Protag_pod.position_max = glm::vec2(1605, 805);

	Empty_pod.position_min = glm::vec2(1126, 157);
	Empty_pod.position_max = glm::vec2(1330, 710);

	Ship_rail.position_min = glm::vec2(230, 385);
	Ship_rail.position_max = glm::vec2(336, 510);

	cryo_interactables.push_back(Light_switch);
	cryo_interactables.push_back(Tool_box);
	cryo_interactables.push_back(Broken_Glass);
	cryo_interactables.push_back(Broken_Glass2);
	cryo_interactables.push_back(Broken_Glass3);
	cryo_interactables.push_back(Commander_body);
	cryo_interactables.push_back(Cabin_door);
	cryo_interactables.push_back(Protag_pod);

	Broken_glass_descr1.push_back(Broken_glass_descr1_1);
	Broken_glass_descr1.push_back(Broken_glass_descr1_2);
	Broken_glass_descr1.push_back(Broken_glass_descr1_3);
	Broken_glass_descr1.push_back(Broken_glass_descr1_4);
	Broken_glass_descr2.push_back(Broken_glass_descr2_1);
	Broken_glass_use_descr1.push_back(Broken_glass_use_descr1_1);
	Broken_glass_use_descr2.push_back(Broken_glass_use_descr2_1);
	Broken_glass_use_descr3.push_back(Broken_glass_use_descr3_1);

	Light_switch_descr1.push_back(Light_switch_descr1_1);
	Light_switch_use_descr1.push_back(Light_switch_use_descr1_1);

	Tool_box_descr1.push_back(Tool_box_descr1_1);
	Tool_box_descr1.push_back(Tool_box_descr1_2);
	Tool_box_descr2.push_back(Tool_box_descr2_1);
	Tool_box_descr3.push_back(Tool_box_descr3_1);
	Tool_box_use_descr1.push_back(Tool_box_use_descr1_1);
	Tool_box_use_descr2.push_back(Tool_box_use_descr2_1);


	Commander_body_descr1.push_back(Commander_body_descr1_1);
	Commander_body_use_descr1.push_back(Commander_body_use_descr1_1);

	Generic_body_descr1.push_back(Generic_body_descr1_1);
	Generic_body_use_descr1.push_back(Generic_body_use_descr1_1);

	Cabin_door_descr1.push_back(Cabin_door_descr1_1);

	Protag_pod_descr1.push_back(Protag_pod_descr1_1);
	Protag_pod_descr1.push_back(Protag_pod_descr1_2);
	Protag_pod_use_descr1.push_back(Protag_pod_use_descr1_1);

	Empty_pod_descr1.push_back(Empty_pod_descr1_1);
	Empty_pod_descr1.push_back(Empty_pod_descr1_2);
	Empty_pod_descr1.push_back(Empty_pod_descr1_3);
	Empty_pod_descr1.push_back(Empty_pod_descr1_4);
	Empty_pod_descr1.push_back(Empty_pod_descr1_5);
	Empty_pod_descr2.push_back(Empty_pod_descr2_1);
	Empty_pod_use_descr1.push_back(Empty_pod_use_descr1_1);

	Ship_rail_descr1.push_back(Ship_rail_descr1_1);
	Ship_rail_use_descr1.push_back(Ship_rail_use_descr1_1);
}


void CabinRoom::check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click,
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

	auto remove_from_interactables = [this](itemID whatev) {
		for (int i = 0; i < cryo_interactables.size(); i++) {
			if (cryo_interactables[i].id == whatev) {
				cryo_interactables.erase(cryo_interactables.begin() + i);
				break;
			}
		}
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
				} else if (cabin_state.tool_box_descr == 2) {
					prepare_message_box(Tool_box_descr2);
				} else { prepare_message_box(Tool_box_descr3); }
				break;

			case commanderBody:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_float); }
				else if (cabin_state.commander_descr == 1) { prepare_message_box(Commander_body_descr1); }
				break;

			case genericBody:
				if (!cabin_state.light_on) {}
				else if (cabin_state.commander_descr == 1) { prepare_message_box(Generic_body_descr1); }
				break;

			case brokenGlassOne:
			case brokenGlassTwo:
			case brokenGlassThree:
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
				else if (cabin_state.tool_box_use_descr == 1) {
					prepare_message_box(Tool_box_use_descr1);
				} else {
					prepare_message_box(Tool_box_use_descr2);
				}
				break;

			case commanderBody:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_use_fail); }
				else { prepare_message_box(Commander_body_use_descr1); }
				break;

			case genericBody:
				if (!cabin_state.light_on) {}
				else { prepare_message_box(Generic_body_use_descr1); }
				break;

			case brokenGlassOne:
			case brokenGlassTwo:
			case brokenGlassThree:
				if (!cabin_state.light_on) { message_box.push_back(cryo_dark_use_fail); }
				else {
					if (item == brokenGlassOne) { 
						inventory.interactables.push_back(Broken_Glass); 
						cabin_state.took_glass_one = true; 
						remove_from_interactables(brokenGlassOne);
					}
					if (item == brokenGlassTwo) { 
						inventory.interactables.push_back(Broken_Glass2);  
						cabin_state.took_glass_two = true; 
						remove_from_interactables(brokenGlassTwo);
					}
					if (item == brokenGlassThree) { 
						inventory.interactables.push_back(Broken_Glass3);  
						cabin_state.took_glass_three = true; 
						remove_from_interactables(brokenGlassThree);
					}

					if (cabin_state.use_broken_glass_descr == 1) {
						prepare_message_box(Broken_glass_use_descr1);
						cabin_state.use_broken_glass_descr++;
					} else if (cabin_state.use_broken_glass_descr == 2) {
						prepare_message_box(Broken_glass_use_descr2);
						cabin_state.use_broken_glass_descr++;
					} else {
						inventory.erase_item(brokenGlassOne);
						inventory.erase_item(brokenGlassTwo);
						inventory.erase_item(brokenGlassThree);
						inventory.interactables.push_back(Bunch_Glass);
						prepare_message_box(Broken_glass_use_descr3);
					}

				}
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

std::pair<std::vector<soundID>, std::vector<soundID>> CabinRoom::check_story(
	std::vector<std::string>& message_box, std::vector<bool> sounds_playing, float elapsed) {

	std::vector<soundID> to_play;
	std::vector<soundID> to_kill;

	if (!cabin_state.intro_text) {

		if (cutscene_one.silence_start > 0) {
			cutscene_one.silence_start -= elapsed;
			return std::make_pair(to_play, to_kill);
		}

		if (!cutscene_one.played_door_open) {
			to_play.push_back(Sdoor_open);
			cutscene_one.played_door_open = true;
			return std::make_pair(to_play, to_kill);
		} else if (cutscene_one.played_door_open && sounds_playing[sid_to_i(Sdoor_open)]) {
			return std::make_pair(to_play, to_kill);
		}

		if (cutscene_one.silence_middle_one > 0) {
			cutscene_one.silence_middle_one -= elapsed;
			return std::make_pair(to_play, to_kill);
		}

		if (!cutscene_one.played_crowbar_hit_one_one) {
			to_play.push_back(ScrowbarOne);
			cutscene_one.played_crowbar_hit_one_one = true;
			return std::make_pair(to_play, to_kill);
		} else if (cutscene_one.played_crowbar_hit_one_one && sounds_playing[sid_to_i(ScrowbarOne)]) {
			return std::make_pair(to_play, to_kill);
		}

		if (cutscene_one.silence_middle_two > 0) {
			cutscene_one.silence_middle_two -= elapsed;
			return std::make_pair(to_play, to_kill);
		}

		if (!cutscene_one.played_crowbar_hit_one_two) {
			to_play.push_back(ScrowbarOne);
			cutscene_one.played_crowbar_hit_one_two = true;
			return std::make_pair(to_play, to_kill);
		} else if (cutscene_one.played_crowbar_hit_one_two && sounds_playing[sid_to_i(ScrowbarOne)]) {
			return std::make_pair(to_play, to_kill);
		}

		if (cutscene_one.silence_middle_three > 0) {
			cutscene_one.silence_middle_three -= elapsed;
			return std::make_pair(to_play, to_kill);
		}

		if (!cutscene_one.played_crowbar_hit_two) {
			to_play.push_back(ScrowbarTwo);
			cutscene_one.played_crowbar_hit_two = true;
			return std::make_pair(to_play, to_kill);
		} else if (cutscene_one.played_crowbar_hit_two && sounds_playing[sid_to_i(ScrowbarTwo)]) {
			return std::make_pair(to_play, to_kill);
		}

		if (cutscene_one.silence_middle_four > 0) {
			cutscene_one.silence_middle_four -= elapsed;
			return std::make_pair(to_play, to_kill);
		}

		if (!cutscene_one.played_crash) {
			to_play.push_back(SshipCrash);
			cutscene_one.played_crash = true;
			return std::make_pair(to_play, to_kill);
		}

		if (cutscene_one.silence_middle_five > 0) {
			cutscene_one.silence_middle_five -= elapsed;
			return std::make_pair(to_play, to_kill);
		}

		to_play.push_back(Semergency);

		if (!cutscene_one.pushed_text_one) {
			message_box.push_back(cryo_dark_intro1);
			message_box.push_back(cryo_dark_intro2);
			message_box.push_back(cryo_dark_intro3);
			cutscene_one.pushed_text_one = true;
		}
		
		if (!cutscene_one.played_protag_open && message_box.size() == 0) {
			to_play.push_back(Sdoor_air);
			cutscene_one.played_protag_open = true;
			return std::make_pair(to_play, to_kill);
		}
		else if (cutscene_one.played_protag_open && sounds_playing[sid_to_i(Sdoor_air)]) {
			return std::make_pair(to_play, to_kill);
		}

		if (cutscene_one.silence_middle_six > 0) {
			cutscene_one.silence_middle_six -= elapsed;
			return std::make_pair(to_play, to_kill);
		}

		if (cutscene_one.pushed_text_one && cutscene_one.played_protag_open &&
			cutscene_one.silence_middle_six < 0) {
			to_kill.push_back(Semergency);
			message_box.push_back(cryo_dark_intro4);
			message_box.push_back(cryo_dark_intro5);
			message_box.push_back(cryo_dark_intro6);
			message_box.push_back(cryo_dark_intro7);
			message_box.push_back(cryo_dark_intro8);
			message_box.push_back(cryo_dark_intro9);
			cabin_state.intro_text = true;
		}
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

	return std::make_pair(to_play, to_kill);
}