#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

//include
#include <FBase.h>
#include <FMedia.h>
#include <FIo.h>
#include <FBaseByteBuffer.h>
#include "GTuner.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Base::Runtime;
using namespace Osp::Media;
using namespace Osp::Io;

class GuitarTuner;
class AudioManager;

class AudioListener: public Osp::Media::IAudioInEventListener {
private:
	GTuner* _pTunerView;
	AudioManager* _pManager;
public:
	AudioListener(GTuner* tunerView,AudioManager* manager);
public:
	void OnAudioInBufferIsFilled(Osp::Base::ByteBuffer* pData);
	void OnAudioInErrorOccurred(result r);
	void OnAudioInInterrupted();
	void OnAudioInReleased();

};

class AudioManager {
private:
	GTuner* _pTunerView;
public:
	AudioManager(GTuner* tunerView);
	virtual ~AudioManager(void);

	// Example Operation
public:
	void StartRecord(void);
	void StopRecord(void);
	// This method must be called only once after new is called!
	result Construct(void);

};

#endif
