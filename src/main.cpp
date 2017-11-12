#include <iostream>
#include "Image.hpp"

int main() {
	Image img;
	//load image
	img.load("test_rgba.tga", TGA);
	//remove Green color from image
	Frame &red = img.getChannel(GREEN);
	std::for_each(red.data().begin(), red.data().end(), [](double &val) {val = 0; });
	//save image
	img.save("rgba_output.tga", TGA);
	return 0;
}