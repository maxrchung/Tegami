#include "Okaerinasai.hpp"
#include "Sprite.hpp"
#include <list>

Okaerinasai::Okaerinasai(Utility* utility)
	: utility(utility) {
	Time start("03:59:105");
	Time end("04:24:041");

	auto points = poissonDisk(20);
	fisherYates(points);

	auto iterations = (end.ms - start.ms) / utility->mspf;
	auto numRects = points.size() / iterations;

	for (auto t = start.ms; t < end.ms; t += utility->mspf) {
		for (auto i = 0; i < numRects; i++) {
			Color color;

			if (!points.empty()) {
				auto sprite = new Sprite("sprite/solidblock.png", points.back());
				points.pop_back();
				sprite->Fade(t, end.ms, 0, 1, Easing::QuadIn);
				sprite->Fade(end.ms, Time("04:29:885").ms, 1, 0, Easing::Linear);

				auto scale = rand() % 10 + 10;
				sprite->Scale(t, t, scale, scale);

				auto rotation = (rand() % 314) / 100.0f;
				sprite->Rotate(t, t, rotation, rotation, Easing::Linear, 1);

				sprite->Color(t, t, color, color);
			}
		}
	}
}

Vector2 Okaerinasai::fromOsu(float x, float y) {
	return Vector2(x - 320, 240 - y);
}

void Okaerinasai::fisherYates(std::vector<Vector2>& points) {
	for (auto i = points.size() - 1; i >= 1; --i) {
		auto random = rand() % (i + 1);
		auto temp = points[i];
		points[i] = points[random];
		points[random] = temp;
	}
}

std::vector<Vector2> Okaerinasai::poissonDisk(float minDist, int maxTries) {
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

	auto active = std::vector<Vector2> {
		Vector2(rand() % 853, rand() % 480)
	};

	// Used for checking surrounding points
	auto checks = std::vector<Vector2> {
		Vector2(0,0),
		Vector2(-1,1),
		Vector2(0,1),
		Vector2(1,1),
		Vector2(1,0),
		Vector2(1,-1),
		Vector2(0,-1),
		Vector2(-1,-1)
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

			bool collision = false;
			//for (auto& check : checks) {
			//	auto checkX = x + (int)check.x;
			//	auto checkY = y + (int)check.y;
			//	if (checkX < 0 || checkX >= grid.size()) {
			//		continue;
			//	}

			//	if (checkY < 0 || checkY >= grid[0].size()) {
			//		continue;
			//	}

			//	if (grid[checkX][checkY]) {
			//		collision = true;
			//		break;
			//	}
			//}

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

