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
Sprite const *sprite_main_bg = nullptr;
Sprite const *sprite_store_bg = nullptr;
Sprite const *sprite_base_bg = nullptr;
Sprite const *sprite_black_bg = nullptr;
Sprite const *sprite_cpu_bg = nullptr;
Sprite const *sprite_gpu_bg = nullptr;
Sprite const *sprite_pl_bg = nullptr;
Sprite const *sprite_pc_bg = nullptr;

Sprite const *sprite_astronaut_bg = nullptr;
Sprite const *sprite_key_bg = nullptr;

Sprite const *room_text = nullptr;
Sprite const *return_room_text = nullptr;
Sprite const *exit_text = nullptr;
Sprite const *go_gpu_text = nullptr;
Sprite const *go_cpu_text = nullptr;
Sprite const *go_pl_text = nullptr;
Sprite const *buy_gpu_text = nullptr;
Sprite const *buy_cpu_text = nullptr;
Sprite const *buy_pl_text = nullptr;
Sprite const *build_text = nullptr;
Sprite const *warn_text = nullptr;
Sprite const *success_text = nullptr;

Load< SpriteAtlas > sprites(LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const *ret = new SpriteAtlas(data_path("space"));

	sprite_left_select = &ret->lookup("text-select-left");

	sprite_main_bg = &ret->lookup("main-bg");
	sprite_store_bg = &ret->lookup("mall-bg");
	sprite_base_bg = &ret->lookup("base-bg");
	sprite_black_bg = &ret->lookup("black-bg");
	sprite_cpu_bg = &ret->lookup("cpu-bg");
	sprite_gpu_bg = &ret->lookup("gpu-bg");
	sprite_pl_bg = &ret->lookup("power-bg");
	sprite_pc_bg = &ret->lookup("pc-bg");

	sprite_astronaut_bg = &ret->lookup("astronaut-bg");
	sprite_key_bg = &ret->lookup("key-bg");

	room_text = &ret->lookup("room-text");
	return_room_text = &ret->lookup("return-room-text");
	exit_text = &ret->lookup("exit-text");
	go_gpu_text = &ret->lookup("go-gpu-text");
	go_cpu_text = &ret->lookup("go-cpu-text");
	go_pl_text = &ret->lookup("go-power-text");
	buy_gpu_text = &ret->lookup("buy-gpu-text");
	buy_cpu_text = &ret->lookup("buy-cpu-text");
	buy_pl_text = &ret->lookup("buy-power-text");
	build_text = &ret->lookup("build-text");
	warn_text = &ret->lookup("warn-text");
	success_text = &ret->lookup("success-text");

	demo_background = &ret->lookup("demo_screen_background");
	demo_text_area = &ret->lookup("textbox_area");

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
	// std::cout<<"storymode\n";
	// std::cout<<evt.type<<" evt_type\n";
	if (evt.type == SDL_KEYDOWN) {
		if (dir != 0) return true;;
		if (evt.key.keysym.sym == SDLK_UP) {
			dir = 1;
			move_remained = 100;
			// return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			dir = 2;
			move_remained = 100;
			// return true;
		} else if (evt.key.keysym.sym == SDLK_RETURN) {
			Mode::set_current(std::make_shared< TerminalMode >());
		} else if (evt.key.keysym.sym == SDLK_LEFT) {
			dir = 3;
			move_remained = 100;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			dir = 4;
			move_remained = 100;
		} else if (evt.key.keysym.sym == SDLK_i) {
			inventory_visible = !inventory_visible;
			inventory.update_inventory();
		} else if (evt.key.keysym.sym == SDLK_m) {
			message_box_visible = 200;
		}
	}
	return false;
}

void StoryMode::update(float elapsed) {
	if (Mode::current.get() == this) {
		//there is no menu displayed! Make one:
		enter_scene();
	}
	if (!background_music || background_music->stopped) {
		background_music = Sound::play(*music_cold_dunes, 1.0f);
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
		draw.draw(*sprite_astronaut_bg, ul);
		if (have_key) {
			glm::vec2 col_key = glm::vec2(view_min.x + 500, view_max.y + 200);
			draw.draw(*sprite_key_bg, col_key);
			if (abs(col_key.x - ul.x) < 40 && abs(col_key.y - ul.y) < 20) {
				Sound::play(*music_correct, 1.0f);
				have_key = false;
				inventory.interactables.push_back(key);
				message_box.emplace_back("A new item is added to your inventory!");
				message_box_visible = 200;
			}
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
			for (int i = 0; i < message_box.size(); i++) {
				draw_text.draw_text_short(message_box[i], at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				at.y -= 35;
			}
		} else if (inventory_visible) {
			inventory.update_inventory();
			for (int i = 0; i < inventory.to_output.size(); i++) {
				draw_text.get_text_extents(inventory.to_output[i], at, 1.0f, &min, &max);
				draw_text.draw_text_short(inventory.to_output[i], at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				if (inventory.interactables.size() > 0) {
					at.x += min.x;
					draw_text.draw_text_short(": " + inventory.interactables[i].description, at, 3.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
				}
				at.y -= 35;
			}
		}
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}
