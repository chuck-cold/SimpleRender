#include "Image.hpp"



static double makeFloatingPointColor(int value) {
	return value / 255.;
}
static inline int makeByteColor(double value) {
	if (value <= 0) return 0;
	if (value >= 1) return 255;
	return int(value * 255 + 0.5);
}



//Image

bool Image::load(const std::string &filename, Format f) {
	switch (f) {
	case TGA:
		return ImageTGA::loadTGA(*this,filename);
	default:
		std::cout << "Can not load image - wrong image format" << std::endl;
		return false;
	}
}

bool Image::save(const std::string &filename, Format f) const {
	if (filename.empty()) return 0;
	switch (f) {
	case TGA:
		return ImageTGA::saveTGA(*this, filename);
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
	catch (std::out_of_range) {
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
		throw std::out_of_range("getting non existing channel");

	}
}



//ImageTGA

bool ImageTGA::loadTGA(Image &img, const std::string &filename) {
	TGAImage tga_img;
	if (!tga_img.read_tga_file(filename.data())) {
		std::cerr << "Can not load image - wrong filename" << std::endl;
		return false;
	}
	img.height_ = tga_img.get_height();
	img.width_ = tga_img.get_width();

	switch (tga_img.get_bytespp()) {
	case 1:
		loadTgaGS(img, tga_img);
		return true;
	case 3:
		loadTgaRGB(img, tga_img);
		return true;
	case 4:
		loadTgaRGBA(img, tga_img);
		return true;
	}
	std::cerr << "Can not load image - wrong channel format" << std::endl;
	return false;


}

void ImageTGA::loadTgaChannel(Image &img, Channels channel,TGAImage& tga_img) {
	img.frameBuff[channel] = Frame(img.width(), img.height());
	for (unsigned i = 0; i < img.height()*img.width(); i++) {
		img.frameBuff[channel][i] = makeFloatingPointColor(tga_img.get(i).val);
	}
}

void ImageTGA::loadTgaGS(Image &img, TGAImage& tga_img){
	loadTgaChannel(img, Channels::GS_VALUE, tga_img);
}

void ImageTGA::loadTgaRGB(Image &img, TGAImage& tga_img) {
	img.frameBuff[Channels::RED] = Frame(img.width_, img.height_);
	img.frameBuff[Channels::GREEN] = Frame(img.width_, img.height_);
	img.frameBuff[Channels::BLUE] = Frame(img.width_, img.height_);

	for (unsigned i = 0; i < img.height_*img.width_; i++) {
		TGAColor tColor = tga_img.get(i);
		img.frameBuff[Channels::RED][i] = makeFloatingPointColor(tColor.r);
		img.frameBuff[Channels::GREEN][i] = makeFloatingPointColor(tColor.g);
		img.frameBuff[Channels::BLUE][i] = makeFloatingPointColor(tColor.b);
	}
}

void ImageTGA::loadTgaRGBA(Image &img, TGAImage& tga_img) {
	img.frameBuff[Channels::RED] = Frame(img.width_, img.height_);
	img.frameBuff[Channels::GREEN] = Frame(img.width_, img.height_);
	img.frameBuff[Channels::BLUE] = Frame(img.width_, img.height_);
	img.frameBuff[Channels::ALFA] = Frame(img.width_, img.height_);

	for (unsigned i = 0; i < img.height_*img.width_; i++) {
		TGAColor tColor = tga_img.get(i);
		img.frameBuff[Channels::RED][i] = makeFloatingPointColor(tColor.r);
		img.frameBuff[Channels::GREEN][i] = makeFloatingPointColor(tColor.g);
		img.frameBuff[Channels::BLUE][i] = makeFloatingPointColor(tColor.b);
		img.frameBuff[Channels::ALFA][i] = makeFloatingPointColor(tColor.a);
	}
}


bool ImageTGA::saveTGA(const Image &img, const std::string &filename){
	if (img.frameBuff.find(Channels::GS_VALUE) != img.frameBuff.end()) {
		return saveTgaGS(img, filename);
	}
	if (img.frameBuff.find(Channels::RED) != img.frameBuff.end() &&
		img.frameBuff.find(Channels::GREEN) != img.frameBuff.end() &&
		img.frameBuff.find(Channels::BLUE) != img.frameBuff.end()) {

		if (img.frameBuff.find(Channels::ALFA) == img.frameBuff.end())
			return saveTgaRGB(img, filename);
		return saveTgaRGBA(img, filename);
	}
	std::cerr << "Can not save image  - no TGA format Available" << std::endl;
	return false;
}

bool ImageTGA::saveTgaChannel(const Image &img, Channels channel, const std::string &filename) {
	TGAImage tga_img(img.width(), img.height(), 1);
	for (unsigned i = 0; i < img.height()*img.width(); i++) {
		int value = makeByteColor(img.frameBuff.at(channel)[i]);
		tga_img.set(i, TGAColor(value, 1));
	}
	return tga_img.write_tga_file(filename.data());
}

bool ImageTGA::saveTgaGS(const Image &img, const std::string &filename){
	return saveTgaChannel(img, Channels::GS_VALUE,filename);
}

bool ImageTGA::saveTgaRGB(const Image &img, const std::string &filename){
	TGAImage tga_img(img.width_, img.height_, 4);
	for (unsigned i = 0; i < img.height_*img.width_; i++) {
		tga_img.set(i,
			TGAColor(
				makeByteColor(img.frameBuff.at(Channels::RED)[i]),
				makeByteColor(img.frameBuff.at(Channels::GREEN)[i]),
				makeByteColor(img.frameBuff.at(Channels::BLUE)[i]),
				255)
		);
	}
	return tga_img.write_tga_file(filename.data());

}

bool ImageTGA::saveTgaRGBA(const Image &img, const std::string &filename){
	TGAImage tga_img(img.width_, img.height_, 4);
	for (unsigned i = 0; i < img.height_*img.width_; i++) {
		tga_img.set(i,
			TGAColor(
				makeByteColor(img.frameBuff.at(Channels::RED)[i]),
				makeByteColor(img.frameBuff.at(Channels::GREEN)[i]),
				makeByteColor(img.frameBuff.at(Channels::BLUE)[i]),
				makeByteColor(img.frameBuff.at(Channels::ALFA)[i]))
		);
	}
	return tga_img.write_tga_file(filename.data());

}


