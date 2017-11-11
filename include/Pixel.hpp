#pragma once
#pragma once
#include <type_traits>




struct Pixel {

protected:
	std::vector<double> data_;
public:
	unsigned size() {
		return data_.size();
	}
	const double &operator [](int index) {
		return data_.size() > index && 0 <= index ? data_.at(index) : 0;
	}

};

struct RGBPixel : public Pixel {
	
	RGBPixel()
	{
		data_.resize(RGB);
	}

	double &r() {
		return data_[0];
	}
	double &r() const{
		return data_[0];
	}
	double &g() {
		return data_[1];
	}
	double &b() {
		return data_[2];
	}
};


struct RGBAPixel : public RGBPixel {

	RGBAPixel()
	{
		data_.resize(RGBA);
	}

	double &a() {
		return data_.at(3);
	}
};



struct GSPixel : public Pixel {
	GSPixel()
	{
		data_.resize(GRAYSCALE);
	}
	double &value(int index){
		return data_.at(index);
	}
};