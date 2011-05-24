/**
 * Name        : GuitarTuner
 * Version     : 
 * Vendor      : Kaan Yamanyar
 * Description : Guitar Tuner for bada
 */

#include "ChromaticTuner.h"
#include "GTuner.h"
//#include "PitchDetection.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::System;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Base::Utility;

ChromaticTuner::ChromaticTuner() {
	_pAudioManager = null;
	_pGTuner = null;
}

ChromaticTuner::~ChromaticTuner() {
	if (_pAudioManager != null) {
		_pAudioManager->StopRecord();
		delete _pAudioManager;
		_pAudioManager = null;
	}
}

Application*
ChromaticTuner::CreateInstance(void) {
	// Create the instance through the constructor.
	ChromaticTuner* g = new ChromaticTuner();
	return g;

}

bool ChromaticTuner::OnAppInitializing(AppRegistry& appRegistry) {
	// TODO:
	// Initialize UI resources and application specific data.
	// The application's permanent data and context can be obtained from the appRegistry.
	//
	// If this method is successful, return true; otherwise, return false.
	// If this method returns false, the application will be terminated.

	// Uncomment the following statement to listen to the screen on/off events.
	//PowerManager::SetScreenEventListener(*this);

	// Create a form
	_pGTuner = new GTuner();
	_pGTuner->Initialize();

	// Add the form to the frame
	Frame *pFrame = GetAppFrame()->GetFrame();
	pFrame->AddControl(*_pGTuner);

	// Set the current form
	pFrame->SetCurrentForm(*_pGTuner);

	// Draw and Show the form
	_pGTuner->Draw();
	_pGTuner->Show();

	return true;
}

bool ChromaticTuner::OnAppTerminating(AppRegistry& appRegistry,
		bool forcedTermination) {
	if (_pAudioManager != null) {
		delete _pAudioManager;
		_pAudioManager = null;
	}

	// TODO:
	// Deallocate resources allocated by this application for termination.
	// The application's permanent data and context can be saved via appRegistry.
	return true;
}

void ChromaticTuner::OnForeground(void) {
	//AppLog("OnForeGround!!!!!!!!!");
	if (_pAudioManager != null) {
		delete _pAudioManager;
		_pAudioManager = null;
	}
	_pAudioManager = new AudioManager(_pGTuner);
	_pAudioManager->Construct();

	_pAudioManager->StartRecord();
	/*
	 * For Testing Uncomment this, expected value is 861.328

	 ShortBuffer* s = GenerateDTMF(1000,440);
	 ByteBuffer* b = new ByteBuffer();
	 b->Construct(2000);
	 s->SetPosition(0);
	 b->SetPosition(0);
	 while (b->HasRemaining()) {
	 short ss;
	 s->Get(ss);
	 b->SetShort(ss);
	 }
	 FftAlgorithm* p = new FftAlgorithm();
	 double d = p->ProcessData(b);
	 AppLog("Val: %f",d);

	 delete s;
	 delete b;
	 delete p;
	 */

}

void ChromaticTuner::OnBackground(void) {
	//AppLog("OnBACKGround!!!!!!!!!");
	if (_pAudioManager != null) {
		_pAudioManager->StopRecord();
		//_pAudioManager->AudioInStop(); //no need deletion will stop it
		delete _pAudioManager;
		_pAudioManager = null;
	}
}

void ChromaticTuner::OnLowMemory(void) {
	// TODO:
	// Free unused resources or close the application.
}

void ChromaticTuner::OnBatteryLevelChanged(BatteryLevel batteryLevel) {
	// TODO:
	// Handle any changes in battery level here.
	// Stop using multimedia features(camera, mp3 etc.) if the battery level is CRITICAL.
}

void ChromaticTuner::OnScreenOn(void) {
	// TODO:
	// Get the released resources or resume the operations that were paused or stopped in OnScreenOff().
}

void ChromaticTuner::OnScreenOff(void) {
	// TODO:
	//  Unless there is a strong reason to do otherwise, release resources (such as 3D, media, and sensors) to allow the device to enter the sleep mode to save the battery.
	// Invoking a lengthy asynchronous method within this listener method can be risky, because it is not guaranteed to invoke a callback before the device enters the sleep mode.
	// Similarly, do not perform lengthy operations in this listener method. Any operation must be a quick one.
}

//this is for testing purposes
ShortBuffer*
ChromaticTuner::GenerateDTMF(int length, float freq1) {
	int PLAYBACKFREQ = 44100;
	float PI2 = 3.1415926535897932384626433832795f * 2;
	int i;
	ShortBuffer* dest = new ShortBuffer();
	dest->Construct(length);
	for (i = 0; i < length; i++) {
		double jj = Math::Sin(i * (PI2 * (PLAYBACKFREQ / freq1)));
		short y = (short) (jj * 16383);
		dest->Set(i, y);
	}
	return dest;
}
