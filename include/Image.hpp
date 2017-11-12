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


class Frame {
	std::vector<double> data_;
public:
	Frame(unsigned w, unsigned h)
		: data_(w * h)
	{
	};
	Frame() {};

	double &operator[](int index) {
		return data_.at(index);
	};

	const double &operator[](int index) const {
		return data_.at(index);
	};
	std::vector<double> &data() {
		return data_;
	};
	const std::vector<double> &data() const {
		return data_;
	};
};


class Image {
private:
	std::map<Channels, Frame> frameBuff;
	unsigned int width_;
	unsigned int height_;
	friend class ImageTGA;

public:
	Image() {};
	Image(unsigned w, unsigned h)
		: width_(w), height_(h)
	{
	};
	//height getter
	unsigned int height() const {
		return height_;
	};
	//width getter
	unsigned int width() const {
		return width_;
	};
	//main loading function
	//@params
	//filename = name of file to load
	//f = format of given file
	bool load(const std::string &filename, Format f);
	//main saving function same as loading
	bool save(const std::string &filename, Format f) const;
	//sets pixel on given coordination to given value if there is suitable pixel, else no change is made
	void setPixel(Channels channel, double value, unsigned int x, unsigned int y);
	//gets pixel on given coordination, if does not exist returns 0
	double getPixel(Channels channel, unsigned int x, unsigned int y) const;
	//gets whole frame by given channel
	Frame &getChannel(Channels channel);

};

//interface for tgaimage library
class ImageTGA {
	//loads greyscale  tga image
	static void loadTgaGS(Image &img, TGAImage& tga_img);
	// loads RGB tga image
	static void loadTgaRGB(Image &img, TGAImage& tga_img);
	//loads RGBA tga image
	static void loadTgaRGBA(Image &img, TGAImage& tga_img);

	//dtc for saving
	static bool saveTgaGS(const Image &img, const std::string &filename);
	static bool saveTgaRGB(const Image &img, const std::string &filename);
	static bool saveTgaRGBA(const Image &img, const std::string &filename);


public:
	// loads TGAfile from file in param
	static bool loadTGA(Image &img, const std::string &filename);

	static bool saveTGA(const Image &img, const std::string &filename);

	static void loadTgaChannel(Image &img, Channels channel, TGAImage& tga_img);

	static bool saveTgaChannel(const Image &img, Channels channel, const std::string &filename);

	//TODO::
	static bool saveTgaChannel(const Frame &frame, const std::string &filename);
	static void loadTgaChannel(Image &img, Channels channel, const std::string& filename);

};

//upgades..
/*
own saving
more image formats
overload setPixel, getPixel
*/