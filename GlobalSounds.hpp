#pragma once

#include "Sound.hpp"
#include "Load.hpp"
#include "data_path.hpp"



Load< Sound::Sample > emergency = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Emergency.opus"));
	});

Load< Sound::Sample > ambience = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("bgm.wav"));
	});

Load< Sound::Sample > finger_one = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Finger1.opus"));
	});

Load< Sound::Sample > finger_two = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Finger2.opus"));
	});

Load< Sound::Sample > door_air = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Door_air.opus"));
	});

Load< Sound::Sample > door_open = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Door.opus"));
	});

Load< Sound::Sample > crowbar_one = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Crowbar_Glass_1.opus"));
	});

Load< Sound::Sample > crowbar_two = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Crowbar_Glass_2.opus"));
	});

Load< Sound::Sample > ship_crash = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Crash.opus"));
	});

Load< Sound::Sample > forced = Load< Sound::Sample >(LoadTagDefault, []() -> Sound::Sample * {
	return new Sound::Sample(data_path("Forced.opus"));
	});