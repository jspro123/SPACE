#include "Interactable.hpp"
#include "Sound.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include <iostream> 
#include <utility>

struct CabinRoom {

	CabinRoom();

	//See descriptions in the story section
	Interactable Broken_Glass = Interactable(brokenGlassOne, "A piece of glass", true, "It's sharp.");
	Interactable Broken_Glass2 = Interactable(brokenGlassTwo, "A piece of glass", true, "It's sharp.");
	Interactable Broken_Glass3 = Interactable(brokenGlassThree, "A piece of glass", true, "It's sharp.");
	Interactable Bunch_Glass = Interactable(bunchOfGlass, "A bunch of glass pieces. ", true, "They're quite sharp.");
	Interactable Light_switch = Interactable(lightSwitch, "Light switch", false, "Turns on the light");
	Interactable Tool_box = Interactable(toolbox, "Toolbox", false, "Stores things.");
	Interactable Commander_body = Interactable(commanderBody, "The Commander", false, "Commander...");
	Interactable Generic_body = Interactable(genericBody, "A crew member", false, "Dead...");
	Interactable Generic_body2 = Generic_body;
	Interactable Cabin_door = Interactable(cryoToHallway, "A door", false, "This leads to the hallway.");
	Interactable Protag_pod = Interactable(playerPod, "My pod", false, "I woke up here.");
	Interactable Empty_pod = Interactable(emptyPod, "Not my pod", false, "I woke up here.");
	Interactable Ship_rail = Interactable(shipRail, "A rail", false, "We use these to get around.");
	Interactable Commander_finger = Interactable(commanderFinger, "The Commander's finger. ", true, "There's blood all around it.");
	Interactable Tape = Interactable(spaceTape, "A roll of space-tape. ", true, "Stronger than duct tape.");
	Interactable Hammer = Interactable(hammer, "A space-hammer. ", true, "Okay, it's just a hammer.");
	Interactable Stone = Interactable(redStone, "A red stone. ", true, "What we came for. . . ");

	std::vector<Interactable> cryo_interactables;

	//called to create menu for current scene:
	void check_interactions(std::vector<std::string>& message_box, bool left_click, bool right_click, itemID item,
		Inventory& inventory, locationID& location);
	std::pair<std::vector<soundID>, std::vector<soundID>> check_story(std::vector<std::string>& message_box, 
		std::vector<bool> sounds_playing, float elapsed);

	//------ story state -------

	// Cabin Interactables
	// light
	struct {

		bool intro_text = false;
		bool light_on = false;
		bool light_on_text = false;
		bool tool_open = false;
		bool tool_open_text = false;
		bool took_finger = false;
		bool took_glass_one = false;
		bool took_glass_two = false;
		bool took_glass_three = false;

		int broken_glass_descr = 1;
		int use_broken_glass_descr = 1;
		int commander_descr = 1;
		int body_descr = 1;
		int light_switch_descr = 1;
		int tool_box_descr = 1;
		int tool_box_use_descr = 1;
		int cabin_door_descr = 1;
		int protag_pod_descr = 1;
		int empty_pod_descr = 1;
		int ship_rail_descr = 1;

	} cabin_state;
	
	struct {

		float silence_start = 5.0f;
		bool played_door_open = false;
		float silence_middle_one = 2.0f;
		bool played_crowbar_hit_one_one = false; 
		float silence_middle_two = 1.5f;
		bool played_crowbar_hit_one_two = false; 
		float silence_middle_three = 1.5f;
		bool played_crowbar_hit_two = false;
		float silence_middle_four = 4.0f;
		bool played_crash = false;
		float silence_middle_five = 1.0f;
		bool played_emergency = false;

		bool pushed_text_one = false;
		bool pushed_text_two = false;
		bool played_protag_open = false;
		float silence_middle_six = 1.0f;

	} cutscene_one;



	/* ============= STORY TEXT ============= */

	/* ============= CRYO ROOM - DARK ============= */

	//Sounds for when text is being written (terminal and protagnist)
	//Sounds of pod opening
	//Sounds of glass shattering (killer breaking into the pods)
	//Sound of crowbar smashing against a human body
	//Sounds of a big (spaceship) crash
	//Sounds of emergency siren in the distance
	//Dark room fades in

	std::string cryo_dark_intro1 = "DAMAGE TO SHIP DETECTED";
	std::string cryo_dark_intro2 = "VITAL SIGNS IN POD 5 DETECTED";
	std::string cryo_dark_intro3 = "EMERGENCY WAKE-UP PROTOCOL ENGAGED...";
	
	//Sounds of "emergency wake up protocol" - maybe some air escaping? pshhhhhhhh

	std::string cryo_dark_intro4 = "Ahhhhhh. . . my body's aching everywhere. . .";
	std::string cryo_dark_intro5 = ". . .? What's that sound? ";
	std::string cryo_dark_intro6 = "No. . . don't tell me. . . are we still in space? ";
	std::string cryo_dark_intro7 = "It sounds like the ship is damaged. Shit. I need to go look into that, now. ";
	std::string cryo_dark_intro8 = ". . .";
	std::string cryo_dark_intro9 = "But what's that smell? There's something in this room. . .";

	/* ============= CRYO ROOM - DARK =============*/

	std::string cryo_dark_self = "Something's wrong. I have to check it out. ";
	std::string cryo_dark_lights = "I should turn on the lights. ";
	std::string cryo_dark_float = "There's something floating in the dark. . .";
	std::string cryo_dark_object = "There's something there, but it's too dark to make out. ";
	std::string cryo_dark_leave = "Fixing the ship is important, but I have a really bad feeling about that smell.";
	std::string cryo_dark_use_self = "Not right now.";
	std::string cryo_dark_use_fail = "Not without the lights on.";

	/* ============= CRYO ROOM - LIGHT - INTRO =============*/

	//Scene goes to black
	std::string cryo_light_intro1 = "No. . . !";
	//Scene cuts in; corpses and shit
	std::string cryo_light_intro2 = "No, no, no, no, no, no! "; //Shaking effect
	std::string cryo_light_intro3 = "What the fuck happened?!";
	std::string cryo_light_intro4 = "Why's everyone. . .";

	/* ============= CRYO ROOM - LIGHT =============*/

	std::vector<std::string> Broken_glass_descr1;
	std::vector<std::string> Broken_glass_descr2;
	std::vector<std::string> Broken_glass_descr3;
	std::vector<std::string> Broken_glass_use_descr1;
	std::vector<std::string> Broken_glass_use_descr2;
	std::vector<std::string> Broken_glass_use_descr3;
	std::string Broken_glass_descr1_1 = "A shard from one of the pods screens. ";
	std::string Broken_glass_descr1_2 = "It's strange. The screens are made to be pretty tough. To break one of them, you'd need a lot of force.";
	std::string Broken_glass_descr1_3 = ". . .";
	std::string Broken_glass_descr1_4 = ". . . or a lot of time. ";
	std::string Broken_glass_descr2_1 = "A shard from one of the pods screens. ";
	std::string Broken_glass_use_descr1_1 = "I should take one, might be useful.";
	std::string Broken_glass_use_descr2_1 = "I'll take another one, why not. ";
	std::string Broken_glass_use_descr3_1 = "And a third. . .  think I've got all the glass I'll ever need, now. ";

	std::vector<std::string> Light_switch_descr1;
	std::vector<std::string> Light_switch_use_descr1;
	std::string Light_switch_descr1_1 = "The light switch. Nothing special about it. ";
	std::string Light_switch_use_descr1_1 = "I'm not turning off the lights. Not after what I've seen. ";

	std::vector<std::string> Tool_box_descr1;
	std::vector<std::string> Tool_box_descr2;
	std::vector<std::string> Tool_box_descr3;
	std::vector<std::string> Tool_box_use_descr1;
	std::vector<std::string> Tool_box_use_descr2;
	std::string Tool_box_descr1_1 = "Hm. It's locked.";
	std::string Tool_box_descr1_2 = "I feel like there's something important inside. I need to break in somehow.";
	std::string Tool_box_descr2_1 = "There's something important inside. I need to break in somehow.";
	std::string Tool_box_descr3_1 = "It's wide open. ";
	std::string Tool_box_use_descr1_1 = "It's locked, but the lock seems weak.";
	std::string Tool_box_use_descr2_1 = "It's already open. There's nothing left to take. ";

	std::vector<std::string> Commander_body_descr1;
	std::vector<std::string> Commander_body_use_descr1;
	std::string Commander_body_descr1_1 = "Commander. . . ";
	std::string Commander_body_use_descr1_1 = "No. . . not without a good reason.";

	std::vector<std::string> Generic_body_descr1;
	std::vector<std::string> Generic_body_use_descr1;
	std::string Generic_body_descr1_1 = "Dead. . . ";
	std::string Generic_body_use_descr1_1 = "No. . . let them rest.";

	std::vector<std::string> Cabin_door_descr1;
	std::string Cabin_door_descr1_1 = "This'll take me to the main hallway. I can get to the control room from there. ";

	std::vector<std::string> Protag_pod_descr1;
	std::vector<std::string> Protag_pod_use_descr1;
	std::string Protag_pod_descr1_1 = "How long have I been asleep? How much time has passed? ";
	std::string Protag_pod_descr1_2 = ". . . how long have they been dead?";
	std::string Protag_pod_use_descr1_1 = "Now's not the time.";

	std::vector<std::string> Empty_pod_descr1;
	std::vector<std::string> Empty_pod_descr2;
	std::vector<std::string> Empty_pod_use_descr1;
	std::string Empty_pod_descr1_1 = "Besides mine, this is the only pod that isn't smashed apart. Let me take a look. ";
	std::string Empty_pod_descr1_2 = ". . . hmm? The pod succesfully turned off at the instructed date. So they woke up early?";
	std::string Empty_pod_descr1_3 = "Who was sleeping here?";
	std::string Empty_pod_descr1_4 = ". . . ";
	std::string Empty_pod_descr1_5 = ". . . the only person not in this room. ";
	std::string Empty_pod_descr2_1 = "Why. . .";
	std::string Empty_pod_use_descr1_1 = "Now's not the time.";

	std::vector<std::string> Ship_rail_descr1;
	std::vector<std::string> Ship_rail_use_descr1;
	std::string Ship_rail_descr1_1 = "A ship rail. We use these to get around in zero-gravity. ";
	std::string Ship_rail_use_descr1_1 = "Not sure how I'd use it.";

};
