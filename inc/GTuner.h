#ifndef _GTUNER_H_
#define _GTUNER_H_

#include <FBase.h>
#include <FUi.h>
#include <FMedia.h>
#include <FApp.h>


class GTuner :
	public Osp::Ui::Controls::Form,
	public Osp::Ui::IActionEventListener,
	public Osp::Ui::IOrientationEventListener
{
private:
	double frequency;
	double scale;
	double ToneStep;



protected:
	static const int ID_EXIT = 100;

// Construction
public:
	GTuner(void);
	virtual ~GTuner(void);
	bool Initialize(void);

protected:
	Osp::Graphics::Bitmap *__pBadaBitmap;
	Osp::Graphics::Bitmap *__pBadaBitmapHand;
	Osp::Graphics::Bitmap *__pBadaBitmapDetail;

public:
	void Update(double newValue);
	Osp::Base::String GetScale(int rounded);
	virtual result OnDraw(void);
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	virtual void OnOrientationChanged(const Osp::Ui::Control &source, Osp::Ui::OrientationStatus orientationStatus);
};

#endif	//_GTUNER_H_
