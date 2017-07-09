#include "StrokeAnimation.hpp"
#include "SpritePool.hpp"
#include "Utility.hpp"

#include <vector>

StrokeAnimation::StrokeAnimation(Utility* utility, std::string path) 
	: utility(utility) {
	std::vector<Frame> frames = parseFrames(path);
	SpritePool* rectanglePool = new SpritePool(utility->blockPath, Origin::Centre);
	drawRectangles(rectanglePool, frames);

	SpritePool* linePool = new SpritePool(utility->blockPath, Origin::CentreLeft);
	drawLines(linePool, frames);
}

void StrokeAnimation::drawRectangles(SpritePool* rectanglePool, std::vector<Frame> frames) {

}

void StrokeAnimation::drawLines(SpritePool* linePool, std::vector<Frame> frames) {
	for (auto& frame : frames) {
		for (int i = 0; i < frame.lines.size(); i++) {
			Sprite* sprite = linePool->Get(i);
			float startTime = frame.time.ms;
			Vector2 startPos = frame.lines[i].start;
			Vector2 endPos = frame.lines[i].end;

			sprite->Move(startTime, startTime, startPos, startPos);

			Vector2 diff = endPos - startPos;
			float rotation = Vector2(1, 0).AngleBetween(diff);
			sprite->Rotate(startTime, startTime, rotation, rotation);

			float dist = diff.Magnitude();
			sprite->ScaleVector(startTime, startTime, dist, 1, dist, 1);
		}
	}
}

std::vector<Frame> StrokeAnimation::parseFrames(std::string path) {
	XMLDocument doc;
	doc.LoadFile(path.c_str());

	XMLElement* root = doc.RootElement();
	XMLElement* frame = root->FirstChildElement();
	std::vector<Frame> frames;
	while (frame != NULL) {
		XMLElement* strokes = frame->FirstChildElement();
		XMLElement* stroke = strokes->FirstChildElement();
		std::vector<Line> lines;
		while (stroke != NULL) {
			Line line = parseLine(stroke);
			lines.push_back(line);
			stroke = stroke->NextSiblingElement();
		}

		XMLElement* colorRectangles = strokes->NextSiblingElement();
		XMLElement* colorRectangle = colorRectangles->FirstChildElement();
		std::vector<Rectangle> rectangles;
		while (colorRectangle != NULL) {
			Rectangle rectangle = parseRectangle(colorRectangle);
			rectangles.push_back(rectangle);
			colorRectangle = colorRectangle->NextSiblingElement();
		}

		XMLElement* TimeSpanString = colorRectangles->NextSiblingElement();
		Time time(TimeSpanString->GetText());

		Frame newFrame(lines, rectangles, time);
		frames.push_back(newFrame);

		frame = frame->NextSiblingElement();
	}

	return frames;
}

Line StrokeAnimation::parseLine(XMLElement* element) {
	XMLElement* first = element->FirstChildElement();
	Vector2 start = parsePoint(first);

	XMLElement* second = first->NextSiblingElement();
	Vector2 end = parsePoint(second);

	Line line(start, end);
	return line;
}

Vector2 StrokeAnimation::parsePoint(XMLElement* element) {
	XMLElement* X = element->FirstChildElement();
	float x = std::atof(X->GetText());

	XMLElement* Y = X->NextSiblingElement();
	float y = std::atof(Y->GetText());

	Vector2 panelCoordinate(x, y);
	Vector2 osuCoordinate = utility->panelToOsu(panelCoordinate);
	return osuCoordinate;
}

Vector2 StrokeAnimation::parseSize(XMLElement* element) {
	XMLElement* width = element->FirstChildElement();
	float widthValue = std::atof(width->GetText());

	XMLElement* height = width->NextSiblingElement();
	float heightValue = std::atof(height->GetText());

	Vector2 size(widthValue, heightValue);
	return size;
}

Rectangle StrokeAnimation::parseRectangle(XMLElement* element) {
	XMLElement* simpleRect = element->FirstChildElement();
	XMLElement* position = simpleRect->FirstChildElement();
	Vector2 positionValue = parsePoint(position);

	XMLElement* size = position->NextSiblingElement();
	Vector2 sizeValue = parseSize(size);

	XMLElement* rotation = simpleRect->NextSiblingElement();
	float rotationValue = std::atof(rotation->GetText());

	Rectangle rectangle(positionValue, sizeValue, rotationValue);
	return rectangle;
}