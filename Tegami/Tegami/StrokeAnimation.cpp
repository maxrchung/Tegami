#include "StrokeAnimation.hpp"
#include "SpritePool.hpp"
#include "Utility.hpp"

#include <vector>

StrokeAnimation::StrokeAnimation(Utility* utility, std::string path) 
	: utility(utility) {

	std::cout << "Parsing frames..." << std::endl;

	std::vector<Frame> frames = parseFrames(path);
	SpritePool* rectanglePool = new SpritePool(utility->blockPath, Origin::Centre);
	drawRectangles(rectanglePool, frames);

	SpritePool* linePool = new SpritePool(utility->blockPath, Origin::Centre);
	drawLines(linePool, frames);
}

void StrokeAnimation::drawRectangles(SpritePool* rectanglePool, std::vector<Frame> frames) {
	std::cout << "Processing rectangles..." << std::endl;

	for (auto& frame : frames) {
		float startTime = frame.time.ms;
		float endTime = startTime + utility->mspf;


		for (int i = 0; i < frame.rectangles.size(); i++) {
			Sprite* sprite = rectanglePool->Get(i);

			Vector2 pos = frame.rectangles[i].center;
			Vector2 size = frame.rectangles[i].size;
			float rotation = frame.rectangles[i].rotation;
			float radians = utility->DToR(rotation);

			sprite->Move(startTime, endTime, pos, pos);
			sprite->Rotate(startTime, endTime, radians, radians, Easing::Linear, 1);
			sprite->ScaleVector(startTime, endTime, size, size, Easing::Linear, 0);

			if (sprite->fade == 0) {
				sprite->Fade(startTime, endTime, 1, 1);
			}
		}
	}
}

void StrokeAnimation::drawLines(SpritePool* linePool, std::vector<Frame> frames) {
	std::cout << "Processing lines..." << std::endl;

	for (auto& frame : frames) {
		float startTime = frame.time.ms;
		float endTime = startTime + utility->mspf;

		for (int i = 0; i < frame.lines.size(); i++) {
			Sprite* sprite = linePool->Get(i);
			Vector2 startPos = frame.lines[i].start;
			Vector2 endPos = frame.lines[i].end;
			Vector2 midPos = (startPos + endPos) / 2;
			sprite->Move(startTime, endTime, midPos, midPos);

			Vector2 diff = endPos - startPos;
			float rotation = Vector2(1, 0).AngleBetween(diff);
			sprite->Rotate(startTime, endTime, rotation, rotation, Easing::Linear, 1);

			float dist = diff.Magnitude() * lineEdgeScale;
			sprite->ScaleVector(startTime, endTime, dist, 1, dist, 1, Easing::Linear, 0);

			if (sprite->fade == 0) {
				sprite->Fade(startTime, endTime, 1, 1);
			}

			if (sprite->color != Color(0)){
				sprite->Color(startTime, startTime, Color(0), Color(0));
			}
		}

		for (int i = frame.lines.size(); i < linePool->sprites.size(); i++) {
			if (linePool->sprites[i]->fade != 0) {
				linePool->sprites[i]->Fade(startTime, startTime, 0, 0);
			}
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
		Time globalTime(TimeSpanString->GetText());
		Time localTime = offsetVideoTime(globalTime);

		Frame newFrame(lines, rectangles, localTime);
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
	Vector2 osuCoordinate = panelToOsuPoint(panelCoordinate);
	return osuCoordinate;
}

Vector2 StrokeAnimation::parseSize(XMLElement* element) {
	XMLElement* width = element->FirstChildElement();
	float widthValue = std::atof(width->GetText());

	XMLElement* height = width->NextSiblingElement();
	float heightValue = std::atof(height->GetText());

	Vector2 panelSize(widthValue, heightValue);
	Vector2 osuSize = panelToOsuSize(panelSize);
	return osuSize;
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

Vector2 StrokeAnimation::panelToOsuPoint(Vector2 panelCoordinates) {
	float convertedX = (panelCoordinates.x - panelMidPoint.x + panelPosition.x) * panelToOsuScaling.x;
	float convertedY = (-panelCoordinates.y + panelMidPoint.y - panelPosition.y) * panelToOsuScaling.y;

	Vector2 osuCoordinate(convertedX, convertedY);
	return osuCoordinate;
}

Vector2 StrokeAnimation::panelToOsuSize(Vector2 panelSize) {
	float convertedWidth = panelSize.x * panelToOsuScaling.x;
	float convertedHeight = panelSize.y * panelToOsuScaling.y;

	Vector2 osuSize(convertedWidth, convertedHeight);
	return osuSize;
}

Time StrokeAnimation::offsetVideoTime(Time globalTime) {
	float localMilliseconds = globalTime.ms - videoOffset;
	Time localTime = Time(localMilliseconds);
	return localTime;
}
