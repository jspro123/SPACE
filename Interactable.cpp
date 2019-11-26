#include "Interactable.hpp"


int sid_to_i (soundID id) {
	switch (id) {
	case SfingerOne:
		return 0;
	case SfingerTwo:
		return 1;
	case Semergency:
		return 2;
	case Sambience:
		return 3;
	case Sdoor_air:
		return 4;
	case Sdoor_open:
		return 5;
	case ScrowbarOne:
		return 6;
	case ScrowbarTwo:
		return 7;
	case SshipCrash:
		return 8;
	case Sforced:
		return 9;
	case Stape:
		return 10;
	case SengineFail:
		return 11;
	case SengineStart:
		return 12;
	case SbreakScreen:
		return 13;
	default:
		return -1;
	}
}

void Inventory :: update_inventory() {
	
	to_output.clear();

	if (state == DISPLAY) {
	
		if (interactables.size() == 0) {
			to_output.push_back(default_message);
			return;
		} 

		for (int i = 0; i < interactables.size(); i++) {
			to_output.push_back(interactables[i].name);
		}
	}

	else {
		//Nothing for now
	}
}

int Inventory::return_index(itemID id) {

	for (int i = 0; i < interactables.size(); i++) {
		if (interactables[i].id == id) {
			return i;
		}
	}

	return -1;
}

void Inventory::erase_item(itemID id) {

	int index = return_index(id);
	if (id != -1) {
		interactables.erase(interactables.begin() + index);
	}
}