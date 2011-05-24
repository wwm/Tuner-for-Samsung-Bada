#ifndef __CHROMATICTUNER_H__
#define __CHROMATICTUNER_H__

#include <FApp.h>
#include <FBase.h>
#include <FGraphics.h>
#include <FSystem.h>
#include <FUi.h>
#include <FBaseByteBuffer.h>
#include <FIo.h>
#include "AudioManager.h"

using namespace Osp::Io;


/**
 * [GuitarTuner] application must inherit from Application class
 * which provides basic features necessary to define an application.
 */
class ChromaticTuner :
	public Osp::App::Application,
	public Osp::System::IScreenEventListener
{
private:
	AudioManager* _pAudioManager;
	GTuner *_pGTuner;
public:

	/**
	 * [GuitarTuner] application must have a factory method that creates an instance of itself.
	 */
	static Osp::App::Application* CreateInstance(void);


public:
	ChromaticTuner();
	~ChromaticTuner();


public:

	//for testing purposes
	Osp::Base::ShortBuffer* GenerateDTMF(int length, float freq1);


	// Called when the application is initializing.
	bool OnAppInitializing(Osp::App::AppRegistry& appRegistry);

	// Called when the application is terminating.
	bool OnAppTerminating(Osp::App::AppRegistry& appRegistry, bool forcedTermination = false);


	// Called when the application's frame moves to the top of the screen.
	void OnForeground(void);


	// Called when this application's frame is moved from top of the screen to the background.
	void OnBackground(void);

	// Called when the system memory is not sufficient to run the application any further.
	void OnLowMemory(void);

	// Called when the battery level changes...
	void OnBatteryLevelChanged(Osp::System::BatteryLevel batteryLevel);

	// Called when the screen turns on.
	void OnScreenOn (void);

	// Called when the screen turns off.
	void OnScreenOff (void);

};

#endif
