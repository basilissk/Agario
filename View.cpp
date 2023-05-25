#include "View.h"

sf::View view;
sf::View SetView(float x, float y) {
	view.setCenter(x, y);
	return view;
}
