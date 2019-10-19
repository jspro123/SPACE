#include "Text.hpp"

#include "GL.hpp"
#include "read_write_chunk.hpp"
#include "load_save_png.hpp"

#include <algorithm>
#include <fstream>

SentenceAtlas::SentenceAtlas(std::string const& filebase) {

	auto replace_all = [](std::string cur_string, std::string to_find, std::string replace_with) {

		size_t search = cur_string.find(to_find);
		while ((int)search != -1) {
			cur_string.replace(search, 1, replace_with);
			search = cur_string.find(to_find);
		}
		return cur_string;
	};

	atlas_path = filebase + ".atlas";

	std::ifstream text_file;
	text_file.open(atlas_path);
	if (!text_file) {
		exit(1);
	}

	std::string line;
	std::string variable_name;
	std::string variable_text;
	size_t del_pos = 0;
	while (std::getline(text_file, line)) {
		if (line.substr(0, 1) == "/" && line.substr(1, 1) == "/") {
			continue;
		}

		if (line.size() == 0) {
			continue;
		}

		del_pos = line.find("=");
		variable_name = line.substr(0, del_pos);
		//Stolen from stack overflow
		variable_name.erase(std::remove(variable_name.begin(), variable_name.end(), ' '), variable_name.end());

		variable_text = line.substr((int)del_pos + 2, line.size() - del_pos - 1);
		variable_text = replace_all(variable_text, "#", "\"");

		Sentence sentence = Sentence(variable_text);
		auto ret = sentences.insert(std::make_pair(variable_name, variable_text));
		if (!ret.second) {
			throw std::runtime_error("Sentence with duplicate name '" + variable_name + "' in sentence atlas '" + atlas_path + "',");
		}
	}

	text_file.close();

}

Sentence const& SentenceAtlas::lookup(std::string const& name) const {
	auto f = sentences.find(name);
	if (f == sentences.end()) {
		throw std::runtime_error("Sentence of name '" + name + "' not found in atlas '" + atlas_path + "'.");
	}
	return f->second;
}