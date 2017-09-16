#pragma once
class Tree
{
public:

	Tree(Utility *utility, Vector2 startingPoint, float angle, float deltaAngle, float startTime, float endTime, float scale, float branchScale, int numberOfIter, Color startColor, Color endColor);
private:

	// Move this into its own class, probalby be more descriptive too
	// If you don't even change endTime then just make this a point
	// Could just pass in as pointer into generateBranch
	typedef struct {
		Vector2 point;
		float endTime;
	} returnStruct;

	returnStruct generateBranch(Utility *utility, Vector2 startingPoint, float startTime, float endTime, float scale, float speed, float angle, Color startColor, Color endColor);
};