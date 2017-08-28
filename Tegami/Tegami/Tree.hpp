#pragma once
class Tree
{
public:

	Tree(Utility *utility, Vector2 startingPoint, float angle, float startTime, float endTime, float scale, int numberOfIter);
private:

	typedef struct {
		Vector2 point;
		float endTime;
	} returnStruct;

	returnStruct generateBranch(Utility *utility, Vector2 startingPoint, float startTime, float scale, float speed, float angle);
};