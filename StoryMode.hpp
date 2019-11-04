
/*
 * StoryMode implements a story about The Planet of Choices.
 *
 */

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

	bool inventory_visible = false;
	Inventory inventory;
	Interactable key = Interactable("A key", nullptr, true, "This is an important item.");
	Interactable broken_glass = Interactable("a piece of sharp glass", nullptr, true, 
		"Maybe you can do something with this.");

	std::vector<std::string> message_box;
	int message_box_visible = 0;
	glm::vec2 textbox_left = glm::vec2(150, 800);
	glm::vec2 textbox_right = glm::vec2(1770, 1048);

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
		glm::vec2 pos_min = glm::vec2(680, 1100);
		glm::vec2 pos_max = glm::vec2(750, 1220);
		bool light_on = false;
	} light_switch;
	
	struct {
		glm::vec2 pos_min = glm::vec2(-960, 1080);
		glm::vec2 pos_max = glm::vec2(-760, 1400);
		bool open = false;
	} tool_box;
		
	struct {
		glm::vec2 pos_min = glm::vec2(-660, 800);
		glm::vec2 pos_max = glm::vec2(-330, 1320);
	} commander_body;

	struct {
		glm::vec2 pos_min = glm::vec2(-200, 1250);
		glm::vec2 pos_max = glm::vec2(80, 1550);
	} light_body_cabin;

	struct {
		glm::vec2 pos_min = glm::vec2(-280, 850);
		glm::vec2 pos_max = glm::vec2(-190, 1000);
	} pickable_glass;

	struct {
		glm::vec2 pos_min = glm::vec2(800, 800);
		glm::vec2 pos_max = glm::vec2(950, 1500);
	} cabin_door;

	//dark


	bool have_key = false;
	bool have_glass = false;
	struct {
		bool first_visit = true;
		bool wont_leave = false;
	} dunes;
	int move_remained = 0;
	int dir = 0; // 1 - up, 2 - down, 3 - left, 4 - right
	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(1920, 1200);
	int ast_x = 0;
	int ast_y = 0;
	bool menu_visible = false;
	bool hint_visible = false;
	std::shared_ptr< Sound::PlayingSample > background_music;
};
