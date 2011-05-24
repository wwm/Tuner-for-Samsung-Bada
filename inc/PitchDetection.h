#ifndef __PITCHDETECTION_H__
#define __PITCHDETECTION_H__

#include <FApp.h>
#include <FBase.h>
#include <FGraphics.h>
#include <FSystem.h>
#include <FUi.h>
#include <AudioManager.h>

using namespace std;
using namespace Osp::Base::Utility;

struct ComplexNumber {
public:
	double Re;
	double Im;
public:
	ComplexNumber() {
		Re = 0;
		Im = 0;
	}
	ComplexNumber(double re) {
		Re = re;
		Im = 0;
	}

	ComplexNumber(double re, double im) {
		Re = re;
		Im = im;
	}

	ComplexNumber operator *(ComplexNumber n) {
		ComplexNumber c;
		c.Re = Re * n.Re - Im * n.Im;
		c.Im = Im * n.Re + Re * n.Im;
		return c;
	}

	ComplexNumber operator +(ComplexNumber n) {
		ComplexNumber c;
		c.Re = Re + n.Re;
		c.Im = Im + n.Im;
		return c;
	}

	ComplexNumber operator -(ComplexNumber n) {
		ComplexNumber c;
		c.Re = Re - n.Re;
		c.Im = Im - n.Im;
		return c;
	}

	ComplexNumber operator -() {
		ComplexNumber c;
		c.Re = -Re;
		c.Im = -Im;
		return c;
	}

	ComplexNumber ParseFromDouble(double n) {
		ComplexNumber c;
		c.Re = n;
		c.Im = 0;
		return c;
	}

	ComplexNumber PoweredE() {
		double e =  Math::Exp(Re);
		ComplexNumber c;
		c.Re = e * Math::Cos(Im);
		c.Im = e * Math::Sin(Im);
		return c;
	}
	double Power2() {
		return Re * Re - Im * Im;

	}
	double AbsPower2() {
		return Re * Re + Im * Im;

	}

};

/**
 * [GuitarTuner] application must inherit from Application class
 * which provides basic features necessary to define an application.
 */
class FftAlgorithm

{


public:
	FftAlgorithm();
	~FftAlgorithm();

public:
	double ProcessData(ByteBuffer* data);

private:
	int Log2(int n);
	int ReverseBits(int n, int bitsCount);
	bool IsPowerOfTwo(int n);
	int GetMin(int a, int b);
	DoubleBuffer* CalculateN(DoubleBuffer* x);
	DoubleBuffer* AutoCorrelateN(DoubleBuffer* x);



};

#endif	//__PITCHDETECTION_H__
