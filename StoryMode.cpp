#include "StoryMode.hpp"

#include "Sprite.hpp"
#include "DrawSprites.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "gl_errors.hpp"
#include "MenuMode.hpp"
#include "Sound.hpp"
#include "Text.hpp"

Sprite const* sprite_left_select = nullptr;
Sprite const* sprite_right_select = nullptr;

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

Load< SpriteAtlas > sprites(LoadTagDefault, []() -> SpriteAtlas const* {

	SpriteAtlas const* ret = new SpriteAtlas(data_path("the-planet"));

	terminal_background = &ret->lookup("screen-background");
	terminal_screen = &ret->lookup("screen");

	sprite_left_select = &ret->lookup("text-select-left");
	sprite_right_select = &ret->lookup("text-select-right");

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

Load< Sound::Sample > music_cold_dunes(LoadTagDefault, []() -> Sound::Sample* {
	return new Sound::Sample(data_path("cold-dunes.opus"));
	});

StoryMode::StoryMode() {
}

StoryMode::~StoryMode() {
}

bool StoryMode::handle_event(SDL_Event const&, glm::uvec2 const& window_size) {
	if (Mode::current.get() != this) return false;

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
	glm::vec2 at = screen_left; 
	at.y = view_max.y - screen_left.y;

	auto get_number_sentences = [](std::string const& sentence) {
		Sprite const* chr;
		float combined_width = 0.0;
		for (size_t pos = 0; pos < sentence.size(); pos++) {
			chr = &sprites->lookup(sentence.substr(pos, 1));
			combined_width += (chr->max_px.x - chr->min_px.x) * FONT_SIZE;
		}

		return ceil(combined_width / 1000.0f);
	};

	auto add_text = [this, &items, &at, &get_number_sentences](Sentence const* sentence) {
		items.emplace_back(sentence->text,
			nullptr,
			FONT_SIZE,
			0,
			sentence->wait_to_print,
			glm::vec4(0xff, 0xff, 0xff, 0xff),
			nullptr,
			at);
		at.x = screen_left.x;
		at.y -= LINE_SKIP * get_number_sentences(sentence->text) * FONT_SIZE;
		at.y -= LINE_SKIP * FONT_SIZE;
	};

	Sentence const test = Sentence("Something remains to accomplish. I won't leave", 1.0f);
	Sentence const test2 = Sentence("Something remains to accomplish. I won't leave", 1.0f);
	add_text(&test);
	add_text(&test2);

	std::shared_ptr< MenuMode > menu = std::make_shared< MenuMode >(items);
	menu->atlas = sprites;
	menu->left_select = sprite_left_select;
	menu->right_select = sprite_right_select;
	menu->view_min = view_min;
	menu->view_max = view_max;
	menu->background = shared_from_this();
	Mode::current = menu;
}

void StoryMode::draw(glm::uvec2 const& drawable_size) {
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
		draw.draw(*terminal_background, ul);
		draw.draw(*terminal_screen, ul);
	}
	GL_ERRORS(); //did the DrawSprites do something wrong?
}
