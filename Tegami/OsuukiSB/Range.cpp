#include "Range.hpp"

Range::Range(int start, int end)
	: start(start), end(end) {}

Range::Range(int startEnd)
	: start(startEnd), end(startEnd) {}

bool Range::Contains(int value) {
	return start <= value && value <= end;
}