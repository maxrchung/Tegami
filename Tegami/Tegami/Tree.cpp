#include "Color.hpp"
#include "Sprite.hpp"
#include "Vector2.hpp"
#include "Time.hpp"
#include "Utility.hpp"

#include "Tree.hpp"


Tree::Tree(Utility *utility, SpritePool* pool, Vector2 startingPoint, float angle, float deltaAngle, float startTime, float endTime, float fadeTime, float speed, float scale, float branchScale, float imageScale, int numberOfIter, Color startColor, Color endColor, bool applyColorToOneBranch)
	: speed(speed), deltaAngle(deltaAngle), branchScale(branchScale), imageScale(imageScale), applyColorToOneBranch(applyColorToOneBranch), pool(pool) {
	timePerBranch = (endTime - startTime) / numberOfIter;

	if (applyColorToOneBranch) {
		colorDifference = endColor - startColor;
	}
	else {
		colorDifference = (endColor - startColor) / numberOfIter;
	}

	CreateTree(utility, startingPoint, angle, startTime, endTime, fadeTime, scale, numberOfIter, startColor, startColor + colorDifference);
}

void Tree::CreateTree(Utility *utility, Vector2 startingPoint, float angle, float startTime, float endTime, float fadeTime, float scale, int numberOfIter, Color startColor, Color endColor) {
	
	returnStruct endStruct = generateBranch(utility, startingPoint, angle, startTime, endTime, fadeTime, scale, numberOfIter, startColor, endColor);
	
	if ((numberOfIter - 1) > 0) {
		Color endColor2 = endColor;
		if (!applyColorToOneBranch) {
			endColor2.r += colorDifference.r;
			endColor2.g += colorDifference.g;
			endColor2.b += colorDifference.b;
		}

		CreateTree(utility, endStruct.point, angle + Tree::deltaAngle, endStruct.endTime, endTime, fadeTime, scale*branchScale, numberOfIter - 1, endColor, endColor2);
		CreateTree(utility, endStruct.point, angle - Tree::deltaAngle, endStruct.endTime, endTime, fadeTime, scale*branchScale, numberOfIter - 1, endColor, endColor2);
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

	Color colorVector = startColor;

	Color localColorDifference = colorDifference / (numberOfDots - 1);
	if (startColor == endColor) {
		localColorDifference = Color(0);
	}

	float currTime = startTime;
	for (int i = 0; i < numberOfDots - 1; i++) {
		branchPoints.push_back(branchPoints[i] + dotOffsetScaled[i]);
		branchPoints[i] = branchPoints[i].RotateAround(startingPoint, angle);

		if (branchPoints[i].x > Vector2::ScreenSize.x / 2 || branchPoints[i].y > Vector2::ScreenSize.y / 2 || branchPoints[i].x < -Vector2::ScreenSize.x / 2 || branchPoints[i].y < -Vector2::ScreenSize.y / 2) {
		}
		else {
			// Use sprite pool
			auto branch = pool->Get();
			branch->Move(currTime, currTime, branchPoints[i], branchPoints[i]);
			branch->Scale(currTime, fadeTime, scale / 5 * imageScale, scale / 5 * imageScale, Easing::Linear);

			branch->Color(currTime, fadeTime, colorVector, colorVector, Easing::Linear, 1);
			branch->Fade(currTime, currTime + deltaTime, 0, 1);
			branch->Fade(fadeTime, fadeTime + utility->quarterTimeStep, 1, 0);
		}

		colorVector.r += localColorDifference.r;
		colorVector.g += localColorDifference.g;
		colorVector.b += localColorDifference.b;

		currTime += deltaTime;
	}

	branchPoints.back() = branchPoints.back().RotateAround(startingPoint, angle);

	out.point = branchPoints.back();
	out.endTime = currTime;

	return out;
}