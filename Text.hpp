#include "GL.hpp"

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>


struct Sentence {
	std::string text;
	float wait_to_print = 1.0f;

	Sentence(std::string const text_, float wait_to_print_ = 1.0f) {
		text = text_;
		wait_to_print = wait_to_print_;
	}
};

struct SentenceAtlas {

	SentenceAtlas(std::string const& filebase);

	Sentence const& lookup(std::string const& name) const;

	//table of sentences
	std::unordered_map< std::string, Sentence > sentences;

	//path to atlas, stored for debugging purposes:
	std::string atlas_path;
};