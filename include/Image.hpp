#pragma once
//
// Created by Chladek_K on 16.09.2017.
//
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <map>


#include "tgaimage.h"

enum Channels {
	GS_VALUE,
	RED,
	GREEN,
	BLUE,
	ALFA
};

enum Format {
	TGA
};


struct Frame {
	std::vector<double> data;

	Frame(unsigned w, unsigned h)
		: data(w * h)
	{
	};
	Frame() {};

	double &operator[](int index) {
		return data.at(index);
	};

	const double &operator[](int index) const {
		return data.at(index);
	};

};


class Image {
private:
	std::map<Channels, Frame> frameBuff;
	unsigned int width_;
	unsigned int height_;

	// loads TGAfile from file in param
	bool loadTGA(const std::string &filename);
	//loads greyscale  tga image
	void loadTgaGS(TGAImage& img);
	// loads RGB tga image
	void loadTgaRGB(TGAImage& img);
	//loads RGBA tga image
	void loadTgaRGBA(TGAImage& img);

	//dtc for saving
	bool saveTGA(const std::string &filename) const;
	bool saveTgaGS(const std::string &filename) const;
	bool saveTgaRGB(const std::string &filename) const;
	bool saveTgaRGBA(const std::string &filename) const;

public:
	Image() {};
	Image(unsigned w, unsigned h)
		: width_(w), height_(h)
	{
	};
	//height getter
	unsigned int height() {
		return height_;
	};
	//width getter
	unsigned int width() {
		return width_;
	};
	//main loading function
	//@params
	//filename = name of file to load
	//f = format of given file
	bool load(const std::string &filename, const Format &f);
	//main saving function same as loading
	bool save(const std::string &filename, const Format &f) const;
	//sets pixel on given coordination to given value if there is suitable pixel, else no change is made
	void setPixel(Channels channel, double value, unsigned int x, unsigned int y);
	//gets pixel on given coordination, if does not exist returns 0
	double getPixel(Channels channel, unsigned int x, unsigned int y) const;
	//gets whole frame by given channel
	Frame &getChannel(Channels channel);
	
};

