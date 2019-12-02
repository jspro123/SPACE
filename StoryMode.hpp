#include "Mode.hpp"
#include "Sound.hpp"
#include "CabinRoom.hpp"
#include "HallwayOne.hpp"
#include "ControlRoom.hpp"
#include "TerminalMode.hpp"
#include "PodRoom.hpp"
#include <utility>

struct StoryMode : Mode {
	StoryMode();
	virtual ~StoryMode();

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	void check_sounds(std::vector<soundID> to_play, std::vector<soundID> to_kill);
	void check_mouse(bool left_click, bool right_click);
	bool check_mouseWithItem(bool left_click, bool right_click);
	bool in_box(glm::vec2 pos_cur, glm::vec2 pos_min, glm::vec2 pos_max);
	bool in_box_polygon(glm::vec2 pos_cur, glm::vec2 polygon[]);

	locationID location = Cabin;
	Inventory inventory;
	//See descriptions in the story section
	CabinRoom cabin_room = CabinRoom();
	HallwayOne hallwayone = HallwayOne();
	ControlRoom control_room = ControlRoom();
	PodRoom pod_room = PodRoom();

	std::vector<std::string> message_box;

	glm::vec2 textbox_left = glm::vec2(150, 800);
	glm::vec2 textbox_right = glm::vec2(1770, 1048);
	glm::vec2 item_pos[4][2];

	glm::vec2 raw_mouse_pos = glm::vec2(0, 0);
	glm::vec2 mouse_pos = glm::vec2(0, 0);
	glm::vec2 msg_pos = glm::vec2(0, 0);
	glm::vec2 floating_animation = glm::vec2(0, -1);
	int floating_interval = 10;
	int danger_flash = 10;
	bool floating_dir = false; //false - up, true - down
	int fading_interval = 0;
	glm::vec2 end_animation_interval = glm::vec2(0, -1);
	bool end_flag = false;

	void check_usage(itemID use, itemID on, bool click);

	//------ story state -------

	struct {

		bool played_opening = false;
		bool in_cutscene = true;
		bool faded_opening = false;

		bool faded_in_pod_one = false;
		bool faded_out_pod_one = false;

		bool faded_in_pod_two = false;
		bool faded_out_pod_two = false;

		float some_time_later = 10.0f;
		bool pushed_text = false;

	} story_state;

	// Cabin Interactables
	// light
	
	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(1920, 1200);
	bool message_box_visible = false;
	bool menu_visible = false;
	bool hint_visible = false;
	// tell if mouse is on items in inventory
	bool on_item = false;
	bool inventory_visible = false;
	int item_selected_ID = -1;
	int inventory_cur_page = 0;
	enum inventoryStatus {ShowItem, ShowDetail, UseItem} inventory_status = ShowItem;
	std::vector<bool> sounds_playing;
	std::vector<std::shared_ptr< Sound::PlayingSample >> sound_ptrs;
	std::shared_ptr< Sound::PlayingSample > background_music;
	soundID soundeffect = none;
	bool effect_playing = false;

	std::shared_ptr< TerminalMode > terminal;
};
