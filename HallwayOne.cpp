#include "HallwayOne.hpp"


HallwayOne::HallwayOne() {

	Door_1.position_min = glm::vec2(40, 470);
	Door_1.position_max = glm::vec2(310, 1000);
	
	Door_2.position_min = glm::vec2(800, 470);
	Door_2.position_max = glm::vec2(1070, 1000);

	Door_3.position_min = glm::vec2(1565, 470);
	Door_3.position_max = glm::vec2(1835, 1000);

	Door_3_panel.position_min = glm::vec2(1845, 660);
	Door_3_panel.position_max = glm::vec2(1890, 710);

	Door_1_descr1.push_back(Door_1_descr1_1);

	Door_2_descr1.push_back(Door_2_descr1_1);

	Door_3_descr1.push_back(Door_3_descr1_1);
	Door_3_use_descr1.push_back(Door_3_use_descr1_1);

	Door_3_panel_descr1.push_back(Door_3_panel_descr1_1);
	Door_3_panel_descr1.push_back(Door_3_panel_descr1_2);
	Door_3_panel_descr2.push_back(Door_3_panel_descr2_1);
	Door_3_panel_use_descr1.push_back(Door_3_panel_use_descr1_1);

	hallwayone_interactables.push_back(Door_1);
	hallwayone_interactables.push_back(Door_2);
	hallwayone_interactables.push_back(Door_3);
	hallwayone_interactables.push_back(Door_3_panel);
}


void HallwayOne::check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click,
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

			case DoorOneToCryo:
				prepare_message_box(Door_1_descr1);
				break;

			case DoorTwoToControl:
				prepare_message_box(Door_2_descr1);
				break;

			case DoorThreeToHallwayTwo:
				prepare_message_box(Door_3_descr1);
				break;

			case DoorThreePanel:
				if (hallwayone_state.door_3_panel_descr == 1) {
					prepare_message_box(Door_3_panel_descr1);
					hallwayone_state.door_3_panel_descr++;
				}
				else { prepare_message_box(Door_3_panel_descr2); }
				break;
		}

	} else if (right_click) {

		switch (item) {

			case DoorOneToCryo:
				location = Cabin;
				break;

			case DoorTwoToControl:
				location = Control;
				break;

			case DoorThreeToHallwayTwo:
				if (hallwayone_state.door_3_open) { location = Hallway2; }
				else { prepare_message_box(Door_3_use_descr1); }
				break;

			case DoorThreePanel:
				prepare_message_box(Door_3_panel_use_descr1);
				break;
		}
	}
}

void HallwayOne::check_story(std::vector<std::string>& message_box) {


}
