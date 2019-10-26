
/*
 * StoryMode implements a story about The Planet of Choices.
 *
 */

#include "Mode.hpp"
#include "Sound.hpp"
#include <queue>
#include "Text.hpp"

struct TerminalMode : Mode {
	TerminalMode();
	virtual ~TerminalMode();

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//called to create menu for current scene:
	void enter_scene();

	//------ story state -------
	enum {
		Dunes,
		Oasis,
		Hill
	} location = Dunes;
	bool have_stone = false;
	bool added_stone = false;
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
	
	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(1920, 1080);
	glm::vec2 screen_left = glm::vec2(330, 220);
	glm::vec2 screen_right = glm::vec2(1630, 900);

	std::queue<std::pair<Sentence, int>> terminal_display;

	//------ background music -------
	std::shared_ptr< Sound::PlayingSample > background_music;
};
