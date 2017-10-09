#include "Okaerinasai.hpp"
#include "Sprite.hpp"

#include <list>
#include <png.h>

Okaerinasai::Okaerinasai(Utility* utility)
	: utility(utility) {
	auto bg = PNGWrapper("okaerinasai.png");
	
	Time start("03:59:105");
	Time generationEnd("04:20:924");
	Time fadeEnd("04:24:041");

	auto points = poissonDisk(10);
	fisherYates(points);

	auto iterations = (generationEnd.ms - start.ms) / utility->mspf + 1;
	auto numRects = (int)(points.size() / iterations);
	
	for (auto t = start.ms; t < generationEnd.ms; t += utility->mspf) {
		for (auto i = 0; i < numRects; i++) {
			auto point = points.back();
			auto sprite = new Sprite("o.png", point);

			sprite->Fade(t, fadeEnd.ms, 0, 1, Easing::QuadIn);
			sprite->Fade(fadeEnd.ms, Time("04:29:885").ms, 1, 0, Easing::Linear);

			auto rotation = (rand() % 314) / 100.0f;
			sprite->Rotate(t, t, rotation, rotation, Easing::Linear, 1);

			Color color = GetFromPNG(bg, point);
			sprite->Color(t, t, color, color);

			points.pop_back();
		}
	}
}

Color Okaerinasai::GetFromPNG(const PNGWrapper& bg, Vector2 point) const {
	auto x = point.x + 854 / 2;
	// Flip to account for PNG coordinates
	auto y = -point.y + 239;
	auto color = bg.Get(x, y);
	return color;
}

void Okaerinasai::fisherYates(std::vector<Vector2>& points) {
	for (auto i = points.size() - 1; i >= 1; --i) {
		auto random = rand() % (i + 1);
		auto temp = points[i];
		points[i] = points[random];
		points[random] = temp;
	}
}

std::vector<Vector2> Okaerinasai::poissonDisk(float minDist, int maxTries) const {
	BoundingRectangle bounds({
		Vector2(0, 0),
		Vector2(853, 0),
		Vector2(853, 480),
		Vector2(0, 480)
	});

	auto cellSize = minDist / sqrtf(2);
	auto widthCount = bounds.width / cellSize + 1;
	auto heightCount = bounds.height / cellSize + 1;
	auto grid = std::vector<std::vector<bool>>(widthCount, std::vector<bool>(heightCount, false));

	// Start with one active point
	auto active = std::vector<Vector2> {
		Vector2(rand() % 853, rand() % 480)
	};

	std::vector<Vector2> total;
	while (!active.empty()) {
		auto tries = 0;
		auto sample = active.back();
		while (tries++ < maxTries) {
			auto dist = rand() % (int)minDist + minDist;
			auto degrees = utility->DToR(rand() % 360);
			auto rotated = Vector2(dist, 0).Rotate(degrees);
			auto next = sample + rotated;
			
			if (!bounds.Contains(next)) {
				continue;
			}

			auto x = (int)(next.x / cellSize);
			auto y = (int)(next.y / cellSize);

			if (grid[x][y]) {
				continue;
			}

			grid[x][y] = true;
			active.push_back(next);
			total.push_back(Vector2((int)next.x - 427, (int)next.y - 240));
		}
		active.pop_back();
	}

	return total;
}

