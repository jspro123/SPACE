#include "PodRoom.hpp"


PodRoom::PodRoom() {

	Windshield.position_min = glm::vec2(720, 350);
	Windshield.position_max = glm::vec2(1150, 800);

	Pod1.position_min = glm::vec2(490, 140);
	Pod1.position_max = glm::vec2(1400 ,349);

	Pod2.position_min = glm::vec2(600, 801);
	Pod2.position_max = glm::vec2(1240, 1040);

	Wrench.position_min = glm::vec2(1265, 830);
	Wrench.position_max = glm::vec2(1455, 1015);

	Door_Hallway.position_min = glm::vec2(0, 25);
	Door_Hallway.position_max = glm::vec2(160, 965);

	Door_Broken.position_min = glm::vec2(1765, 140);
	Door_Broken.position_max = glm::vec2(1920, 1075);

	Door_Broken_Panel.position_min = glm::vec2(1723, 410);
	Door_Broken_Panel.position_max = glm::vec2(1770, 525);

	Tech.position_min = glm::vec2(1195, 1090);
	Tech.position_max = glm::vec2(1363, 1155);

	Door_Hallway_descr1.push_back(Door_Hallway_descr1_1);

	Door_Broken_descr1.push_back(Door_Broken_descr1_1);
	Door_Broken_use_descr1.push_back(Door_Broken_use_descr1_1);

	Door_Broken_Panel_descr1.push_back(Door_Broken_Panel_descr1_1);
	Door_Broken_Panel_use_descr1.push_back(Door_Broken_Panel_descr1_1);

	Pod_descr1.push_back(Pod_descr1_1);
	Pod_descr1.push_back(Pod_descr1_2);
	Pod_descr2.push_back(Pod_descr2_1);
	Pod_use_descr1.push_back(Pod_use_descr1_1);

	Windshield_descr1.push_back(Windshield_descr1_1);
	Windshield_descr2.push_back(Windshield_descr2_1);
	Windshield_use_descr1.push_back(Windshield_use_descr1_1);

	Wrench_descr1.push_back(Wrench_descr1_1);
	Wrench_descr1.push_back(Wrench_descr1_2);
	Wrench_descr2.push_back(Wrench_descr2_1);
	Wrench_use_descr1.push_back(Wrench_use_descr1_1);

	Tech_descr1.push_back(Tech_descr1_1);
	Tech_use_descr1.push_back(Tech_use_descr1_1);
}


bool PodRoom::check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click,
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


	} else if (right_click) {

	}

	return false;
}

void PodRoom::check_story(std::vector<std::string>& message_box) {


}
