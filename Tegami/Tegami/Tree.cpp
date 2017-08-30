#include "Color.hpp"
#include "Sprite.hpp"
#include "Vector2.hpp"
#include "Time.hpp"
#include "Utility.hpp"

#include "Tree.hpp"



Tree::Tree(Utility *utility, Vector2 startingPoint, float angle, float deltaAngle, float startTime, float endTime, float scale, float branchScale, int numberOfIter, Color startColor, Color endColor)
{
	returnStruct endStruct = Tree::generateBranch(utility, startingPoint, startTime, endTime, scale, numberOfIter/(endTime - startTime), angle, startColor, endColor);
	if ((numberOfIter - 1) > 0) {
		Tree::Tree(utility, endStruct.point, angle + deltaAngle, deltaAngle, endStruct.endTime, endTime, scale*branchScale, branchScale, numberOfIter - 1, endColor, endColor);
		Tree::Tree(utility, endStruct.point, angle - deltaAngle, deltaAngle, endStruct.endTime, endTime, scale*branchScale, branchScale, numberOfIter - 1, endColor, endColor);
	}
}


Tree::returnStruct Tree::generateBranch(Utility *utility, Vector2 startingPoint, float startTime, float endTime, float scale, float speed, float angle, Color startColor, Color endColor) {
	int numberOfDots = 17; 

	returnStruct out;

	speed *= 10000000;

	std::vector<Vector2> branchPoints = { startingPoint };
	std::vector<Vector2> dotOffsets = {Vector2::Vector2(scale * 31,scale * 28),
		Vector2::Vector2(scale * 24,scale * 36),
		Vector2::Vector2(scale * 15,scale * 39),
		Vector2::Vector2(scale * 5,scale * 42),
		Vector2::Vector2(scale * -5,scale * 42),
		Vector2::Vector2(scale * -15,scale * 39),
		Vector2::Vector2(scale * -24,scale * 36),
		Vector2::Vector2(scale * -31,scale * 28),

		Vector2::Vector2(scale * -31,scale * 28), //halfway
		Vector2::Vector2(scale * -24,scale * 36), 
		Vector2::Vector2(scale * -15,scale * 39),
		Vector2::Vector2(scale * -5,scale * 42),
		Vector2::Vector2(scale * 5,scale * 42),
		Vector2::Vector2(scale * 15,scale * 39),
		Vector2::Vector2(scale * 24,scale * 36),
		Vector2::Vector2(scale * 31,scale * 28)
	};

	std::vector<Color> colorVector{ startColor };

	Color colorDifference = startColor - endColor;

	colorDifference = colorDifference / (numberOfDots - 1);

	float currTime = startTime;
	for (int i = 0; i < numberOfDots-1; i++) {

		colorVector.push_back(startColor + colorDifference*i);
		branchPoints.push_back(branchPoints[i] + dotOffsets[i]);
		branchPoints[i] = branchPoints[i].RotateAround(startingPoint, angle);

		Sprite *branch = new Sprite("sprite/lildot.png",branchPoints[i], Layer::Foreground);
		branch->Scale(currTime, endTime, scale/5, scale/5);
		branch->Color(currTime, endTime, colorVector[i], colorVector[1]);
		branch->Fade(currTime, currTime+(speed/numberOfDots), 0, 1);

		currTime += (speed / numberOfDots);
	}

	branchPoints.back() = branchPoints.back().RotateAround(startingPoint, angle);

	out.point = branchPoints.back();
	out.endTime = currTime;

	return out;
}