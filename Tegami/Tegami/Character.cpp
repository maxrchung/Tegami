#include "Character.hpp"

Character::Character(std::vector<Stroke> strokes)
	: strokes(strokes) {
}

//Character Character::operator*(float a) {
//	std::vector<Stroke> out{ strokes.size() };
//	for (int i = 0; i < strokes.size(); i++) {
//		out[i] = strokes[i] * a;
//	}
//	return Character(out);
//}

void Character::scale(float scale) {
	for (int i = 0; i < strokes.size(); i++) {
		for (int j = 0; j < strokes[i].points.size(); j++) {
			strokes[i].points[j] *= scale;
		}
	}
}