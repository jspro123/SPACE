#include "Mode.hpp"
#include "Sound.hpp"
#include "Interactable.hpp"

struct StoryMode : Mode {
	StoryMode();
	virtual ~StoryMode();

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	void check_mouse(bool left_click, bool right_click);
	bool in_box(glm::vec2 pos_cur, glm::vec2 pos_min, glm::vec2 pos_max);

	Inventory inventory;
	Interactable Broken_Glass = Interactable(brokenGlass, "A piece of sharp glass", true, "It's sharp.");
	Interactable Light_switch = Interactable(lightSwitch, "Light switch", false, "Turns on the light");
	Interactable Tool_box = Interactable(toolbox, "Toolbox", false, "Stores things.");
	Interactable Commander_body = Interactable(commanderBody, "The Commander", false, "Commander...");
	Interactable Generic_body = Interactable(genericBody, "A crew member", false, "Dead...");
	Interactable Cabin_door = Interactable(cryoDoor, "A door.", false, "This leads to the hallway.");

	std::vector<Interactable> cryo_interactables;
	std::vector<std::string> message_box;

	glm::vec2 textbox_left = glm::vec2(150, 800);
	glm::vec2 textbox_right = glm::vec2(1770, 1048);
	glm::vec2 raw_mouse_pos = glm::vec2(0, 0);
	glm::vec2 mouse_pos = glm::vec2(0, 0);
	glm::vec2 msg_pos = glm::vec2(0, 0);

	//called to create menu for current scene:
	void enter_scene();

	//------ story state -------
	enum {
		Cabin
	} location = Cabin;

	// Cabin Interactables
	// light
	struct {
		bool light_on = false;
		bool tool_open = false;
	} cryo_room;
	
	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(1920, 1200);
	bool message_box_visible = false;
	bool menu_visible = false;
	bool hint_visible = false;
	bool inventory_visible = false;

	std::shared_ptr< Sound::PlayingSample > background_music;
};
