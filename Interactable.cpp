#include "Interactable.hpp"



//Need to add support for pages later
//Need to make everything "chooseable"
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