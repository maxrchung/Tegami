#pragma once
class Tree
{
public:

	Tree(Utility *utility, Vector2 startingPoint, float angle, float deltaAngle, float startTime, float endTime, float scale, float branchScale, int numberOfIter, Color startColor, Color endColor);
private:

	typedef struct {
		Vector2 point;
		float endTime;
	} returnStruct;

	returnStruct generateBranch(Utility *utility, Vector2 startingPoint, float startTime, float endTime, float scale, float speed, float angle, Color startColor, Color endColor);
};