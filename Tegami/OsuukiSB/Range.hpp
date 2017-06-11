#ifndef RANGE_HPP
#define RANGE_HPP

// Pretty general purpose Range class for holding two ints, ideally used for indices
// Honestly seems kind of pointless since I might as well just make a Vector2i class
// or something. Whatever, I'm using this for a very small thing in Dualive's
// Moire background. I'll re-evaluate this class in the future.

// lol i never actually used it in the Moire
class Range {
public:
	Range(int start, int end);
	explicit Range(int startEnd);
	// Returns if start <= value <= end
	bool Contains(int value);
	int start;
	int end;
};

#endif//RANGE_HPP