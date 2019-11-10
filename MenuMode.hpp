/*
 * MenuMode is a game mode that implements a multiple-choice system.
 *
 */

#include "Sprite.hpp"
#include "Mode.hpp"

#include <vector>
#include <functional>

struct MenuMode : Mode {
	struct Item;
	MenuMode(std::vector< Item > const &items);
	virtual ~MenuMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- menu state -----

	//Each menu item is an "Item":
	struct Item {
		Item(
			std::string const& name_,
			Sprite const* sprite_ = nullptr,
			float scale_ = 1.0f,
			int current_chr_ = 0,
			float wait_to_print_ = 1.0f,
			int drawing_speed_ = 1,
			float start_at_ = 200,
			float wrap_at_ = 1000, 
			glm::u8vec4 const &tint_ = glm::u8vec4(0xff),
			std::function< void(Item const &) > const &on_select_ = nullptr,
			glm::vec2 const &at_ = glm::vec2(0.0f)
			) : name(name_), sprite(sprite_), scale(scale_), current_chr(current_chr_), wait_to_print(wait_to_print_), 
			drawing_speed(drawing_speed_), start_at(start_at_), wrap_at(wrap_at_), tint(tint_), selected_tint(tint_), on_select(on_select_), at(at_) {
		}
		std::string name;
		Sprite const *sprite; //sprite drawn for item
		float scale; //scale for sprite
		int current_chr = 0;; //Used when drawing slowly. -1 if done
		float wait_to_print = 0.0f; //Number of seconds to wait before printing
		int drawing_speed = 1;
		float start_at; //Which coord to start at
		float wrap_at; //Which coord to wrap at. 
		glm::u8vec4 tint; //tint for sprite (unselected)
		glm::u8vec4 selected_tint; //tint for sprite (selected)
		std::function< void(Item const &) > on_select; //if set, item is selectable
		glm::vec2 at; //location to draw item
		std::unordered_map <size_t, int> fit_list; //Used by the line wrapping to make 
										   //sure words don't spill to the next line (ironic).

	};
	std::vector< Item > items;

	//call to arrange items in a centered list:
	void layout_items(float gap = 0.0f);

	//if set, used to highlight the current selection:
	Sprite const *left_select = nullptr;
	Sprite const *right_select = nullptr;

	glm::vec2 left_select_offset = glm::vec2(0.0f);
	glm::vec2 right_select_offset = glm::vec2(0.0f);

	glm::u8vec4 left_select_tint = glm::u8vec4(0xff);
	glm::u8vec4 right_select_tint = glm::u8vec4(0xff);

	float select_bounce_amount = 0.0f;
	float select_bounce_acc = 0.0f;

	//must be set to the atlas from which all the sprites used herein are taken:
	SpriteAtlas const *atlas = nullptr;

	//currently selected item:
	uint32_t selected = -1;

	//area to display; by default, menu lays items out in the [-1,1]^2 box:
	glm::vec2 view_min = glm::vec2(-1.0f, -1.0f);
	glm::vec2 view_max = glm::vec2( 1.0f,  1.0f);

	//if not nullptr, background's functions are called as follows:
	// background->handle_event() is called at the end of handle_event() [if this doesn't handle the event]
	// background->update() is called at the end of update()
	// background->draw() is called at the start of draw()
	//IMPORTANT NOTE: this means that if background->draw() ends up deleting this (e.g., by removing
	//  the last shared_ptr that references it), then it will crash. Don't do that!
	std::shared_ptr< Mode > background;

};
