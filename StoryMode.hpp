
/*
 * StoryMode implements a story about The Planet of Choices.
 *
 */

#include "Mode.hpp"
#include "Sound.hpp"

struct StoryMode : Mode {
	StoryMode();
	virtual ~StoryMode();

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//called to create menu for current scene:
	void enter_scene();

	//------ story state -------
	enum {
		Store,
		Room
	} location = Store;
	enum {
		CPU,
		GPU,
		PL
	} Type;
	bool have_GPU = false;
	bool have_CPU = false;
	bool have_key = true;
	bool have_PC = false;
	bool build_res = false;
	bool correct = false;
	bool wrong = false;
	struct {
		bool first_visit = true;
		bool wont_leave = false;
	} dunes;
	struct {
		bool first_visit = true;
		bool took_stone = false;
	} oasis;
	struct {
		bool first_visit = true;
		bool added_stone = false;
	} hill;
	int move_remained = 0;
	int dir = 0; // 1 - up, 2 - down, 3 - left, 4 - right
	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(1920, 1080);
	int ast_x = 0;
	int ast_y = 0;
	std::vector<std::string> inventory;
	std::shared_ptr< Sound::PlayingSample > background_music;
};
