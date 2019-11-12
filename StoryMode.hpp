#include "Mode.hpp"
#include "Sound.hpp"
#include "CabinRoom.hpp"
#include "HallwayOne.hpp"
#include "ControlRoom.hpp"

struct StoryMode : Mode {
	StoryMode();
	virtual ~StoryMode();

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	void check_mouse(bool left_click, bool right_click);
	bool in_box(glm::vec2 pos_cur, glm::vec2 pos_min, glm::vec2 pos_max);

	locationID location = Cabin;
	Inventory inventory;
	//See descriptions in the story section
	CabinRoom cabin_room = CabinRoom();
	HallwayOne hallwayone = HallwayOne();
	ControlRoom control_room = ControlRoom();

	std::vector<std::string> message_box;

	glm::vec2 textbox_left = glm::vec2(150, 800);
	glm::vec2 textbox_right = glm::vec2(1770, 1048);
	glm::vec2 raw_mouse_pos = glm::vec2(0, 0);
	glm::vec2 mouse_pos = glm::vec2(0, 0);
	glm::vec2 msg_pos = glm::vec2(0, 0);
	glm::vec2 floating_animation = glm::vec2(0, -1);
	int floating_interval = 10;
	bool floating_dir = false; //false - up, true - down

	//called to create menu for current scene:
	void enter_scene();
	void check_story();

	//------ story state -------

	// Cabin Interactables
	// light
	
	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(1920, 1200);
	bool message_box_visible = false;
	bool menu_visible = false;
	bool hint_visible = false;
	bool inventory_visible = false;
	int item_selected_ID = -1;
	int inventory_cur_page = 0;
	enum inventoryStatus {ShowItem, ShowDetail, UseItem} inventory_status = ShowItem;
	// std::set<std::pair(Interactable, Interactable)> item_interactions;

	float shake_constant = 0.1f;
	bool shake_left = false;
	float shake_time = shake_constant;
	std::shared_ptr< Sound::PlayingSample > background_music;
};
