#include "PitchDetection.h"
#include "Settings.h"




FftAlgorithm::FftAlgorithm() {


};

FftAlgorithm::~FftAlgorithm(){
};

/// <summary>
/// Gets number of significat bytes.
/// </summary>
/// <param name="n">Number</param>
/// <returns>Amount of minimal bits to store the number.</returns>
int FftAlgorithm::Log2(int n) {
	int i = 0;
	while (n > 0) {
		++i;
		n >>= 1;
	}
	return i;
}

/// <summary>
/// Reverses bits in the number.
/// </summary>
/// <param name="n">Number</param>
/// <param name="bitsCount">Significant bits in the number.</param>
/// <returns>Reversed binary number.</returns>
int FftAlgorithm::ReverseBits(int n, int bitsCount) {
	int reversed = 0;
	for (int i = 0; i < bitsCount; i++) {
		int nextBit = n & 1;
		n >>= 1;

		reversed <<= 1;
		reversed |= nextBit;
	}
	return reversed;
}

/// <summary>
/// Checks if number is power of 2.
/// </summary>
/// <param name="n">number</param>
/// <returns>true if n=2^k and k is positive integer</returns>
bool FftAlgorithm::IsPowerOfTwo(int n) {
	return n > 1 && (n & (n - 1)) == 0;
}
DoubleBuffer* FftAlgorithm::AutoCorrelateN(DoubleBuffer* x) {
	DoubleBuffer* ac = new DoubleBuffer();
	ac->Construct(x->GetLimit());
	ac->SetPosition(0);
	double sum;
	int i, j;
	int order = x->GetLimit();
	double jValue, jiValue;
	for (i = 0; i < order; i++) {
		sum = 0;
		for (j = 0; j < order - i; j++) {
			x->Get(j, jValue);
			x->Get(j + 1, jiValue);
			sum += jValue * jiValue;
		}
		ac->Set(i,sum);
	}
	ac->SetPosition(0);
	return ac;

}
/// <summary>
/// Calculates FFT using Cooley-Tukey FFT algorithm.
/// </summary>
/// <param name="x">input data</param>
/// <returns>spectrogram of the data</returns>
/// <remarks>
/// If amount of data items not equal a power of 2, then algorithm
/// automatically pad with 0s to the lowest amount of power of 2.
/// </remarks>
DoubleBuffer* FftAlgorithm::CalculateN(DoubleBuffer* x) {
	int length;
	int bitsInLength;
	if (IsPowerOfTwo(x->GetLimit())) {
		length = x->GetLimit();
		bitsInLength = Log2(length) - 1;
	} else {
		bitsInLength = Log2(x->GetLimit());
		length = 1 << bitsInLength;
		// the items will be pad with zeros
	}

	// bit reversal
	ComplexNumber* data = new ComplexNumber[length];
	for (int i = 0; i < x->GetLimit(); i++) {
		int j = ReverseBits(i, bitsInLength);
		double q;
		x->Get(q);
		data[j] = ComplexNumber(q,0);

	}

	// Cooley-Turkey
	for (int i = 0; i < bitsInLength; i++) {
		int m = 1 << i;
		int n = m * 2;
		double alpha = -(2 * PI / n);

		for (int k = 0; k < m; k++) {
			// e^(-2*pi*i/N*k)
			ComplexNumber oddPartMultiplier(0,alpha * k);
			oddPartMultiplier = oddPartMultiplier.PoweredE();

			for (int j = k; j < length; j += n) {
				ComplexNumber evenPart = data[j];
				ComplexNumber oddPart = oddPartMultiplier * data[j + m];
				data[j] = evenPart + oddPart;
				data[j + m] = evenPart - oddPart;
			}
		}
	}

	// calculate spectrogram
	DoubleBuffer* spectrogram = new DoubleBuffer();
	spectrogram->Construct(length);

	for (int i = 0; i < spectrogram->GetLimit(); i++) {
		double z;
		z = data[i].AbsPower2();

		spectrogram->Set(i, z);
	}

	delete[] data;
	data = null;
	return spectrogram;
}

int FftAlgorithm::GetMin(int a, int b) {
	if (a>b) return b; else return a;
}

double FftAlgorithm::ProcessData(ByteBuffer* data) {
	DoubleBuffer* doubleBuffer = new DoubleBuffer();

	doubleBuffer->Construct(data->GetLimit()/2);
	doubleBuffer->SetPosition(0);
	data->SetPosition(0);
	while (data->HasRemaining()) {
		signed short int s;
		data->GetShort(s);
		doubleBuffer->Set(s);
	}
	doubleBuffer->Flip();
	doubleBuffer->SetPosition(0);

	DoubleBuffer* spectr = CalculateN(doubleBuffer);
	delete doubleBuffer;



	int index = 0;
	double max = 0;
	spectr->Get(index,max);
	int spectrumLimit = spectr->GetLimit();
	int usefullMaxSpectr = GetMin(spectrumLimit,(int) (MAX_EXPECTED_FREQ * spectrumLimit / SAMPLE_RATE)+1);
	for (int i = 1; i < usefullMaxSpectr; i++) {
		double c=0;
		spectr->Get(i,c);
		if (max < c) {
			max = c;
			index = i;
		}

	}


	double freq = (double) SAMPLE_RATE * index / spectrumLimit ;

	//bu ikiye bolme tam yalan
	//TODO KONTROL ET
	//freq = freq*2;


	delete spectr;
		spectr=null;

	if (freq < MIN_EXPECTED_FREQ)
		return 0;

	return freq;
}

