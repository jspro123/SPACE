#include "TerminalMode.hpp"
// #include "StoryMode.hpp"
#include "Sprite.hpp"
#include "DrawSprites.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "gl_errors.hpp"
#include "MenuMode.hpp"
#include "StoryMode.hpp"
#include "Sound.hpp"

Sprite const* sprite_left_select_terminalmode = nullptr;
Sprite const* sprite_right_select_terminalmode = nullptr;

Sprite const* sprite_dunes_bg = nullptr;
Sprite const* sprite_dunes_traveller = nullptr;
Sprite const* sprite_dunes_ship = nullptr;

Sprite const* sprite_oasis_bg = nullptr;
Sprite const* sprite_oasis_traveller = nullptr;
Sprite const* sprite_oasis_missing = nullptr;

Sprite const* sprite_hill_bg = nullptr;
Sprite const* sprite_hill_traveller = nullptr;
Sprite const* sprite_hill_missing = nullptr;

Sprite const* terminal_background = nullptr;
Sprite const* terminal_screen = nullptr;

Load< SpriteAtlas > terminal(LoadTagDefault, []() -> SpriteAtlas const* {
	SpriteAtlas const* ret = new SpriteAtlas(data_path("the-planet"));
	terminal_background = &ret->lookup("screen-background");
	terminal_screen = &ret->lookup("screen");
	return ret;
	});

Load< SpriteAtlas > sprites_terminal(LoadTagDefault, []() -> SpriteAtlas const* {

	SpriteAtlas const* ret = new SpriteAtlas(data_path("the-planet"));

	terminal_background = &ret->lookup("screen-background");
	terminal_screen = &ret->lookup("screen");

	sprite_left_select_terminalmode = &ret->lookup("text-select-left");
	sprite_right_select_terminalmode = &ret->lookup("text-select-right");

	sprite_dunes_bg = &ret->lookup("dunes-bg");
	sprite_dunes_traveller = &ret->lookup("dunes-traveller");
	sprite_dunes_ship = &ret->lookup("dunes-ship");

	sprite_oasis_bg = &ret->lookup("oasis-bg");
	sprite_oasis_traveller = &ret->lookup("oasis-traveller");
	sprite_oasis_missing = &ret->lookup("oasis-missing");

	sprite_hill_bg = &ret->lookup("hill-bg");
	sprite_hill_traveller = &ret->lookup("hill-traveller");
	sprite_hill_missing = &ret->lookup("hill-missing");

	return ret;
	});

Load< Sound::Sample > music_cold_dunes_terminal(LoadTagDefault, []() -> Sound::Sample* {
	return new Sound::Sample(data_path("cold-dunes.opus"));
	});

TerminalMode::TerminalMode() {
}

TerminalMode::~TerminalMode() {
	Sound::stop_all_samples();
}

bool TerminalMode::handle_event(SDL_Event const &evt, glm::uvec2 const& window_size) {
	// if (Mode::current.get() != this) return false;
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_BACKSPACE) {
			Mode::set_current(std::make_shared< StoryMode >());
		}
	}

	return false;
}

void TerminalMode::update(float elapsed) {
	if (Mode::current.get() == this) {
		//there is no menu displayed! Make one:
		enter_scene();
	}

	if (!background_music || background_music->stopped) {
		background_music = Sound::play(*music_cold_dunes_terminal, 1.0f);
	}
}

void TerminalMode::enter_scene() {
	//just entered this scene, adjust flags and build menu as appropriate:
	std::vector< MenuMode::Item > items;
	glm::vec2 at = screen_left; 
	at.y = view_max.y - screen_left.y;

	auto get_number_sentences = [this](std::string const& sentence) {
		Sprite const* chr;
		float combined_width = 0.0;
		for (size_t pos = 0; pos < sentence.size(); pos++) {
			chr = &sprites_terminal->lookup(sentence.substr(pos, 1));
			combined_width += (chr->max_px.x - chr->min_px.x) * FONT_SIZE;
		}

		return ceil(combined_width / (screen_right.x - screen_left.x));
	};

	auto add_text = [this, &items, &at, &get_number_sentences](Sentence const* sentence) {
		items.emplace_back(sentence->text,
			nullptr,
			FONT_SIZE,
			0,
			sentence->wait_to_print,
			sentence->drawing_speed,
			screen_left.x,
			screen_right.x,
			glm::vec4(0xff, 0xff, 0xff, 0xff),
			nullptr,
			at);
		at.x = screen_left.x;
		at.y -= LINE_SKIP * get_number_sentences(sentence->text) * FONT_SIZE;
		at.y -= LINE_SKIP * FONT_SIZE;
	};

	auto add_choice = [this, &items, &at, &get_number_sentences](Sentence const* sentence, std::function< void(MenuMode::Item const&) > const& fn) {
		items.emplace_back(sentence->text,
			nullptr,
			FONT_SIZE,
			0,
			sentence->wait_to_print,
			sentence->drawing_speed,
			screen_left.x,
			screen_right.x,
			glm::vec4(0xff, 0xff, 0xff, 0xff),
			fn,
			at);
		at.x = screen_left.x;
		at.y -= LINE_SKIP * get_number_sentences(sentence->text) * FONT_SIZE;
		at.y -= LINE_SKIP * FONT_SIZE;
	};

	auto skip_lines = [&at]() {
		at.y -= LINE_SKIP * FONT_SIZE;
	};

	auto create_menu = [this, &add_choice]() {
		add_choice(&menu1, [this](MenuMode::Item const&) {
			terminal = DIAG;
			Mode::current = shared_from_this();
		});

		add_choice(&menu2, [this](MenuMode::Item const&) {
			terminal = DISENG;
			Mode::current = shared_from_this();
		});

		add_choice(&menu3, [this](MenuMode::Item const&) {
			terminal = LOG;
			Mode::current = shared_from_this();
		});

		add_choice(&menu4, [this](MenuMode::Item const&) {
			terminal = ENG;
			Mode::current = shared_from_this();
		});

		add_choice(&menu5, [this](MenuMode::Item const&) {
			terminal = EXIT;
			Mode::current = shared_from_this();
		});
	};

	displayed_boot = true;
	if (!displayed_boot) {
		//Do something
		return;
	}

	if (terminal == MENU) {
		create_menu();
	} else if (terminal == DIAG) {
		add_text(&diag1);
		add_text(&diag2);
		add_text(&diag3);
		add_text(&diag4);
		add_text(&lines);
		create_menu();
	} else if (terminal == ENG) {
		add_text(&eng1);
		add_text(&eng2);
		add_text(&lines);
		create_menu();
	} else if (terminal == LOG) {
		add_choice(&log1_1, [this](MenuMode::Item const&) {
			terminal = LOG1;
			Mode::current = shared_from_this();
			});
		add_choice(&log2_1, [this](MenuMode::Item const&) {
			terminal = LOG2;
			Mode::current = shared_from_this();
			});
		add_choice(&log3_1, [this](MenuMode::Item const&) {
			terminal = LOG3;
			Mode::current = shared_from_this();
			});
		add_choice(&log4_1, [this](MenuMode::Item const&) {
			terminal = LOG4;
			Mode::current = shared_from_this();
			});
		add_choice(&log5_1, [this](MenuMode::Item const&) {
			terminal = LOG5;
			Mode::current = shared_from_this();
			});
		add_choice(&log6_1, [this](MenuMode::Item const&) {
			terminal = LOG6;
			Mode::current = shared_from_this();
			});
		add_choice(&log7_1, [this](MenuMode::Item const&) {
			terminal = LOG7;
			Mode::current = shared_from_this();
			});
		add_choice(&back, [this](MenuMode::Item const&) {
			terminal = MENU;
			Mode::current = shared_from_this();
			});
	} else if (terminal == EXIT) {
		//Return to control room
		create_menu();
	} else if (terminal == DISENG) {
		add_text(&diseng1);
		add_text(&diseng2);
		add_text(&lines);
		create_menu();
	} else if (terminal == LOG1) {
		add_text(&log1_1);
		add_text(&lines);
		add_text(&log1_2);
		add_text(&log1_3);
		add_text(&log1_4);
		add_choice(&back, [this](MenuMode::Item const&) {
			terminal = LOG;
			Mode::current = shared_from_this();
			});
	} else if (terminal == LOG2) {
		add_text(&log2_1);
		add_text(&lines);
		add_text(&log2_2);
		add_text(&log2_3);
		add_choice(&back, [this](MenuMode::Item const&) {
			terminal = LOG;
			Mode::current = shared_from_this();
			});
	} else if (terminal == LOG3) {
		add_text(&log3_1);
		add_text(&lines);
		add_text(&log3_2);
		add_text(&log3_3);
		add_text(&log3_4);
		add_choice(&back, [this](MenuMode::Item const&) {
			terminal = LOG;
			Mode::current = shared_from_this();
			});
	} else if (terminal == LOG4) {
		add_text(&log4_1);
		add_text(&lines);
		add_text(&log4_2);
		add_text(&log4_3);
		add_choice(&back, [this](MenuMode::Item const&) {
			terminal = LOG;
			Mode::current = shared_from_this();
			});
	} else if (terminal == LOG5) {
		add_text(&log5_1);
		add_text(&lines);
		add_text(&log5_2);
		add_text(&log5_3);
		add_text(&log5_4);
		add_choice(&back, [this](MenuMode::Item const&) {
			terminal = LOG;
			Mode::current = shared_from_this();
			});
	} else if (terminal == LOG6) {
		add_text(&log6_1);
		add_text(&lines);
		add_text(&log6_2);
		add_text(&log6_3);
		add_choice(&back, [this](MenuMode::Item const&) {
			terminal = LOG;
			Mode::current = shared_from_this();
			});
	} else if (terminal == LOG7) {
		add_text(&log7_1);
		add_text(&lines);
		add_text(&log7_2);
		add_choice(&back, [this](MenuMode::Item const&) {
			terminal = LOG;
			Mode::current = shared_from_this();
			});
	}

	std::shared_ptr< MenuMode > menu = std::make_shared< MenuMode >(items);
	menu->atlas = sprites_terminal;
	menu->left_select = sprite_left_select_terminalmode;
	menu->right_select = sprite_right_select_terminalmode;
	menu->view_min = view_min;
	menu->view_max = view_max;
	menu->background = shared_from_this();
	Mode::current = menu;
}

void TerminalMode::draw(glm::uvec2 const& drawable_size) {
	//clear the color buffer:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	{ //use a DrawSprites to do the drawing:
		DrawSprites draw(*sprites_terminal, view_min, view_max, drawable_size, DrawSprites::AlignPixelPerfect);
		glm::vec2 ul = glm::vec2(view_min.x, view_max.y);
		draw.draw(*terminal_background, ul);
		draw.draw(*terminal_screen, ul);
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}
