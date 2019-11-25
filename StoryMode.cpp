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
#include "GlobalSounds.hpp"
#include <algorithm>

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
Sprite const* hallwayone_door_green = nullptr;
Sprite const* hallwayone_door_red = nullptr;
// Sprite const* hallwayone_door_3 = nullptr;
// Sprite const* hallwayone_door_3_panel = nullptr;
Sprite const* control_bg = nullptr;
Sprite const* control_fg = nullptr;
Sprite const* control_crowbar = nullptr;
Sprite const* control_blood = nullptr;
Sprite const* all_black = nullptr;

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
	// hallwayone_bg = &ret->lookup("hallway_background");
	hallwayone_door_green = &ret->lookup("hallway_green");
	hallwayone_door_red = &ret->lookup("hallway_red");
	// hallwayone_door_3 = &ret->lookup("hallway_hallway");
	// hallwayone_door_3_panel = &ret->lookup("hallway_panel");
	control_bg = &ret->lookup("control_background");
	control_fg = &ret->lookup("control_foreground");
	control_crowbar = &ret->lookup("control_crowbar");
	control_blood = &ret->lookup("control_blood");
	all_black = &ret->lookup("black_screen");
	return ret;
});

Load< SpriteAtlas > sprites_text(LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const *ret = new SpriteAtlas(data_path("trade-font"));

	return ret;
});

Load< Sound::Sample > music_correct(LoadTagDefault, []() -> Sound::Sample *{
	return new Sound::Sample(data_path("correct.wav"));
});

// Load< Sound::Sample > music_wrong(LoadTagDefault, []() -> Sound::Sample *{
// 	return new Sound::Sample(data_path("wrong.wav"));
// });


StoryMode::StoryMode() {
	item_pos[0][0] = glm::vec2(150, 810);
	item_pos[0][1] = glm::vec2(600, 860);

	item_pos[1][0] = glm::vec2(150, 870);
	item_pos[1][1] = glm::vec2(600, 920);

	item_pos[2][0] = glm::vec2(150, 930);
	item_pos[2][1] = glm::vec2(600, 980);

	item_pos[3][0] = glm::vec2(150, 990);
	item_pos[3][1] = glm::vec2(600, 1048);

	for (int i = 0; i < NUM_SOUNDS; i++) {
		sounds_playing.push_back(false);
		sound_ptrs.push_back(nullptr);
	}
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
	// 150 600
	// 800 850
	// if (Mode::current.get() != this) return false;
	bool left_click = false; 
	bool right_click = false;
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_i && !story_state.in_cutscene) {
			if (inventory_visible) inventory_status = ShowItem;
			inventory_visible = !inventory_visible;
			inventory.update_inventory();
		} else {
			if (evt.key.keysym.sym == SDLK_PAGEUP) {
				if (inventory_cur_page > 0) {
					inventory_cur_page --;
				}
			} else if (evt.key.keysym.sym == SDLK_PAGEDOWN) {
				if (inventory_cur_page * 4 + 4 < inventory.interactables.size()) {
					inventory_cur_page ++;
				}
			}
			if (inventory_status == ShowItem && inventory.interactables.size() > 0) {
				if (evt.key.keysym.sym == SDLK_1) {
					item_selected_ID = inventory_cur_page * 4 + 0;
					inventory_status = ShowDetail;
				} else if (evt.key.keysym.sym == SDLK_2) {
					item_selected_ID = inventory_cur_page * 4 + 1;
					inventory_status = ShowDetail;
				}  else if (evt.key.keysym.sym == SDLK_3) {
					item_selected_ID = inventory_cur_page * 4 + 2;
					inventory_status = ShowDetail;
				}  else if (evt.key.keysym.sym == SDLK_4) {
					item_selected_ID = inventory_cur_page * 4 + 3;
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
			if (inventory_status == ShowDetail) {
				inventory_status = ShowItem;
			} else if (inventory_status == UseItem) {
				inventory_status = ShowDetail;
			}
		} else if (evt.button.button == SDL_BUTTON_LEFT) {
			left_click = true;
		} else if (evt.button.button == SDL_BUTTON_MIDDLE && !story_state.in_cutscene) {
			if (!inventory_visible) {
				inventory_visible ^= 1;
			} else if (inventory_status == ShowItem) {
				inventory_visible = false;
			}
		}
		if (inventory_visible && inventory_status == ShowItem) {
			if (check_mouseWithItem(left_click, right_click)) return false;
		}
		if (message_box_visible || (inventory_visible && inventory_status != ShowDetail)) { left_click = false; right_click = false; }
		if (message_box_visible) {
			message_box.erase(message_box.begin());
			if (message_box.size() == 0) {
				message_box_visible = false;
			}
		}
	} else if (evt.type == SDL_MOUSEWHEEL) {
        if (evt.wheel.y < 0) { // scroll up 
			if (inventory_cur_page * 4 + 4 < inventory.interactables.size()) {
				inventory_cur_page ++;
			}
        }
        else if (evt.wheel.y > 0) { // scroll down
             // Put code for handling "scroll down" here!
			if (inventory_cur_page > 0) {
				inventory_cur_page --;
			}
        }

        if(evt.wheel.x < 0) { // scroll right
			if (inventory_cur_page > 0) {
				inventory_cur_page --;
			}
        }
        else if(evt.wheel.x > 0) { // scroll left
			if (inventory_cur_page * 4 + 4 < inventory.interactables.size()) {
				inventory_cur_page ++;
			}
        }
    }
	if (inventory_visible) {
		check_mouseWithItem(left_click, right_click);
	}

	check_mouse(left_click, right_click);
	return false;
}

bool StoryMode::check_mouseWithItem(bool left_click, bool right_click) {
	on_item = false;
	if (inventory_status == ShowItem) {
		for (int i = 0; i < 4 && i < (inventory.interactables.size() - inventory_cur_page * 4); i ++) {
			if (in_box(mouse_pos, item_pos[i][0], item_pos[i][1])) {
				hint_visible = true;
				on_item = true;
				if (left_click) {
					item_selected_ID = inventory_cur_page * 4 + i;
					inventory_status = ShowDetail;
				}
				return true;
			}
		}
	}
	return false;
}


void StoryMode::check_usage(itemID use, itemID on, bool click) {

	if (!click) return;

	if (use == brokenGlass && on == commanderBody && !control_room.control_state.accessed_terminal) {
		message_box.push_back("Why the fuck would I do that? ");
	}
	else if (use == brokenGlass && on == commanderBody && control_room.control_state.accessed_terminal) {
		message_box.push_back(". . . I'm sorry, commander. ");
		//Cutting up noises
		inventory.interactables.push_back(cabin_room.Commander_finger);
		cabin_room.cabin_state.took_finger = true;
	}
	else if (use == brokenGlass && on == commanderBody && cabin_room.cabin_state.took_finger) {
		message_box.push_back("I won't mutilate the Commander twice. ");
		//Cutting up noises
	}
	else if (use == commanderFinger && on == controlTerminal && !control_room.control_state.commander_bio) {
		control_room.control_state.commander_bio = true;
		message_box.push_back("Okay, I should be able to unlock the door now. ");
	} 
	else if (use == commanderFinger && control_room.control_state.commander_bio) {
		message_box.push_back(". . . why am I still holding on to this?");
		inventory.erase_item(commanderFinger);
	}
	else if (use == killerFinger && on == controlTerminal && !control_room.control_state.diary_bio) {
		control_room.control_state.diary_bio = true;
		message_box.push_back("There might be something important in there. I should take a look when I can. ");
	}
	else if (use == killerFinger && control_room.control_state.diary_bio) {
		message_box.push_back(". . . why am I still holding on to this?");
		inventory.erase_item(killerFinger);
	}
	else if (use == brokenGlass && on == controlBody && !control_room.control_state.accessed_terminal) {
		message_box.push_back(". . . it isn't right. Even after what he did. ");
	}
	else if (use == brokenGlass && on == controlBody && control_room.control_state.accessed_terminal) {
		message_box.push_back(". . . guess I have to do this. ");
		//Cutting up noises
		inventory.interactables.push_back(control_room.Killer_finger);
		control_room.control_state.took_finger = true;
	}
	else if (use == brokenGlass && on == controlBody && control_room.control_state.took_finger) {
		message_box.push_back("No. . . I'm not doing that again. ");
		//Cutting up noises
	}
	else if (use == keyCard && on == controlTerminal && !control_room.control_state.used_key_card) {
		control_room.control_state.used_key_card = true;
		message_box.push_back("I've unlocked the terminal. I should be able to use it now. ");
		//For the sequel
	}
	else if (use == keyCard && on == controlTerminal && control_room.control_state.used_key_card) {
		message_box.push_back("Swiping again didn't do anaything. ");
		//For the sequel
	}
	else if (use == crowbar && on == toolbox) {
		message_box.push_back("This should work. . . but not right now! ");
		//For the sequel
	}
	else {
		message_box.push_back("That didn't work. ");
	}
}

void StoryMode::update(float elapsed) {


	if (story_state.played_opening && (!background_music || background_music->stopped)) {
		background_music = Sound::play(*ambience, 1.0f);
	}

	std::pair<std::vector<soundID>, std::vector<soundID>> pair;

	//Check story
	switch (location) {

	case Cabin:
		pair = cabin_room.check_story(message_box, sounds_playing, elapsed);
		if (cabin_room.cabin_state.intro_text) {
			story_state.played_opening = true;
			story_state.in_cutscene = false;
		}
		break;

	case Hallway1:
		hallwayone.check_story(message_box);
		break;

	case Control:
		control_room.check_story(message_box);

	}

	check_sounds(pair.first, pair.second);

	if (message_box.size() != 0) message_box_visible = true;
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
	//std::cout << left_click << " " << right_click << "\n";
	//std::cout << mouse_pos.x << " " << mouse_pos.y << "\n";

	if (on_item || story_state.in_cutscene)
		return;

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
				if (inventory_status == ShowDetail) {
					check_usage(inventory.interactables[item_selected_ID].id, current.id, left_click);
				} else {
					cabin_room.check_interactions(message_box, left_click, right_click, current.id, inventory, location);
				}
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
				if (inventory_status == ShowDetail) {
					check_usage(inventory.interactables[item_selected_ID].id, current.id, left_click);
				}
				else {
					hallwayone.check_interactions(message_box, left_click, right_click, current.id, inventory, location);
				}
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
				if (inventory_status == ShowDetail) {
					check_usage(inventory.interactables[item_selected_ID].id, current.id, left_click);
				}
				else {
					activate_terminal = control_room.check_interactions(message_box, left_click, right_click, current.id, inventory, location);
				}
				if (activate_terminal) {
					std::shared_ptr< TerminalMode > terminal = std::make_shared< TerminalMode >();
					terminal->shared_from = shared_from_this();
					terminal->log_permission = control_room.control_state.diary_bio;
					terminal->door_permission = control_room.control_state.commander_bio;
					Mode::set_current(terminal);
					std::cout << hallwayone.hallwayone_state.door_3_open << std::endl;
				}
			}
		}
	}

	if(message_box.size() != 0) message_box_visible = true;
	if (!on_something || message_box_visible || (inventory_visible && inventory_status != ShowDetail)) hint_visible = on_item;
}

void StoryMode::check_sounds(std::vector<soundID> to_play, std::vector<soundID> to_kill) {

	auto sid_to_i = [](soundID id) {
		switch (id) {
		case SfingerOne:
			return 0;
		case SfingerTwo:
			return 1;
		case Semergency:
			return 2;
		case Sambience:
			return 3;
		case Sdoor_air:
			return 4;
		case Sdoor_open:
			return 5;
		case ScrowbarOne:
			return 6;
		case ScrowbarTwo:
			return 7;
		case SshipCrash:
			return 8;
		default:
			return -1;
		}
	};

	int index = -1;

	for (int i = 0; i < to_play.size(); i++) {
		index = sid_to_i(to_play[i]);
		if (sounds_playing[index]) {
			continue;
		}
		sounds_playing[index] = true;
		switch (to_play[i]) {
			case SfingerOne:
				sound_ptrs[index] = Sound::play(*finger_one, 1.0f);
				break;
			case SfingerTwo:
				sound_ptrs[index] = Sound::play(*finger_two, 1.0f);
				break;
			case Semergency:
				sound_ptrs[index] = Sound::play(*emergency, 1.0f);
				break;
			case Sambience:
				sound_ptrs[index] = Sound::play(*ambience, 1.0f);
				break;
			case Sdoor_air:
				sound_ptrs[index] = Sound::play(*door_air, 1.0f);
				break;
			case Sdoor_open:
				sound_ptrs[index] = Sound::play(*door_open, 1.0f);
				break;
			case ScrowbarOne:
				sound_ptrs[index] = Sound::play(*crowbar_one, 1.0f);
				break;
			case ScrowbarTwo:
				sound_ptrs[index] = Sound::play(*crowbar_two, 1.0f);
				break;
			case SshipCrash:
				sound_ptrs[index] = Sound::play(*ship_crash, 1.0f);
				break;
		}
	}

	for (int i = 0; i < to_kill.size(); i++) {
		index = sid_to_i(to_kill[i]);
		if (sound_ptrs[index]) { sound_ptrs[index]->stop(5.0f); }
		sounds_playing[index] = false;
	}

	for (int i = 0; i < sound_ptrs.size(); i++) {
		if (sound_ptrs[i] && sound_ptrs[i]->stopped) {
			sounds_playing[i] = false;
		}
	}
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
				draw.draw(*sprite_light_body2, ul + floating_animation * glm::vec2(0.5, 0.5));
				draw.draw(*sprite_light_upper_glass, ul + floating_animation);
			}
			else {
				draw.draw(*sprite_dark_cabin, ul);
				draw.draw(*sprite_dark_glass, ul);
			}
			if (!story_state.played_opening) {
				draw.draw(*all_black, ul);
			}
		} else if (location == Hallway1) {
			draw.draw(*hallwayone_door_red, ul);
		} else if (location == Control) {
			draw.draw(*control_bg, ul);
			draw.draw(*control_fg, ul);
			if (!control_room.control_state.picked_up_crowbar) {
				draw.draw(*control_crowbar, ul); //Only if haven't picked up yet
			}
			draw.draw(*control_blood, ul);
		}

		if (inventory_visible || message_box_visible) {
			draw.draw(*demo_text_area, ul);
		}

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

		int dis_between_text_lines = 60;
		float old_f = 3.0f;
		float new_f = 3.0f;
		if (message_box_visible){
			//Hacky way of using the auto-wrap code without creating a Sentence object
			//for each message I want to print
			int current_chr = -1; 
			std::unordered_map <size_t, int> fit_list;
			draw_text.draw_text(message_box[0], at, FONT_SIZE, glm::u8vec4(0xff, 0xff, 0xff, 0xff), current_chr, 1, fit_list, textbox_left.x + 20, textbox_right.x - 20);
		} else if (inventory_visible) {
			inventory.update_inventory();
			if (inventory_status == ShowItem) {
				glm::vec2 page_at = at + glm::vec2(1450, -dis_between_text_lines * 3);
				std::string tmp = "";
				if (inventory.interactables.size() > 0) {
					int page_max = (int) ((inventory.interactables.size() + 3.0) / 4.0);
					tmp.append(std::to_string(inventory_cur_page + 1));
					tmp.append(" / ");
					tmp.append(std::to_string(page_max));
					draw_text.get_text_extents(tmp, page_at, old_f, &min, &max);
					draw_text.draw_text_short(tmp, page_at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				}
				
				for (int i = inventory_cur_page * 4; i < std::min((int) inventory.to_output.size(), (int) inventory_cur_page * 4 + 4); i++) {
					// std::cout<<i<<"\n";
					tmp = "";
					// if (inventory.interactables.size() > 0) {
					// 	tmp.append("[");
					// 	tmp.append(std::to_string(i + 1));
					// 	tmp.append("] ");
					// }
					tmp.append(inventory.to_output[i]);
					draw_text.get_text_extents(tmp, at, old_f, &min, &max);
					draw_text.draw_text_short(tmp, at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
					at.y -= dis_between_text_lines;
				}
			} else if (inventory_status == ShowDetail) {
				if (item_selected_ID >= inventory.interactables.size()) {
					item_selected_ID = -1;
					inventory_status = ShowItem;
				} else {
					Interactable cur_item = inventory.interactables[item_selected_ID];
					std::string tmp = cur_item.name;
					tmp.append(". ");
					tmp.append(cur_item.description);
					draw_text.get_text_extents(tmp, at, old_f, &min, &max);
					draw_text.draw_text_short(tmp, at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
					at.y -= dis_between_text_lines;
					tmp = "[Left Click] Use it";
					draw_text.get_text_extents(tmp, at, old_f, &min, &max);
					draw_text.draw_text_short(tmp, at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
					at.y -= dis_between_text_lines;
					tmp = "[Right Click] Back to the item list";
					draw_text.get_text_extents(tmp, at, old_f, &min, &max);
					draw_text.draw_text_short(tmp, at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
					at.y -= dis_between_text_lines;
				}
			} else if (inventory_status == UseItem) {
				Interactable cur_item = inventory.interactables[item_selected_ID];
				std::string tmp = cur_item.name;
				tmp.append(" is selected.");
				draw_text.get_text_extents(tmp, at, old_f, &min, &max);
				draw_text.draw_text_short(tmp, at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= dis_between_text_lines;
				tmp = "Try to do something...";
				draw_text.get_text_extents(tmp, at, old_f, &min, &max);
				draw_text.draw_text_short(tmp, at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= dis_between_text_lines;
				tmp = "[Left Click] Use it on something";
				draw_text.get_text_extents(tmp, at, old_f, &min, &max);
				draw_text.draw_text_short(tmp, at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= dis_between_text_lines;
				tmp = "[Right Click] Back to the item list";
				draw_text.get_text_extents(tmp, at, old_f, &min, &max);
				draw_text.draw_text_short(tmp, at, new_f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= dis_between_text_lines;
			} else {
				item_selected_ID = -1;
			}
		}
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}

const double eps = 1e-6;
const double PI = acos(-1);

int dcmp(double x) {
    if (fabs(x) < eps) return 0;
    else
        return x < 0 ? -1 : 1;
}

bool OnSegment(glm::vec2 P1, glm::vec2 P2, glm::vec2 Q) {
	glm::vec2 r1 = P1 - Q;
	glm::vec2 r2 = P2 - Q;
	double r3 = r1.x * r2.y - r1.y * r2.x;
	double r4 = r1.x * r2.x + r1.y + r2.y;
    return dcmp(r3) == 0 && dcmp(r4) <= 0;
}

bool InPolygon(glm::vec2 P, glm::vec2 polygon[], int n) {
    bool flag = false;
    glm::vec2 P1, P2;
    for(int i = 1, j = n; i <= n; j = i++)
    {
        P1 = polygon[i];
        P2 = polygon[j];
        if(OnSegment(P1, P2, P)) return true;
        if((dcmp(P1.y - P.y) > 0 != dcmp(P2.y - P.y) > 0) && dcmp(P.x - (P.y - P1.y) * (P1.x - P2.x) / (P1.y - P2.y) - P1.x) < 0)
            flag = !flag;
    }
    return flag;
}