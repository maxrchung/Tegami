#include "Color.hpp"
#include "Sprite.hpp"
#include "Vector2.hpp"
#include "Time.hpp"
#include "Utility.hpp"

#include "Tree.hpp"


Tree::Tree(Utility *utility, Vector2 startingPoint, float angle, float deltaAngle, float startTime, float endTime, float fadeTime, float speed, float scale, float branchScale, float imageScale, int numberOfIter, Color startColor, Color endColor)
	: speed(speed), deltaAngle(deltaAngle), branchScale(branchScale), imageScale(imageScale) {
	timePerBranch = (endTime - startTime) / numberOfIter;
	CreateTree(utility, startingPoint, angle, startTime, endTime, fadeTime, scale, numberOfIter, startColor, endColor);
}

void Tree::CreateTree(Utility *utility, Vector2 startingPoint, float angle, float startTime, float endTime, float fadeTime, float scale, int numberOfIter, Color startColor, Color endColor) {
	
	returnStruct endStruct = generateBranch(utility, startingPoint, angle, startTime, endTime, fadeTime, scale, numberOfIter, startColor, endColor);
	
	if ((numberOfIter - 1) > 0) {
		CreateTree(utility, endStruct.point, angle + Tree::deltaAngle, endStruct.endTime, endTime, fadeTime, scale*branchScale, numberOfIter - 1, endColor, endColor);
		CreateTree(utility, endStruct.point, angle - Tree::deltaAngle, endStruct.endTime, endTime, fadeTime, scale*branchScale, numberOfIter - 1, endColor, endColor);
	}
}

Tree::returnStruct Tree::generateBranch(Utility *utility, Vector2 startingPoint, float angle, float startTime, float endTime, float fadeTime, float scale, int numberOfIter, Color startColor, Color endColor) {
	int numberOfDots = 17; 

	float deltaTime = (1/Tree::speed)*1000; //inverse speed is the amount of time between each dot, speed is dot/second, converting seconds to milliseonds

	returnStruct out;

	std::vector<Vector2> branchPoints = { startingPoint };

	std::vector<Vector2> dotOffsetScaled(17);
	dotOffsetScaled = dotOffsets;
	for (auto& e : dotOffsetScaled) {
		e *= scale;
	}

	std::vector<Color> colorVector{ startColor };

	// Combine this code
	Color colorDifference = startColor - endColor;
	colorDifference = colorDifference / (numberOfDots - 1);

	float currTime = startTime;
	for (int i = 0; i < numberOfDots - 1; i++) {
		colorVector.push_back(startColor + colorDifference*i);
		branchPoints.push_back(branchPoints[i] + dotOffsetScaled[i]);
		branchPoints[i] = branchPoints[i].RotateAround(startingPoint, angle);

		if (branchPoints[i].x > Vector2::ScreenSize.x / 2 || branchPoints[i].y > Vector2::ScreenSize.y / 2 || branchPoints[i].x < -Vector2::ScreenSize.x / 2 || branchPoints[i].y < -Vector2::ScreenSize.y / 2) {
		}
		else {
			// Use sprite pool
			Sprite *branch = new Sprite("m.png", branchPoints[i]);
			branch->Scale(currTime, fadeTime, scale / 5 * imageScale, scale / 5 * imageScale);

			// Need to fix this? [1]?
			branch->Color(currTime, fadeTime, colorVector[i], colorVector[i]);
			branch->Fade(currTime, currTime + deltaTime, 0, 1);
			branch->Fade(fadeTime, fadeTime + utility->quarterTimeStep, 1, 0);
		}
		currTime += deltaTime;
	}

	branchPoints.back() = branchPoints.back().RotateAround(startingPoint, angle);

	out.point = branchPoints.back();
	out.endTime = currTime;

	return out;
}