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
}

StoryMode::~StoryMode() {
	Sound::stop_all_samples();
}

bool StoryMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	// if (Mode::current.get() != this) return false;
	bool left_click = false;
	bool right_click = false;
	if (evt.type == SDL_KEYDOWN) {
		// if (dir != 0) return true;;
		// if (evt.key.keysym.sym == SDLK_UP) {
		// 	dir = 1;
		// 	move_remained = 100;
		// 	// return true;
		// } else if (evt.key.keysym.sym == SDLK_DOWN) {
		// 	dir = 2;
		// 	move_remained = 100;
		// 	// return true;
		// } else if (evt.key.keysym.sym == SDLK_RETURN) {
		// 	Mode::set_current(std::make_shared< TerminalMode >());
		// } else if (evt.key.keysym.sym == SDLK_LEFT) {
		// 	dir = 3;
		// 	move_remained = 100;
		// } else 
		if (evt.key.keysym.sym == SDLK_RIGHT) {
			dir = 4;
			move_remained = 100;
			light_switch.light_on ^= 1;
		} else if (evt.key.keysym.sym == SDLK_i) {
			inventory_visible = !inventory_visible;
			inventory.update_inventory();
		} else if (evt.key.keysym.sym == SDLK_m) {
			message_box_visible = 200;
		}
	} else if (evt.type == SDL_MOUSEMOTION) {
		mouse_pos = glm::vec2(evt.motion.x * 2 - view_max.x / 2, view_max.y * 1.5 - evt.motion.y * 2);
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
	std::cout<<"check\n";			
	std::cout<<mouse_pos.x<<" "<<mouse_pos.y<<"\n";
	std::cout<<left_click<<" "<<right_click<<"\n";
	if (location == Cabin) {
		if (light_switch.light_on) {
			// set light interactables
			if (in_box(mouse_pos, light_switch.pos_min, light_switch.pos_max)) {
				hint_visible = true;
				if (left_click) {
					light_switch.light_on ^= 1;
				}
			} else if (in_box(mouse_pos, tool_box.pos_min, tool_box.pos_max)) {
				hint_visible = true;
				if (left_click) {
					if (have_key) {
						message_box.emplace_back("The tool box is unlocked now!");
					} else {
						message_box.emplace_back("The tool box is locked!");
						// get a hammer and some other things
					}
					message_box_visible = 200;
				}
			} else if (in_box(mouse_pos, commander_body.pos_min, commander_body.pos_max)) {
				hint_visible = true;
				if (left_click) {
					message_box.emplace_back("A dead body...");
					message_box_visible = 200;
				}
			} else if (in_box(mouse_pos, light_body_cabin.pos_min, light_body_cabin.pos_max)) {
				hint_visible = true;
				if (left_click) {
					message_box.emplace_back("A dead body...");
					message_box_visible = 200;
				}
			} else if (in_box(mouse_pos, pickable_glass.pos_min, pickable_glass.pos_max)) {
				hint_visible = true;
				if (left_click) {
					if (have_glass == false) {
						have_glass = true;
						inventory.interactables.push_back(broken_glass);
						message_box.emplace_back("A new item is added to your inventory!");
						message_box_visible = 200;
					}
				}
			} else if (in_box(mouse_pos, cabin_door.pos_min, cabin_door.pos_max)) {
				hint_visible = true;
				if (left_click) {
					// go to the hall way
				}
			} else {
				hint_visible = false;
			}
		} else {
			if (in_box(mouse_pos, light_switch.pos_min, light_switch.pos_max)) {
				hint_visible = true;
				if (left_click) {
					light_switch.light_on ^= 1;
				}
			} else {
				hint_visible = false;
			}
		}
	}
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
		if (inventory_visible || message_box_visible > 0) {
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
			msg_pos.x = mouse_pos.x + 60;
			msg_pos.y = mouse_pos.y;
			draw.draw(*sprite_msg_bg, msg_pos);
		}
		
	}

	{ // draw text
		DrawSprites draw_text(*sprites_text, view_min, view_max, drawable_size, DrawSprites::AlignPixelPerfect);
		// glm::vec2 ul = glm::vec2(view_min.x, view_max.y);

		glm::vec2 min, max;

		glm::vec2 at = textbox_left;
		at.x += 20;
		at.y = view_max.y - textbox_left.y - 50;


		if (message_box_visible > 0){
			message_box_visible --;
			for (int i = (int) message_box.size() - 1; i < message_box.size(); i++) {
				draw_text.draw_text_short(message_box[i], at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= 35;
			}
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
