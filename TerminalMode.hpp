
/*
 * StoryMode implements a story about The Planet of Choices.
 *
 */

#include "Mode.hpp"
#include "Sound.hpp"
#include <queue>
#include "Text.hpp"
#include <string>

struct TerminalMode : Mode {
	TerminalMode();
	virtual ~TerminalMode();

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//called to create menu for current scene:
	void enter_scene();


	/* ================== LOG TEXT 1 ================== */

	std::string s_log1_2 = "Recording this log before I enter my pod. Launch went smooth as butter. "
		"We've checked and double-checked and triple-checked that the ship's trajectory is right; "
		"if we're off by even one degree we'll probably never wake up.";

	std::string s_log1_3 = "Still pissed that they picked me. I told them I'm married, no one fucking cared. "
		"I told them I won't go, they tried to arrest me. Fuck that. How does a guy tell his family "
		"that he'll be gone for six years straight?";

	std::string s_log1_4 = "Commander's telling me it's time to go. . . Mary, I'm so sorry.";

	/* ================== LOG TEXT 2 ================== */

	std::string s_log2_2 = "Well, we're alive. That's nice. ";

	std::string s_log2_3 = "We've landed on the important piece of shit that they sent us to. Nothing here, "
		"just empty space and craters everywhere. Apparently the valuable stuff is somewhere under all this "
		"garbage. Commander says we'll be here for three months, just digging till we find something. "
		"We start tomorrow. Can't wait. ";

	/* ================== LOG TEXT 3 ================== */

	std::string s_log3_2 = "Been a week, still nothing. You'd think they would have given us a rock sensor or "
		"something with all the fancy digging equipment we got. ";

	std::string s_log3_3 = "I can't wait to leave, to be honest. It's been a rough week. I've been "
		"having nightmares non-stop since coming here. Commander says it's a rare side effect of the cryo, "
		"but I don't remember hearing anything like that back on Earth. And it's not just me, either; looks "
		"like everyone has the same problem. ";

	std::string s_log3_4 = "Whatever. The faster we finish, the sooner we leave. ";

	/* ================== LOG TEXT 4 ================== */

	std::string s_log4_2 = "Major breakthrough: it turns out that we do have a magic rock sensor. I found it "
		"today near the digging equipment. Somehow, we all forgot about it. Very strange. Another side "
		"effect of the cryo, the commander says. ";

	std::string s_log4_3 = "Doesn't matter, with this thing, we'll be able to work twice as fast. "
		"Mary, I'll be coming home soon. . . soonish. ";

	/* ================== LOG TEXT 5 ================== */

	std::string s_log5_2 = "We've finally found it. A sample of the rock-thing that the scientists said would "
		"be here. It looks almost like a ruby - very pretty. Soon as we found it, the commander grabbed it "
		"and threw it in the toolbox near the cryo room. No idea why. Said we're done here and to start "
		"preparations for departure. Told us to dump all the digging equipment out. To lighten the load "
		"for the return trip. ";

	std::string s_log5_3 = "We came all the way here for one rock? The hell? Commander says these orders "
		"come from the top. Need-to-know basis. I'm too much of a fucking peasant in the pecking order "
		"to know, he says. Well, he didn't say that bit. But it's implied. ";

	std::string s_log5_4 = "We leave within three days. Finally.";

	/* ================== LOG TEXT 6 ================== */

	std::string s_log6_2 = "Nightmares are getting worse. Way worse. Ever since we brought the rock in. "
		"Just me this time, though. Doesn't help I sleep in the cryo-room alone. ";

	std::string s_log6_3 = "In my dreams, I hear a voice, telling me to do things. Horrible things. "
		"I don't understand what's going on. ";

	/* ================== LOG TEXT 7 ================== */

	std::string s_log7_2 = "I changed my pod's wake-up time. Sometime in two years. I don't know why. "
		"Just a feeling. That there's something I'll need to do. ";


	/* ================== BOOTING ================== */

	Sentence const lines = Sentence("------------------------------------", 0.5, 1);
	Sentence const boot1 = Sentence("BOOTING TERMINAL", 1.0f, 1);
	Sentence const boot2 = Sentence(" ", 1.0f, 1);
	Sentence const boot3 = Sentence("TERMINAL. . . FUNCTIONAL", 1.0f, 1);
	Sentence const boot4 = Sentence("COMMUNICATIONS. . . FUNCTIONAL", 1.0f, 1);
	Sentence const boot5 = Sentence("COMMUNICATIONS. . . FUNCTIONAL", 1.0f, 2);
	Sentence const boot6 = Sentence("COMMUNICATIONS. . . FUNCTIONAL", 1.0f, 3);
	Sentence const boot7 = Sentence("COMMUNICATIONS. . . FUNCTIONAL", 1.0f, 1);

	/* ================== TERMINAL MENU ================== */

	Sentence const menu1 = Sentence("RUN SHIP DIAGNOSTIC ", 0.25f, 1);
	Sentence const menu2 = Sentence("DISENGAGE ALL LOCKS ", 0.25f, 1);
	Sentence const menu3 = Sentence("MISSION LOGS ", 0.25f, 1);
	Sentence const menu4 = Sentence("ACTIVATE ENGINES ", 0.25f, 1);
	Sentence const menu5 = Sentence("EXIT TERMINAL ", 0.25f, 1);

	/* ================== MISSION LOGS ================== */

	Sentence const back = Sentence("BACK", 0.75f, 1);
	Sentence const log1_1 = Sentence("02/03/2034", 0.1f, 1);
	Sentence const log1_2 = Sentence(s_log1_2, 0.75f, 1);
	Sentence const log1_3 = Sentence(s_log1_3, 0.75f, 1);
	Sentence const log1_4 = Sentence(s_log1_4, 0.75f, 1);

	Sentence const log2_1 = Sentence("01/18/2037", 0.1f, 1);
	Sentence const log2_2 = Sentence(s_log2_2, 0.75f, 1);
	Sentence const log2_3 = Sentence(s_log2_3, 0.75f, 1);

	Sentence const log3_1 = Sentence("01/25/2037", 0.1f, 1);
	Sentence const log3_2 = Sentence(s_log3_2, 0.75f, 1);
	Sentence const log3_3 = Sentence(s_log3_3, 0.75f, 1);
	Sentence const log3_4 = Sentence(s_log3_4, 0.75f, 1);

	Sentence const log4_1 = Sentence("01/28/2037", 0.1f, 1);
	Sentence const log4_2 = Sentence(s_log4_2, 0.75f, 1);
	Sentence const log4_3 = Sentence(s_log4_3, 0.75f, 1);

	Sentence const log5_1 = Sentence("02/06/2037", 0.1f, 1);
	Sentence const log5_2 = Sentence(s_log5_2, 0.75f, 1);
	Sentence const log5_3 = Sentence(s_log5_3, 0.75f, 1);
	Sentence const log5_4 = Sentence(s_log5_4, 0.75f, 1);

	Sentence const log6_1 = Sentence("02/08/2037", 0.1f, 1);
	Sentence const log6_2 = Sentence(s_log6_2, 0.75f, 1);
	Sentence const log6_3 = Sentence(s_log6_3, 0.75f, 1);

	Sentence const log7_1 = Sentence("02/09/2037", 0.1f, 1);
	Sentence const log7_2 = Sentence(s_log7_2, 0.75f, 1);

	/* ================== RUN SHIP DIAGNOSTIC  ================== */

	Sentence const diag1 = Sentence("AIR LEAK IN HALLWAY 2", 0.5, 1);
	Sentence const diag2 = Sentence("ENGINE 2 DESTROYED", 0.5, 1);
	Sentence const diag3 = Sentence("ENGINE 3 DESTROYED", 0.5, 1);
	Sentence const diag4 = Sentence("COMMUNICATIONS CHIP FUNCTIONAL", 0.5, 1);

	/* ================== DISENGAGE LOCKS ================== */

	Sentence const diseng1 = Sentence("DISENGAGING LOCKS. . . ", 0.5, 1);
	Sentence const diseng2 = Sentence("FAILURE. DUE TO AIR LEAK, COMMANDER BIOMETRICS REQUIRED", 0.5, 1);

	/* ================== ACTIVATE ENGINES ================== */

	Sentence const eng1 = Sentence("FAILURE. MAJORITY OF ENGINES DAMAGED", 0.5, 1);
	Sentence const eng2 = Sentence("PROBABILITY OF SUCCESFUL ACTIVATION: 0", 0.5, 1);

	enum t_state {DIAG, LOG, EXIT, DISENG, ENG, MENU, LOG1, LOG2, LOG3, LOG4, LOG5, LOG6, LOG7} terminal = MENU;
	bool displayed_boot = false;

	glm::vec2 view_min = glm::vec2(0,0);
	glm::vec2 view_max = glm::vec2(1920, 1080);
	glm::vec2 screen_left = glm::vec2(220, 150);
	glm::vec2 screen_right = glm::vec2(1730, 960);

	std::queue<std::pair<Sentence, int>> terminal_display;

	//------ background music -------
	std::shared_ptr< Sound::PlayingSample > background_music;
};
