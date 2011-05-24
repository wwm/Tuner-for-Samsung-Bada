#include "GTuner.h"
#include "Settings.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Media;
using namespace Osp::Graphics;
using namespace Osp::Base::Utility;

GTuner::GTuner(void) :
	__pBadaBitmap(null), __pBadaBitmapHand(null), __pBadaBitmapDetail(null),
			frequency(0), scale(0) {
	ToneStep = Math::Log10(Math::Pow(2, 1.0 / 12));
}

GTuner::~GTuner(void) {
}

void GTuner::Update(double newValue) {
	this->frequency = newValue;
	RequestRedraw();
}

result GTuner::OnDraw(void) {
	result r = E_SUCCESS;
	if (frequency == 0)
		this->scale = 0;
	else
		this->scale = Math::Log10(frequency / 27.5) / ToneStep;

	//AppLog("Freq: %f Scale: %f",frequency,scale);
	Osp::Base::String scaleAsText = this->GetScale(Math::Round(this->scale));
	double noteDetail = (this->scale - Math::Round(this->scale) + 0.5) * 200;

	while (this->scale >= 12)
		this->scale = this->scale - 12;
	double regular_degree = (360.0 / 12) * (this->scale);
	double degree = regular_degree * (PI / 180.0);
	Canvas* pCanvas = GetCanvasN();
	if (pCanvas != null) {

		//prepare points
		Rectangle rt = GetClientAreaBounds();
		Point topLeft = rt.GetTopLeft();
		Point bottomRight = rt.GetBottomRight();
		Point mainFrameTopLeft(0, 0);
		int dimension = 420;

		OrientationStatus status = GetOrientationStatus();
		if (status == ORIENTATION_STATUS_PORTRAIT || status
				== ORIENTATION_STATUS_PORTRAIT_REVERSE) {
			mainFrameTopLeft.x = topLeft.x + 30;
			mainFrameTopLeft.y = topLeft.y + 90;
		} else if (status == ORIENTATION_STATUS_LANDSCAPE || status
				== ORIENTATION_STATUS_LANDSCAPE_REVERSE) {
			mainFrameTopLeft.x = topLeft.x + 30;
			mainFrameTopLeft.y = topLeft.y + 10;
			dimension = 380;
		}

		Rectangle mainFrame(mainFrameTopLeft, Dimension(dimension, dimension));
		pCanvas->DrawBitmap(mainFrame, *__pBadaBitmap);

		//pCanvas->DrawRectangle(mainFrame);
		//	pCanvas->DrawRectangle(scaleArcBox);
		{
			int yp = Math::Sin(degree) * (dimension / 2);
			int xp = Math::Cos(degree) * (dimension / 2);
			Point middleOfMainFrame(mainFrameTopLeft.x + (dimension / 2),
					mainFrame.y + (dimension / 2));
			Point destinationPoint(middleOfMainFrame.x + xp,
					middleOfMainFrame.y - yp);
			//	pCanvas->DrawLine(middleOfMainFrame, destinationPoint);
			pCanvas->DrawBitmap(middleOfMainFrame, *__pBadaBitmapHand, Point(
					15, 13), 90 - regular_degree);//17,14

			//set font
			Font font;
			font.Construct(FONT_STYLE_PLAIN, 36);
			pCanvas->SetFont(font);
			Point detailBox;
			if (status == ORIENTATION_STATUS_PORTRAIT || status
					== ORIENTATION_STATUS_PORTRAIT_REVERSE) {
				pCanvas->DrawText(Point(middleOfMainFrame.x,
						middleOfMainFrame.y + 230), scaleAsText);
				detailBox = Point(middleOfMainFrame.x - 125,
						middleOfMainFrame.y + 270);



			} else if (status == ORIENTATION_STATUS_LANDSCAPE || status
					== ORIENTATION_STATUS_LANDSCAPE_REVERSE) {
				pCanvas->DrawText(Point(middleOfMainFrame.x + 400,
						middleOfMainFrame.y - 20), scaleAsText);
				detailBox = Point(middleOfMainFrame.x + 275,
						middleOfMainFrame.y + 30);



			}


			pCanvas->DrawBitmap(Rectangle(detailBox, Dimension(250, 87)),
					*__pBadaBitmapDetail);
			pCanvas->SetForegroundColor(Color::COLOR_BLACK);
			noteDetail += 25; //correction for img fit
			pCanvas->DrawLine(Point(detailBox.x + noteDetail, detailBox.y+9),
					Point(detailBox.x + noteDetail, detailBox.y + 78));




		}

		delete pCanvas;
	}

	return r;

}

bool GTuner::Initialize(void) {
	Form::Construct(FORM_STYLE_NORMAL | FORM_STYLE_TITLE | FORM_STYLE_INDICATOR
			| FORM_STYLE_SOFTKEY_1);
	SetName(L"IDF_GTuner");
	SetTitleText(L"Tuner");
	SetSoftkeyText(SOFTKEY_1, L"Back");
	SetSoftkeyActionId(SOFTKEY_1, ID_EXIT);
	AddSoftkeyActionListener( SOFTKEY_1, *this);
	SetBackgroundColor(Color(0,0,0,0));

	return true;
}

Osp::Base::String GTuner::GetScale(int rounded) {

	if (rounded < 15)
		return "?";
	if (rounded > 62)
		return "?";

	switch (rounded) {
	case 15:
		return "C"; //c1 octave 1 starts
	case 16:
		return "C#";
	case 17:
		return "D";
	case 18:
		return "D#";
	case 19:
		return "E";
	case 20:
		return "F";
	case 21:
		return "F#";
	case 22:
		return "G";
	case 23:
		return "G#";
	case 24:
		return "A";
	case 25:
		return "A#";
	case 26:
		return "B";
	case 27:
		return "C"; //c2 octave 2 starts
	case 28:
		return "C#";
	case 29:
		return "D";
	case 30:
		return "D#";
	case 31:
		return "E";
	case 32:
		return "F";
	case 33:
		return "F#";
	case 34:
		return "G";
	case 35:
		return "G#";
	case 36:
		return "A";
	case 37:
		return "A#";
	case 38:
		return "B";
	case 39:
		return "C"; //c3 octave 3 starts
	case 40:
		return "C#";
	case 41:
		return "D";
	case 42:
		return "D#";
	case 43:
		return "E";
	case 44:
		return "F";
	case 45:
		return "F#";
	case 46:
		return "G";
	case 47:
		return "G#";
	case 48:
		return "A";
	case 49:
		return "A#";
	case 50:
		return "B";
	case 51:
		return "C";// c4 octave 4 begins
	case 52:
		return "C#";
	case 53:
		return "D";
	case 54:
		return "D#";
	case 55:
		return "E";
	case 56:
		return "F";
	case 57:
		return "F#";
	case 58:
		return "G";
	case 59:
		return "G#";
	case 60:
		return "A";
	case 61:
		return "A#";
	case 62:
		return "B";
	}

	return "?";

}

result GTuner::OnInitializing(void) {
	result r = E_SUCCESS;

	SetOrientation(ORIENTATION_AUTOMATIC_FOUR_DIRECTION);
	AddOrientationEventListener(*this);

	// Crate a Bitmap
	Image image;
	image.Construct();
	__pBadaBitmap = image.DecodeN(L"/Res/background.png", BITMAP_PIXEL_FORMAT_ARGB8888);

	Image image2;
	image2.Construct();
	__pBadaBitmapHand = image2.DecodeN(L"/Res/foreground.png",BITMAP_PIXEL_FORMAT_ARGB8888);

	Image image3;
	image3.Construct();
	__pBadaBitmapDetail = image3.DecodeN(L"/Res/kucuk_kutu.png",BITMAP_PIXEL_FORMAT_ARGB8888);

	return r;
}

result GTuner::OnTerminating(void) {
	result r = E_SUCCESS;

	delete __pBadaBitmap;
	delete __pBadaBitmapHand;
	delete __pBadaBitmapDetail;
	return r;
}

void GTuner::OnOrientationChanged(const Osp::Ui::Control &source,
		Osp::Ui::OrientationStatus orientationStatus) {
	RequestRedraw();
}

void GTuner::OnActionPerformed(const Osp::Ui::Control& source, int actionId) {
	Osp::App::Application::GetInstance()->Terminate();
}
