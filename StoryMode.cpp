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
	SpriteAtlas const *ret = new SpriteAtlas(data_path("computer"));

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

	return ret;
});

Load< Sound::Sample > music_cold_dunes(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("bgm.wav"));
});

Load< Sound::Sample > music_correct(LoadTagDefault, []() -> Sound::Sample *{
	return new Sound::Sample(data_path("correct.wav"));
});

Load< Sound::Sample > music_wrong(LoadTagDefault, []() -> Sound::Sample *{
	return new Sound::Sample(data_path("wrong.wav"));
});

StoryMode::StoryMode() {
}

StoryMode::~StoryMode() {
	Sound::stop_all_samples();
}

bool StoryMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	// if (Mode::current.get() != this) return false;
	std::cout<<"storymode\n";
	// if (Mode::current.get() != this) return false;
	std::cout<<evt.type<<" evt_type\n";
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
	if (correct) {
		Sound::play(*music_correct, 1.0f);
		correct = false;
	}
	if (wrong) {
		Sound::play(*music_wrong, 1.0f);
		wrong = false;
	}
}

void StoryMode::enter_scene() {
	//just entered this scene, adjust flags and build menu as appropriate:
	std::vector< MenuMode::Item > items;
	glm::vec2 at(3.0f, view_max.y - 13.0f);
	auto add_text = [&items,&at](Sprite const *text) {
		assert(text);
		items.emplace_back("");
		at.y -= text->max_px.y - text->min_px.y;
		at.y -= 4.0f;
	};
	auto add_choice = [&items,&at](Sprite const *text, std::function< void(MenuMode::Item const &) > const &fn) {
		assert(text);
		items.emplace_back("");
		at.y -= text->max_px.y - text->min_px.y;
		at.y -= 4.0f;
	};
	
	if (location == Room) {
		add_text(room_text);
		at.y -= 28.0f; //gap before choices
		add_choice(go_cpu_text, [this](MenuMode::Item const &){
			location = Store;
			Type = CPU;
			Mode::current = shared_from_this();
		});
		at.y -= 8.0f; //gap before choices
		add_choice(go_gpu_text, [this](MenuMode::Item const &){
			location = Store;
			Type = GPU;
			Mode::current = shared_from_this();
		});
		at.y -= 8.0f; //gap before choices
		add_choice(go_pl_text, [this](MenuMode::Item const &){
			location = Store;
			Type = PL;
			Mode::current = shared_from_this();
		});
		if(build_res == false) {
			at.y -= 8.0f; //gap before choices
			add_choice(build_text, [this](MenuMode::Item const &){
				if(have_CPU && have_GPU && have_PL) {
					have_CPU = have_GPU = have_PL = false;
					have_PC = true;
					build_res = true;
					correct = true;
				} else {
					build_res = false;
					wrong = true;
				}
				Mode::current = shared_from_this();
			});
			add_text(warn_text);
		} else {
			add_text(success_text);
			correct = true;
		}
	} else {
		if (Type == CPU) {
			add_choice(buy_cpu_text, [this](MenuMode::Item const &){
				location = Room;
				if (have_CPU) {
					wrong = true;
				} else {
					correct = true;
				}
				have_CPU = true;
				Mode::current = shared_from_this();
			});
		} else if (Type == GPU) {
			add_choice(buy_gpu_text, [this](MenuMode::Item const &){
				location = Room;
				if (have_GPU) {
					wrong = true;
				} else {
					correct = true;
				}
				have_GPU = true;
				Mode::current = shared_from_this();
			});

		} else {
			add_choice(buy_pl_text, [this](MenuMode::Item const &){
				location = Room;
				if (have_PL) {
					wrong = true;
				} else {
					int tmp_rand = rand() % 3;
					if(tmp_rand == 0) {
						correct = true;
						have_PL = true;
					} else {
						wrong = true;
					}
				}
				Mode::current = shared_from_this();
			});
		}
		at.y -= 8.0f; //gap before choices
		add_choice(return_room_text, [this](MenuMode::Item const &){
			location = Room;
			Mode::current = shared_from_this();
		});
		
	}
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
		
		if (location == Room) {
			draw.draw(*sprite_main_bg, ul);
			if (have_CPU) {
				draw.draw(*sprite_cpu_bg, ul);
			}
			if (have_GPU) {
				draw.draw(*sprite_gpu_bg, ul);
			}
			// if (have_PL) {
			// 	draw.draw(*sprite_pl_bg, ul);
			// }
			if (build_res) {
				draw.draw(*sprite_pc_bg, ul);
			}
		}
		else if (location == Store) {
			if (move_remained > 0 && dir != 0) {
				draw.draw(*sprite_black_bg, ul);
				if (dir == 1) {
					ast_y++;
				} else if (dir == 2) {
					ast_y--;
				} else if (dir == 3) {
					ast_x--;
				} else if (dir == 4) {
					ast_x++;
				}
				ul = glm::vec2(view_min.x + ast_x, view_max.y + ast_y);
				draw.draw(*sprite_astronaut_bg, ul);
				move_remained--;
			} else {
				dir = 0;
				move_remained = 100;
				draw.draw(*sprite_black_bg, ul);
				ul = glm::vec2(view_min.x + ast_x, view_max.y + ast_y);
				draw.draw(*sprite_astronaut_bg, ul);
			}
		}
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}
