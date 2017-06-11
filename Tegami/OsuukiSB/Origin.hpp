#ifndef ORIGIN_HPP
#define ORIGIN_HPP

#include <string>

enum Origin {
	TopLeft,
	TopCentre,
	TopRight,
	CentreLeft,
	Centre,
	CentreRight,
	BottomLeft,
	BottomCentre,
	BottomRight,
	OriginCount
};

extern std::string Origins[];

#endif//ORiGIN_HPP