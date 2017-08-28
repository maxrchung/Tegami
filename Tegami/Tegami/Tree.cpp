#include "Sprite.hpp"
#include "Vector2.hpp"
#include "Time.hpp"
#include "Utility.hpp"

#include "Tree.hpp"



Tree::Tree(Utility *utility, Vector2 startingPoint, float angle, float startTime, float endTime, float scale, int numberOfIter)
{
	//for (int i = 0; i < numberOfIter; i++) {
	//	for (int j = 0; j < 2 ^ i; j++) {
	//		std::vector<Vector2> endPoints;
	//		Vector2 endPoint = Tree::generateBranch(utility, );
	//		endPoints.push_back(endPoint);
	//	}
	//}

	returnStruct endStruct = Tree::generateBranch(utility, startingPoint, startTime, scale, numberOfIter/(endTime - startTime), angle);
	if ((numberOfIter - 1) > 0) {
		Tree::Tree(utility, endStruct.point, angle + (PI/5), endStruct.endTime, endTime, scale*0.8, numberOfIter - 1);
		Tree::Tree(utility, endStruct.point, angle - (PI/5), endStruct.endTime, endTime, scale*0.8, numberOfIter - 1);
	}
}


Tree::returnStruct Tree::generateBranch(Utility *utility, Vector2 startingPoint, float startTime, float scale, float speed, float angle) {
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

	//for (int i = 0; i < numberOfDots-1; i++) {
	//	branchPoints.push_back(branchPoints[i] + dotOffsets[i]);
	//}
	float currTime = startTime;
	for (int i = 0; i < numberOfDots-1; i++) {
		
		branchPoints.push_back(branchPoints[i] + dotOffsets[i]);
		branchPoints[i] = branchPoints[i].RotateAround(startingPoint, angle);

		Sprite *branch = new Sprite("sprite/lildot.png",branchPoints[i], Layer::Foreground);
		branch->Scale(currTime, Time("10:00:000").ms, scale/5, scale/5);
		branch->Color(currTime, Time("10:00:000").ms, Color(255, 255, 255), Color(255, 255, 255));
		branch->Fade(currTime, currTime+(speed/numberOfDots), 0, 1);

		currTime += (speed / numberOfDots);
	}

	branchPoints.back() = branchPoints.back().RotateAround(startingPoint, angle);

	out.point = branchPoints.back();
	out.endTime = currTime;

	return out;
}