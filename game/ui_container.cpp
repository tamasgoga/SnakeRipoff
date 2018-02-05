#include "ui_container.hpp"


Container::Container(int x, int y, int w, int h) noexcept
	: box({x, y, w, h})
	, nextIndex(0)
{;}


Container::~Container() {}


Container::index Container::add(const Widget& w) {
	if (nextIndex == container.size()) {
		container.push_back(&w);
		return ++nextIndex;
	}

	container[nextIndex] = &w;

	do {
		++nextIndex;
	} while (nextIndex < container.size() && container[nextIndex] != nullptr);

	return nextIndex;
}


bool Container::remove(index i) {
	if (i >= container.size() || container[i] == nullptr)
		return false;

	container[i] = nullptr;

	if (i < nextIndex)
		nextIndex = i;

	return true;
}
