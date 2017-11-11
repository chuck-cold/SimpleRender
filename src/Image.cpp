#include "Image.hpp"

bool Image::load(const std::string &filename, const Format &f) {
	switch (f) {
	case TGA:
		return loadTGA(filename);
	default:
		std::cout << "Can not load image - wrong image format" << std::endl;
		return false;
	}
};

static double makeFlotingColor(int value) {
	return value/255.;
}

void Image::loadTgaGS(TGAImage& img){
	frameBuff[GS_VALUE] = Frame(width_, height_);
	for (unsigned i = 0; i < height_*width_; i++) {
		frameBuff[GS_VALUE][i] = makeFlotingColor(img.get(i).val);
	}
}

void Image::loadTgaRGB(TGAImage& img) {
	frameBuff[RED] = Frame(width_, height_);
	frameBuff[GREEN] = Frame(width_, height_);
	frameBuff[BLUE] = Frame(width_, height_);

	for (unsigned i = 0; i < height_*width_; i++) {
		TGAColor tColor = img.get(i);
		frameBuff[RED][i] = makeFlotingColor(tColor.r);
		frameBuff[GREEN][i] = makeFlotingColor(tColor.g);
		frameBuff[BLUE][i] = makeFlotingColor(tColor.b);
	}
}

void Image::loadTgaRGBA(TGAImage& img) {
	frameBuff[RED] = Frame(width_, height_);
	frameBuff[GREEN] = Frame(width_, height_);
	frameBuff[BLUE] = Frame(width_, height_);
	frameBuff[ALFA] = Frame(width_, height_);

	for (unsigned i = 0; i < height_*width_; i++) {
		TGAColor tColor = img.get(i);
		frameBuff[RED][i] = makeFlotingColor(tColor.r);
		frameBuff[GREEN][i] = makeFlotingColor(tColor.g);
		frameBuff[BLUE][i] = makeFlotingColor(tColor.b);
		frameBuff[ALFA][i] = makeFlotingColor(tColor.a);
	}
}

bool Image::loadTGA(const std::string &filename) {
	TGAImage img;
	if (!img.read_tga_file(filename.data())) {
		std::cerr << "Can not load image - wrong filename" << std::endl;
		return false;
	}
	height_ = img.get_height();
	width_ = img.get_width();
	
	switch (img.get_bytespp()) {
	case 1:
		loadTgaGS(img);
		return true;
	case 3:
		loadTgaRGB(img);
		return true;
	case 4:
		loadTgaRGBA(img);
		return true;
	}
	std::cerr << "Can not load image - wrong channel format" << std::endl;
	return false;

	
}

bool Image::save(const std::string &filename, const Format &f) const {
	if (filename.empty()) return 0;
	switch (f) {
	case TGA:
		return saveTGA(filename);
	default:
		std::cout << "Can not save image  - Wrong image format" << std::endl;
		return false;
	}

}
void Image::setPixel(Channels channel, double value, unsigned int x, unsigned int y)
{
	try {
		frameBuff.at(channel)[width_*y + x] = value;
	}
	catch(std::out_of_range){
		std::cerr << "setting pixel on nonexisting position" << std::endl;
	}
}
double Image::getPixel(Channels channel, unsigned int x, unsigned int y) const
{
	double result;
	try {
		result = frameBuff.at(channel)[width_*y + x];
	}
	catch (std::out_of_range) {
		std::cerr << "getting pixel on nonexisting position" << std::endl;
		result = 0;
	}
	return result;
}
Frame & Image::getChannel(Channels channel)
{
	try {
		return frameBuff.at(channel);
	}
	catch (std::out_of_range) {
		throw std::out_of_range ("getting non existing channel");
		
	}
}
;

static inline int makeByteColor(double value) {
	if (value <= 0) return 0;
	if (value >= 1) return 255;
	return int(value * 255 + 0.5);
}
bool Image::saveTgaGS(const std::string &filename) const{
	TGAImage img(width_, height_, 1);
	for (unsigned i = 0; i < height_*width_; i++) {
		int value = makeByteColor(frameBuff.at(GS_VALUE)[i]);
		img.set(i, TGAColor(value, 1));
	}
	return img.write_tga_file(filename.data());
}
bool Image::saveTgaRGB(const std::string &filename) const {
	TGAImage img(width_, height_, 4);
	for (unsigned i = 0; i < height_*width_; i++) {
		img.set(i,
			TGAColor(
				makeByteColor(frameBuff.at(RED)[i]),
				makeByteColor(frameBuff.at(GREEN)[i]),
				makeByteColor(frameBuff.at(BLUE)[i]),
				255)
		);
	}
	return img.write_tga_file(filename.data());

}
bool Image::saveTgaRGBA(const std::string &filename) const {
	TGAImage img(width_, height_, 4);
	for (unsigned i = 0; i < height_*width_; i++) {
		img.set(i,
			TGAColor(
				makeByteColor(frameBuff.at(RED)[i]),
				makeByteColor(frameBuff.at(GREEN)[i]),
				makeByteColor(frameBuff.at(BLUE)[i]),
				makeByteColor(frameBuff.at(ALFA)[i]))
		);
	}
	return img.write_tga_file(filename.data());

}

bool Image::saveTGA(const std::string &filename) const{
	if (frameBuff.find(GS_VALUE) != frameBuff.end()) {
		return saveTgaGS(filename);
	}
	if (frameBuff.find(RED) != frameBuff.end() &&
		frameBuff.find(GREEN) != frameBuff.end() &&
		frameBuff.find(BLUE) != frameBuff.end()) {

		if (frameBuff.find(ALFA) == frameBuff.end())
			return saveTgaRGB(filename);
		return saveTgaRGBA(filename);
	}
	std::cerr << "Can not save image  - no TGA format Available" << std::endl;
	return false;
}
