#include "StoryMode.hpp"
#include "Sprite.hpp"
#include "TerminalMode.hpp"
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
Sprite const *sprite_light_upper_glass = nullptr;
Sprite const *sprite_light_body1 = nullptr;
Sprite const *sprite_light_body2 = nullptr;
Sprite const *sprite_light_cabin = nullptr;
Sprite const *sprite_dark_cabin = nullptr;
Sprite const *sprite_dark_glass = nullptr;
Sprite const *sprite_msg_bg = nullptr;
Sprite const* hallwayone_bg = nullptr;
Sprite const* hallwayone_door_1 = nullptr;
Sprite const* hallwayone_door_2 = nullptr;
Sprite const* hallwayone_door_3 = nullptr;
Sprite const* hallwayone_door_3_panel = nullptr;
Sprite const* control_bg = nullptr;
Sprite const* control_fg = nullptr;
Sprite const* control_crowbar = nullptr;
Sprite const* control_blood = nullptr;

Load< SpriteAtlas > sprites(LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const *ret = new SpriteAtlas(data_path("space"));

	// sprite_left_select = &ret->lookup("text-select-left");

	sprite_astronaut_bg = &ret->lookup("astronaut_bg");
	sprite_key_bg = &ret->lookup("key_bg");
	sprite_msg_bg = const_cast<Sprite*> (&ret->lookup("msg_bg"));

	demo_background = &ret->lookup("demo_screen_background");
	demo_text_area = &ret->lookup("textbox_area");
	sprite_light_upper_glass = &ret->lookup("light_upper_glass");
	sprite_light_body1 = &ret->lookup("light_body1");
	sprite_light_body2 = &ret->lookup("light_body2");
	sprite_light_cabin = &ret->lookup("light_cabin");
	sprite_dark_cabin = &ret->lookup("dark_cabin");
	sprite_dark_glass = &ret->lookup("dark_glass");
	hallwayone_bg = &ret->lookup("hallway_background");
	hallwayone_door_1 = &ret->lookup("hallway_cryo");
	hallwayone_door_2 = &ret->lookup("hallway_control");
	hallwayone_door_3 = &ret->lookup("hallway_hallway");
	hallwayone_door_3_panel = &ret->lookup("hallway_panel");
	control_bg = &ret->lookup("control_background");
	control_fg = &ret->lookup("control_foreground");
	control_crowbar = &ret->lookup("control_crowbar");
	control_blood = &ret->lookup("control_blood");
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

	auto clamp = [](float v, float x, float y) {
		if (v < x) return x;
		if (v > y) return y;
		return v;
	};

	// if (Mode::current.get() != this) return false;
	bool left_click = false; bool right_click = false;
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_i) {
			inventory_visible = !inventory_visible;
			inventory.update_inventory();
		} else {
			if (inventory_status == ShowItem && inventory.interactables.size() > 0) {
				if (evt.key.keysym.sym == SDLK_1) {
					item_selected_ID = 0;
					inventory_status = ShowDetail;
				} else if (evt.key.keysym.sym == SDLK_2) {
					item_selected_ID = 1;
					inventory_status = ShowDetail;
				}  else if (evt.key.keysym.sym == SDLK_3) {
					item_selected_ID = 2;
					inventory_status = ShowDetail;
				}  else if (evt.key.keysym.sym == SDLK_4) {
					item_selected_ID = 3;
					inventory_status = ShowDetail;
				} 
			} else if (inventory_status == ShowDetail) {
				if (evt.key.keysym.sym == SDLK_1) {
					inventory_status = UseItem;
				} else if (evt.key.keysym.sym == SDLK_2) {
					inventory_status = ShowItem;
					item_selected_ID = -1;
				}
			} else if (inventory_status == UseItem) {
				if (evt.key.keysym.sym == SDLK_1) {
					inventory_status = ShowDetail;
				} else if (evt.key.keysym.sym == SDLK_2) {
					inventory_status = ShowItem;
					item_selected_ID = -1;
				}
			}
		}
	} else if (evt.type == SDL_MOUSEMOTION) {

		//This maps the mouse coordinates to the corresponding coordinates on Yixin's art.
		//I think it makes more sense than what we had before, with all the negative numbers

		raw_mouse_pos = glm::vec2(evt.motion.x, evt.motion.y);
		float aspect = (float) (window_size.x) / window_size.y;
		float view_aspect = view_max.x / view_max.y;
		float offset = 0.0f;
		float bar_length = 0.0f;
		if (aspect > view_aspect) { //Too much horizontal space ==> Black bars on left and right
			float good_x = window_size.y * view_aspect; //The window size of x that corrects the aspect ratio
			offset = (window_size.x - good_x);
			bar_length = offset / 2; 
			//I clamp to "out of bound" coordinates in case any of the interactables start at (0, y) or something.
			mouse_pos.x = clamp((evt.motion.x - bar_length) * (view_max.x / (window_size.x - offset)), -1, view_max.x + 1);
			mouse_pos.y = evt.motion.y * (view_max.y / window_size.y);
		} else if (aspect < view_aspect) { //Too much vertical screen ==> Black bars on top and bottom
			float good_y = window_size.x / view_aspect; //The window size of x that corrects the aspect ratio
			offset =  abs((good_y - window_size.y));
			bar_length = offset / 2; //How much black shit is on both sides of the screen
			mouse_pos.x = evt.motion.x * (view_max.x / window_size.x);
			mouse_pos.y = clamp((evt.motion.y - bar_length) * (view_max.y / (window_size.y - offset)), -1, view_max.y + 1);
		} else {
			mouse_pos.x = evt.motion.x * (view_max.x / window_size.x);
			mouse_pos.y = evt.motion.y * (view_max.y / window_size.y);
		}

	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (evt.button.button == SDL_BUTTON_RIGHT) {
			right_click = true;
		}
		else if (evt.button.button == SDL_BUTTON_LEFT) {
			left_click = true;
		}
		if (message_box_visible || inventory_visible) { left_click = false; right_click = false; }
		if (message_box_visible) {
			message_box.erase(message_box.begin());
			if (message_box.size() == 0) {
				message_box_visible = false;
			}
		}
	}

	check_mouse(left_click, right_click);
	return false;
}

void StoryMode::update(float elapsed) {


	shake_time -= elapsed;

	if (shake_time < 0) {
		shake_time = shake_constant;
		shake_left = !shake_left;
	}
	check_story();

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

	auto prepare_message_box = [this](std::vector<std::string> to_add) {
		for (int i = 0; i < to_add.size(); i++) {
			message_box.push_back(to_add[i]);
		}
	};

	bool on_something = false;
	if (location == Cabin) {

		for (int i = 0; i < cabin_room.cryo_interactables.size(); i++) {
			Interactable current = cabin_room.cryo_interactables[i];
			glm::vec2 tar_min = current.position_min;
			glm::vec2 tar_max = current.position_max;
			if (current.id == brokenGlass || current.id == genericBody || current.id == commanderBody) {
				tar_min -= floating_animation;
				tar_max -= floating_animation;
			}
			if (in_box(mouse_pos, tar_min, tar_max)) {
				on_something = true;
				hint_visible = true;
				cabin_room.check_interactions(message_box, left_click, right_click, current.id, inventory, location);
			}
		}

	} else if (location == Hallway1) {
		for (int i = 0; i < hallwayone.hallwayone_interactables.size(); i++) {
			Interactable current = hallwayone.hallwayone_interactables[i];
			glm::vec2 tar_min = current.position_min;
			glm::vec2 tar_max = current.position_max;
			if (in_box(mouse_pos, tar_min, tar_max)) {
				on_something = true;
				hint_visible = true;
				hallwayone.check_interactions(message_box, left_click, right_click, current.id, inventory, location);
			}
		}
	} else if (location == Control) {
		for (int i = 0; i < control_room.control_interactables.size(); i++) {
			Interactable current = control_room.control_interactables[i];
			glm::vec2 tar_min = current.position_min;
			glm::vec2 tar_max = current.position_max;
			bool activate_terminal = false;
			if (in_box(mouse_pos, tar_min, tar_max)) {
				on_something = true;
				hint_visible = true;
				activate_terminal = control_room.check_interactions(message_box, left_click, right_click, current.id, inventory, location);
				if (activate_terminal) {
					std::shared_ptr< TerminalMode > terminal = std::make_shared< TerminalMode >();
					terminal->shared_from = shared_from_this();
					terminal->log_permission = control_room.control_state.diary_bio;
					terminal->door_permission = control_room.control_state.commander_bio;
					Mode::set_current(terminal);
				}
			}
		}
	}

	if(message_box.size() != 0) message_box_visible = true;
	if (!on_something || message_box_visible || inventory_visible) hint_visible = false;
}


void StoryMode::check_story() {

	switch (location) {
		
		case Cabin:
			cabin_room.check_story(message_box);
			break;

		case Hallway1:
			hallwayone.check_story(message_box);
			break;

		case Control:
			control_room.check_story(message_box);
	
	}

	if (message_box.size() != 0) message_box_visible = true;

}

void StoryMode::enter_scene() {
	//just entered this scene, adjust flags and build menu as appropriate:
	/*
	std::vector< MenuMode::Item > items;
	glm::vec2 at(3.0f, view_max.y - 13.0f);

	std::shared_ptr< MenuMode > menu = std::make_shared< MenuMode >(items);
	menu->atlas = sprites;
	menu->left_select = sprite_left_select;
	menu->view_min = view_min;
	menu->view_max = view_max;
	menu->background = shared_from_this();
	Mode::current = menu;
	*/
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
		if (location == Cabin) {
			if (cabin_room.cabin_state.light_on) {
				draw.draw(*sprite_light_cabin, ul);
				draw.draw(*sprite_light_body1, ul + floating_animation);
				draw.draw(*sprite_light_body2, ul + floating_animation);
				draw.draw(*sprite_light_upper_glass, ul + floating_animation);
			}
			else {
				draw.draw(*sprite_dark_cabin, ul);
				draw.draw(*sprite_dark_glass, ul);
			}
		} else if (location == Hallway1) {
			draw.draw(*hallwayone_bg, ul);
			draw.draw(*hallwayone_door_1, ul);
			draw.draw(*hallwayone_door_2, ul);
			draw.draw(*hallwayone_door_3, ul);
			draw.draw(*hallwayone_door_3_panel, ul);
		} else if (location == Control) {
			draw.draw(*control_bg, ul);
			draw.draw(*control_fg, ul);
			draw.draw(*control_crowbar, ul); //Only if haven't picked up yet
			draw.draw(*control_blood, ul);
		}
		if (inventory_visible || message_box_visible) {
			draw.draw(*demo_text_area, ul);
		}

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


		// animation
		if (floating_interval == 0) {
			if (floating_dir == false) {
				if (floating_animation.y > -10) {
					floating_animation.y --;
				} else {
					floating_dir = true;
				}
			} else {
				if (floating_animation.y < 10) {
					floating_animation.y ++;
				} else {
					floating_dir = false;
				}
			}
			floating_interval = 10;
			// call check mouse even if we don't move mouse
			check_mouse(false, false);
		}
		floating_interval --;

		if (hint_visible) {
			float good_x = (mouse_pos.x + 25) - (view_max.x / 2);
			float good_y = ((view_max.y / 2) - mouse_pos.y) + view_max.y - 25;
			draw.draw(*sprite_msg_bg, glm::vec2(good_x, good_y));
			draw.draw(*sprite_msg_bg, glm::vec2(good_x, good_y));
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
			//Hacky way of using the auto-wrap code without creating a Sentence object
			//for each message I want to print
			int current_chr = -1; 
			std::unordered_map <size_t, int> fit_list;
			draw_text.draw_text(message_box[0], at, FONT_SIZE, glm::u8vec4(0xff, 0xff, 0xff, 0xff), current_chr, 1, fit_list, textbox_left.x + 20, textbox_right.x - 20);
		} else if (inventory_visible) {
			inventory.update_inventory();
			if (inventory_status == ShowItem) {
				for (int i = 0; i < inventory.to_output.size(); i++) {
					std::string tmp = "";
					if (inventory.interactables.size() > 0) {
						tmp.append("[");
						tmp.append(std::to_string(i + 1));
						tmp.append("] ");
					}
					tmp.append(inventory.to_output[i]);
					draw_text.get_text_extents(tmp, at, 1.0f, &min, &max);
					draw_text.draw_text_short(tmp, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
					at.y -= 45;
				}
			} else if (inventory_status == ShowDetail) {
				if (item_selected_ID + inventory_cur_page * 4 >= inventory.interactables.size()) {
					item_selected_ID = -1;
					inventory_status = ShowItem;
				} else {
					Interactable cur_item = inventory.interactables[item_selected_ID];
					std::string tmp = cur_item.name;
					tmp.append(" : ");
					tmp.append(cur_item.description);
					draw_text.get_text_extents(tmp, at, 1.0f, &min, &max);
					draw_text.draw_text_short(tmp, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
					at.y -= 45;
					tmp = "[1] Use it";
					draw_text.get_text_extents(tmp, at, 1.0f, &min, &max);
					draw_text.draw_text_short(tmp, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
					at.y -= 45;
					tmp = "[2] Back to the item list";
					draw_text.get_text_extents(tmp, at, 1.0f, &min, &max);
					draw_text.draw_text_short(tmp, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
					at.y -= 45;
				}
			} else if (inventory_status == UseItem) {
				Interactable cur_item = inventory.interactables[item_selected_ID];
				std::string tmp = cur_item.name;
				tmp.append(" is selected.");
				draw_text.get_text_extents(tmp, at, 1.0f, &min, &max);
				draw_text.draw_text_short(tmp, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= 45;
				tmp = "Try to do something...";
				draw_text.get_text_extents(tmp, at, 1.0f, &min, &max);
				draw_text.draw_text_short(tmp, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= 45;
				tmp = "[1] Back to the selected item";
				draw_text.get_text_extents(tmp, at, 1.0f, &min, &max);
				draw_text.draw_text_short(tmp, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= 45;
				tmp = "[2] Back to the item list";
				draw_text.get_text_extents(tmp, at, 1.0f, &min, &max);
				draw_text.draw_text_short(tmp, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= 45;
			}
		}
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}
