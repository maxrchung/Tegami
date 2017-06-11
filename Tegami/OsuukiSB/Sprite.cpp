#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <sstream>

Sprite::Sprite(const std::string& filePath, Vector2 position, Layer layer, Origin origin)
	: layer(layer), origin(origin), filePath(filePath), position(position), scale(1.0f), scaleVector(1.0f, 1.0f), rotation(0.0f), color(Color::Color(0, 0, 0)), fade(1.0f) {
	// Moved out for clarity
	this->startPosition = Vector2(position.x, -position.y) + Vector2::Midpoint;
	Storyboard::Instance()->sprites[layer].push_back(this);
}

// _M,<easing>,<starttime>,<endtime>,<start_x>,<start_y>,<end_x>,<end_y>
void Sprite::Move(int startTime, int endTime, float startX, float startY, float endX, float endY, Easing easing) {
	if (endTime > this->endTime) {
		this->endTime = endTime;
	}

	position.x = endX;
	position.y = endY;
	Vector2 offsetStart = Vector2::Midpoint + Vector2(startX, -startY);
	Vector2 offsetEnd = Vector2::Midpoint + Vector2(endX, -endY);

	std::ostringstream command;
	command << "_M," << easing << "," << startTime << "," << endTime << "," << offsetStart.x << "," << offsetStart.y << "," << offsetEnd.x << "," << offsetEnd.y;
	commands.push_back(command.str());
}

void Sprite::Move(int startTime, int endTime, Vector2 startPos, Vector2 endPos, Easing easing) {
	Sprite::Move(startTime, endTime, startPos.x, startPos.y, endPos.x, endPos.y, easing);
}

// _MX,<easing>,<starttime>,<endtime>,<start_x>,<end_x>
void Sprite::MoveX(int startTime, int endTime, float startX, float endX, Easing easing) {
	if (endTime > this->endTime) {
		this->endTime = endTime;
	}

	position.x = endX;
	float offsetStart = Vector2::Midpoint.x + startX;
	float offsetEnd = Vector2::Midpoint.x + endX;

	std::ostringstream command;
	command << "_MX," << easing << "," << startTime << "," << endTime << "," << offsetStart << "," << offsetEnd;
	commands.push_back(command.str());
}

// _MY,<easing>,<starttime>,<endtime>,<start_y>,<end_y>
void Sprite::MoveY(int startTime, int endTime, float startY, float endY, Easing easing) {
	if (endTime > this->endTime) {
		this->endTime = endTime;
	}

	position.y = endY;
	float offsetStart = Vector2::Midpoint.y + startY;
	float offsetEnd = Vector2::Midpoint.y + endY;

	std::ostringstream command;
	command << "_MY," << easing << "," << startTime << "," << endTime << "," << offsetStart << "," << offsetEnd;
	commands.push_back(command.str());
}

// _F,<easing>,<starttime>,<endtime>,<start_opacity>,<end_opacity>
void Sprite::Fade(int startTime, int endTime, float startOpacity, float endOpacity, Easing easing) {
	if (endTime > this->endTime) {
		this->endTime = endTime;
	}

	fade = endOpacity;
	std::ostringstream command;
	command << "_F," << easing << "," << startTime << "," << endTime << "," << startOpacity << "," << endOpacity;
	commands.push_back(command.str());
}

// _R,<easing>,<starttime>,<endtime>,<start_rotate>,<end_rotate>
void Sprite::Rotate(int startTime, int endTime, float startRotate, float endRotate, Easing easing) {
	if (endTime > this->endTime) {
		this->endTime = endTime;
	}

	rotation = endRotate;
	std::ostringstream command;
	command << "_R," << easing << "," << startTime << "," << endTime << "," << startRotate << "," << endRotate;
	commands.push_back(command.str());
}

// _S,<easing>,<starttime>,<endtime>,<start_scale>,<end_scale>
void Sprite::Scale(int startTime, int endTime, float startScale, float endScale, Easing easing) {
	if (endTime > this->endTime) {
		this->endTime = endTime;
	}

	scale = endScale;
	std::ostringstream command;
	command << "_S," << easing << "," << startTime << "," << endTime << "," << startScale << "," << endScale;
	commands.push_back(command.str());
}

// _V,<easing>,<starttime>,<endtime>,<start_scale_x>,<start_scale_y>,<end_scale_x>,<end_scale_y>
void Sprite::ScaleVector(int startTime, int endTime, float startX, float startY, float endX, float endY, Easing easing) {
	if (endTime > this->endTime) {
		this->endTime = endTime;
	}

	scaleVector.x = endX;
	scaleVector.y = endY;
	std::ostringstream command;
	command << "_V," << easing << "," << startTime << "," << endTime << "," << startX << "," << startY << "," << endX << "," << endY;
	commands.push_back(command.str());
}

void Sprite::ScaleVector(int startTime, int endTime, Vector2 startScale, Vector2 endScale, Easing easing) {
	Sprite::ScaleVector(startTime, endTime, startScale.x, startScale.y, endScale.x, endScale.y, easing);
}

// _C,<easing>,<starttime>,<endtime>,<start_r>,<start_g>,<start_b>,<end_r>,<end_g>,<end_b>
void Sprite::Color(int startTime, int endTime, int startR, int startG, int startB, int endR, int endG, int endB, Easing easing) {
	if (endTime > this->endTime) {
		this->endTime = endTime;
	}

	color = Color::Color(endR, endG, endB);
	std::ostringstream command;
	command << "_C," << easing << "," << startTime << "," << endTime << "," << startR << "," << startG << "," << startB << "," << endR << "," << endG << "," << endB;
	commands.push_back(command.str());
}

void Sprite::Color(int startTime, int endTime, ::Color startColor, ::Color endColor, Easing easing) {
	Sprite::Color(startTime, endTime, startColor.r, startColor.g, startColor.b, endColor.r, endColor.g, endColor.b, easing);
}

//_L, <starttime>, <loopcount>
//__<event>, <easing>, <relative_starttime>, <relative_endtime>, <params...>
void Sprite::Loop(int startTime, int loopCount, const std::vector<std::string>& loopCommands) {
	std::ostringstream command;
	command << "_L," << startTime << "," << loopCount;
	commands.push_back(command.str());

	for (auto& loopCommand : loopCommands) {
		commands.push_back(loopCommand);
	}
}

void Sprite::Write(std::ofstream& outputFile) {
	// Sprite,<layer>,<origin>,"<filepath>",<x>,<y>
	outputFile << "Sprite," << Layers[layer] << "," << Origins[origin] << ",\"" << filePath << "\"," << startPosition.x << "," << startPosition.y << std::endl;
	for (auto command : commands) {
		outputFile << command << std::endl;
	}
}