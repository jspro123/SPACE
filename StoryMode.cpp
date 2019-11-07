#include "StoryMode.hpp"
#include "TerminalMode.hpp"
#include "Sprite.hpp"
#include "DrawSprites.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "gl_errors.hpp"
#include "MenuMode.hpp"
#include "Sound.hpp"
#include "stdlib.h"

Sprite const* demo_background = nullptr;
Sprite const* demo_text_area = nullptr;

Sprite const *sprite_left_select = nullptr;

Sprite const *sprite_astronaut_bg = nullptr;
Sprite const *sprite_key_bg = nullptr;
Sprite const *sprite_msg_bg = nullptr;
Sprite const *sprite_light_upper_glass = nullptr;
Sprite const *sprite_light_body1 = nullptr;
Sprite const *sprite_light_body2 = nullptr;
Sprite const *sprite_light_cabin = nullptr;
Sprite const *sprite_dark_cabin = nullptr;
Sprite const *sprite_dark_glass = nullptr;

Load< SpriteAtlas > sprites(LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const *ret = new SpriteAtlas(data_path("space"));

	// sprite_left_select = &ret->lookup("text-select-left");

	sprite_astronaut_bg = &ret->lookup("astronaut_bg");
	sprite_key_bg = &ret->lookup("key_bg");
	sprite_msg_bg = &ret->lookup("msg_bg");

	demo_background = &ret->lookup("demo_screen_background");
	demo_text_area = &ret->lookup("textbox_area");
	sprite_light_upper_glass = &ret->lookup("light_upper_glass");
	sprite_light_body1 = &ret->lookup("light_body1");
	sprite_light_body2 = &ret->lookup("light_body2");
	sprite_light_cabin = &ret->lookup("light_cabin");
	sprite_dark_cabin = &ret->lookup("dark_cabin");
	sprite_dark_glass = &ret->lookup("dark_glass");
	return ret;
});

Load< SpriteAtlas > sprites_text(LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const *ret = new SpriteAtlas(data_path("trade-font"));

	return ret;
});

Load< Sound::Sample > music_cold_dunes(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("bgm.wav"));
});

Load< Sound::Sample > music_correct(LoadTagDefault, []() -> Sound::Sample *{
	return new Sound::Sample(data_path("correct.wav"));
});

// Load< Sound::Sample > music_wrong(LoadTagDefault, []() -> Sound::Sample *{
// 	return new Sound::Sample(data_path("wrong.wav"));
// });


StoryMode::StoryMode() {

	Light_switch.position_min = glm::vec2(1655, 585);
	Light_switch.position_max = glm::vec2(1690, 700);
	Tool_box.position_min = glm::vec2(0, 440);
	Tool_box.position_max = glm::vec2(200, 645);
	Broken_Glass.position_min = glm::vec2(700, 820);
	Broken_Glass.position_max = glm::vec2(760, 940);
	Commander_body.position_min = glm::vec2(310, 480);
	Commander_body.position_max = glm::vec2(630, 850);
	Generic_body.position_min = glm::vec2(850, 300);
	Generic_body.position_max = glm::vec2(1030, 560);
	Cabin_door.position_min = glm::vec2(1700, 300);
	Cabin_door.position_max = glm::vec2(1900, 1140);

	cryo_interactables.push_back(Light_switch);
	cryo_interactables.push_back(Tool_box);
	cryo_interactables.push_back(Broken_Glass);
	cryo_interactables.push_back(Commander_body);
	cryo_interactables.push_back(Generic_body);
	cryo_interactables.push_back(Cabin_door);
}

StoryMode::~StoryMode() {
	Sound::stop_all_samples();
}

bool StoryMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	auto clamp = [](float v, float x, float y) {
		if (v < x) return x;
		if (v > y) return y;
		return v;
	};

	// if (Mode::current.get() != this) return false;
	bool left_click = false;
	bool right_click = false;
	if (evt.type == SDL_KEYDOWN) {
		if (message_box_visible) {
			message_box.erase(message_box.begin());
			if (message_box.size() == 0) {
				message_box_visible = false;
			}
		} else if (evt.key.keysym.sym == SDLK_i) {
			inventory_visible = !inventory_visible;
			inventory.update_inventory();
		}
	} else if (evt.type == SDL_MOUSEMOTION) {

		float aspect = (float) (window_size.x) / window_size.y;
		float view_aspect = view_max.x / view_max.y;
		float offset = 0.0f;
		float bar_length = 0.0f;
		if (aspect > view_aspect) { //Too much horizontal space ==> Black bars on left and right
			float good_x = window_size.y * view_aspect; //The window size of x that corrects the aspect ratio
			offset = (window_size.x - good_x);
			bar_length = offset / 2; 
			mouse_pos.x = clamp((evt.motion.x - bar_length) * (view_max.x / (window_size.x - offset)), 0, view_max.x);
			mouse_pos.y = evt.motion.y * (view_max.y / window_size.y);
		} else if (aspect < view_aspect) { //Too much vertical screen ==> Black bars on top and bottom
			float good_y = window_size.x / view_aspect; //The window size of x that corrects the aspect ratio
			offset =  abs((good_y - window_size.y));
			bar_length = offset / 2; //How much black shit is on both sides of the screen
			mouse_pos.x = evt.motion.x * (view_max.x / window_size.x);
			mouse_pos.y = clamp((evt.motion.y - bar_length) * (view_max.y / (window_size.y - offset)), 0, view_max.y);
		} else {
			mouse_pos.x = evt.motion.x * (view_max.x / window_size.x);
			mouse_pos.y = evt.motion.y * (view_max.y / window_size.y);
		}

		//This maps the mouse coordinates to the corresponding coordinates on Yixin's art.
		//I think it makes more sense than what we had before, with all the negative numbers
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (message_box_visible) {
			message_box.erase(message_box.begin());
			if (message_box.size() == 0) {
				message_box_visible = false;
			}
		}
	} else if (evt.type == SDL_MOUSEBUTTONUP) {
		if (evt.button.button == SDL_BUTTON_RIGHT) {
			right_click = true;
		} else if (evt.button.button == SDL_BUTTON_LEFT) {
			left_click = true;
		}
	}
	check_mouse(left_click, right_click);
	return false;
}

void StoryMode::update(float elapsed) {
	// if (Mode::current.get() == this) {
	// 	//there is no menu displayed! Make one:
	// 	enter_scene();
	// }
	// if (!background_music || background_music->stopped) {
		// background_music = Sound::play(*music_cold_dunes, 1.0f);
	// }
}

bool StoryMode::in_box(glm::vec2 pos_cur, glm::vec2 pos_min, glm::vec2 pos_max) {
	if (pos_cur.x >= pos_min.x && pos_cur.x <= pos_max.x && pos_cur.y >= pos_min.y && pos_cur.y <= pos_max.y) {
		return true;
	} else {
		return false;
	}
}

void StoryMode::check_mouse(bool left_click, bool right_click) {
	//std::cout << "check\n";
	//std::cout << mouse_pos.x << " " << mouse_pos.y << "\n";
	//std::cout << left_click << " " << right_click << "\n";
	bool on_something = false;
	if (location == Cabin) {

		for (int i = 0; i < cryo_interactables.size(); i++) {
			if (in_box(mouse_pos, cryo_interactables[i].position_min, cryo_interactables[i].position_max)) {
				on_something = true;
				hint_visible = true;
				if (left_click) {
					//No need to write brek; ID's are unique
					switch (cryo_interactables[i].id) {

						case lightSwitch:
							light_switch.light_on ^= 1;
							break;

						case toolbox:
							message_box.emplace_back(Tool_box.description);
							break;

						case commanderBody:
							message_box.emplace_back(Commander_body.description);
							break;

						case genericBody:
							message_box.emplace_back(Generic_body.description);
							break;

						case brokenGlass:
							std::cout << "Here" << std::endl;
							inventory.interactables.push_back(Broken_Glass);
							message_box.emplace_back("Lemme stash this shit real quick...");
							cryo_interactables.erase(cryo_interactables.begin() + i);
							break;

						case cryoDoor:
							std::cout << "Do something" << std::endl;
							break;
							//Go to hallway
					}
				} 
			}
		}
	}

	if(message_box.size() != 0) message_box_visible = true;
	if (!on_something) hint_visible = false;
}

void StoryMode::enter_scene() {
	//just entered this scene, adjust flags and build menu as appropriate:
	std::vector< MenuMode::Item > items;
	glm::vec2 at(3.0f, view_max.y - 13.0f);

	std::shared_ptr< MenuMode > menu = std::make_shared< MenuMode >(items);
	menu->atlas = sprites;
	menu->left_select = sprite_left_select;
	menu->view_min = view_min;
	menu->view_max = view_max;
	menu->background = shared_from_this();
	Mode::current = menu;
}

void StoryMode::draw(glm::uvec2 const &drawable_size) {
	//clear the color buffer:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	{ //use a DrawSprites to do the drawing:
		DrawSprites draw(*sprites, view_min, view_max, drawable_size, DrawSprites::AlignPixelPerfect);
		glm::vec2 ul = glm::vec2(view_min.x, view_max.y);

		draw.draw(*demo_background, ul);
		if (light_switch.light_on) {
			draw.draw(*sprite_light_cabin, ul);
			draw.draw(*sprite_light_body1, ul);
			draw.draw(*sprite_light_body2, ul);
			draw.draw(*sprite_light_upper_glass, ul);
		} else {
			draw.draw(*sprite_dark_cabin, ul);
			draw.draw(*sprite_dark_glass, ul);
		}
		if (inventory_visible || message_box_visible) {
			draw.draw(*demo_text_area, ul);
		}

		if (move_remained > 0 && dir != 0) {
			if (dir == 1) {
				ast_y++;
			} else if (dir == 2) {
				ast_y--;
			} else if (dir == 3) {
				ast_x--;
			} else if (dir == 4) {
				ast_x++;
			}
			move_remained--;
		} else {
			dir = 0;
			move_remained = 100;
		}
		ul = glm::vec2(view_min.x + 5 * ast_x, view_max.y + 5 * ast_y);
		// draw.draw(*sprite_astronaut_bg, ul);
		// if (have_key) {
		// 	glm::vec2 col_key = glm::vec2(view_min.x + 500, view_max.y + 200);
		// 	// draw.draw(*sprite_key_bg, col_key);
		// 	if (abs(col_key.x - ul.x) < 40 && abs(col_key.y - ul.y) < 20) {
		// 		Sound::play(*music_correct, 1.0f);
		// 		have_key = false;
		// 		inventory.interactables.push_back(key);
		// 		message_box.emplace_back("A new item is added to your inventory!");
		// 		message_box_visible = 200;
		// 	}
		// }

		if (hint_visible) {
			glm::vec2 draw_mouse = glm::vec2(mouse_pos.x, mouse_pos.y);
			draw.draw(*sprite_msg_bg, draw_mouse);
		}
		
	}

	{ // draw text
		DrawSprites draw_text(*sprites_text, view_min, view_max, drawable_size, DrawSprites::AlignPixelPerfect);
		// glm::vec2 ul = glm::vec2(view_min.x, view_max.y);

		glm::vec2 min, max;

		glm::vec2 at = textbox_left;
		at.x += 20;
		at.y = view_max.y - textbox_left.y - 50;


		if (message_box_visible){
			draw_text.draw_text_short(message_box[0], at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
			at.y -= 35;
		} else if (inventory_visible) {
			inventory.update_inventory();
			for (int i = 0; i < inventory.to_output.size(); i++) {
				draw_text.get_text_extents(inventory.to_output[i], at, 1.0f, &min, &max);
				draw_text.draw_text_short(inventory.to_output[i], at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				// description
				// if (inventory.interactables.size() > 0) {
				// 	at.x += max.x;
				// 	draw_text.draw_text_short(": " + inventory.interactables[i].description, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				// }
				at.y -= 35;
			}
		}
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}
