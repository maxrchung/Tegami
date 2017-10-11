#pragma once

#include "SpritePool.hpp"

class Tree
{
public:

	Tree(Utility *utility, SpritePool& pool, Vector2 startingPoint, float angle, float deltaAngle, float startTime, float endTime, float fadeTime, float speed, float scale, float branchScale, float imageScale, int numberOfIter, Color startColor, Color endColor, bool applyColorToOneBranch = false);
	void Tree::CreateTree(Utility *utility, Vector2 startingPoint, float angle, float startTime, float endTime, float fadeTime, float scale, int numberOfIter, Color startColor, Color endColor);

private:

	// Move this into its own class, probalby be more descriptive too
	// If you don't even change endTime then just make this a point
	// Could just pass in as pointer into generateBranch
	typedef struct {
		Vector2 point;
		float endTime;
	} returnStruct;

	float timePerBranch;
	float speed;
	float branchScale;
	float deltaAngle;
	float imageScale;
	Color colorDifference; //perbranch
	bool applyColorToOneBranch;
	SpritePool pool;

	std::vector<Vector2> dotOffsets = { 
		Vector2::Vector2(31,28),
		Vector2::Vector2(24,36),
		Vector2::Vector2(15,39),
		Vector2::Vector2(5,42),
		
		Vector2::Vector2(-5,42),
		Vector2::Vector2(-15,39),
		Vector2::Vector2(-24,36),
		Vector2::Vector2(-31,28),

		Vector2::Vector2(-31,28), //halfway
		Vector2::Vector2(-24,36),
		Vector2::Vector2(-15,39),
		Vector2::Vector2(-5,42),
		
		Vector2::Vector2(5,42),
		Vector2::Vector2(15,39),
		Vector2::Vector2(24,36),
		Vector2::Vector2(31,28)
	};

	returnStruct generateBranch(Utility *utility, Vector2 startingPoint, float angle, float startTime, float endTime, float fadeTime, float scale, int numberOfIter, Color startColor, Color endColor);
};