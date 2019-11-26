#include "Interactable.hpp"

struct PodRoom {

	PodRoom();

	//See descriptions in the story section

	Interactable Windshield = Interactable(windShield, "The pod's windshield. ", false, " . ");
	Interactable Pod1 = Interactable(escapePod, "An escape pod. ", false, " . ");
	Interactable Pod2 = Interactable(escapePod, "An escape pod. ", false, " . ");
	Interactable Wrench = Interactable(wrench, "A wrench. ", false, " . ");
	Interactable Door_Hallway = Interactable(bayToHallway, "A door. ", false, " . ");
	Interactable Door_Broken = Interactable(bayToBroken, "A door. ", false, " . ");
	Interactable Door_Broken_Panel = Interactable(bayToBrokenPanel, "A panel. ", false, " . ");
	Interactable Tech = Interactable(podTech, "A bunch of tech. ", false, " . ");

	std::vector<Interactable> pod_interactables;

	bool check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click, itemID item,
		Inventory& inventory, locationID& location);
	std::pair<std::vector<soundID>, std::vector<soundID>> check_story(std::vector<std::string>& message_box,
		std::vector<bool> sounds_playing, float elapsed);

	//------ story state -------

	// Cabin Interactables
	// light
	struct {

		bool glass_set_down = false;
		bool glass_taped_up = false;
		bool need_chip = false;
		bool attempt_take_off = false;
		bool attempted_take_off = false;

		int windshield_descr = 1;
		int pod_descr = 1;
		int wrench_descr = 1;

	} pod_state;

	struct {

		bool pushed_text_one = false;
		bool played_failed_launch = false;

	} cutscene_one;
	
	/* ============= POD ROOM =============*/

	std::vector<std::string> Door_Hallway_descr1;
	std::string Door_Hallway_descr1_1 = "This'll take me back to the hallway. ";

	std::vector<std::string> Door_Broken_descr1;
	std::vector<std::string> Door_Broken_use_descr1;
	std::string Door_Broken_descr1_1 = "This leads to the other escape pods. ";
	std::string Door_Broken_use_descr1_1 = "That's not a good idea. ";

	std::vector<std::string> Door_Broken_Panel_descr1;
	std::vector<std::string> Door_Broken_Panel_use_descr1;
	std::string Door_Broken_Panel_descr1_1 = "Looks like there's a massive air leak beyond this door. ";
	std::string Door_Broken_Panel_use_descr1_1 = "It's just a screen. Nothing I can do.";

	std::vector<std::string> Pod_descr1;
	std::vector<std::string> Pod_descr2;
	std::vector<std::string> Pod_use_descr1;
	std::string Pod_descr1_1 = "An escape pod. My only chance at getting out of here. ";
	std::string Pod_descr1_2 = "Looks like it's not in the best shape, though. . .  ";
	std::string Pod_descr2_1 = "An escape pod. My only chance at getting out of here. ";
	std::string Pod_use_descr1_1 = "I have to repair it first. ";

	std::vector<std::string> Windshield_descr1;
	std::vector<std::string> Windshield_descr2;
	std::vector<std::string> Windshield_use_descr1;
	std::string Windshield_descr1_1 = "Shit, it's broken pretty bad. ";
	std::string Windshield_descr2_1 = "All better now. Hopefully. ";
	std::string Windshield_use_descr1_1 = "Nothing I can really do with my bare hands. ";

	std::vector<std::string> Wrench_descr1;
	std::vector<std::string> Wrench_descr2;
	std::vector<std::string> Wrench_use_descr1;
	std::string Wrench_descr1_1 = "A wrench. Looks like the crashed knocked it out of the tool-box. ";
	std::string Wrench_descr1_2 = "It's also probably what damaged the escape pod. Sigh.  ";
	std::string Wrench_descr2_1 = "A wrench. Looks like the crashed knocked it out of the tool-box. ";
	std::string Wrench_use_descr1_1 = "I have a really bad feeling all of a sudden. Better not touch it. ";

	std::vector<std::string> Tech_descr1;
	std::vector<std::string> Tech_use_descr1;
	std::string Tech_descr1_1 = "A status-panel. Looks like the pod is launchable. ";
	std::string Tech_use_descr1_1 = "I can't really do anything to it. ";


	/* ============= TAKE OFF - 1 =============*/

	std::string takeoff1_1 = "Alright, okay, I can do this. ";
	std::string takeoff1_2 = "Please work. . . please, please, work. . . ";
	std::string takeoff1_3 = "No. . . fuck no. . .  ";
	std::string takeoff1_4 = "It's all over for me. . .  ";
	std::string takeoff1_5 = ". . .  ";
	std::string takeoff1_6 = "Wait. . . looks like only the CCU chip is damaged.  ";
	std::string takeoff1_7 = "Maybe I can fix this. . . ";
};
