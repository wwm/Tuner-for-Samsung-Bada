//includes
#include "AudioManager.h"
#include "PitchDetection.h"
#include "Settings.h"
static ByteBuffer* __pByteBuffer1 = null;
static ByteBuffer* __pByteBuffer2 = null;
static ByteBuffer* __pByteBuffer3 = null;
static ByteBuffer* __pByteBuffer4 = null;
static AudioIn* __pAudioInInstance = null;
static AudioListener* __pMyAudioInListener = null;
static FftAlgorithm* __pitchDetection = null;

void ConsumeBuffer(ByteBuffer* pBuffer);

AudioManager::AudioManager(GTuner* tuner) {
	this->_pTunerView = tuner;
}

AudioListener::AudioListener(GTuner* tuner,AudioManager* manager) {
	this->_pTunerView = tuner;
	this->_pManager = manager;
}

result AudioManager::Construct() {

	__pMyAudioInListener = new AudioListener(_pTunerView,this);
	if (__pMyAudioInListener == null)
		return E_OUT_OF_MEMORY;

	__pitchDetection = new FftAlgorithm();

	return E_SUCCESS;
}

AudioManager::~AudioManager() {
	StopRecord();

	if (__pMyAudioInListener != null) {
		delete __pMyAudioInListener;
		__pMyAudioInListener = null;
	}

	if (__pitchDetection != null) {
		delete __pitchDetection;
		__pitchDetection = null;
	}

}

void AudioManager::StartRecord(void) {
	// Initialize the AudioIn
	if (__pAudioInInstance != null) {
		delete __pAudioInInstance;
		__pAudioInInstance = null;
	}
	__pAudioInInstance = new AudioIn();
	/*if (__pAudioInInstance == null)
		return E_OUT_OF_MEMORY;*/

	// Construct an AudioIn instance
	__pAudioInInstance->Construct(*__pMyAudioInListener);

	// Prepare an AudioIn instance
	__pAudioInInstance->Prepare(AUDIO_INPUT_DEVICE_MIC, AUDIO_TYPE_PCM_S16_LE,
			AUDIO_CHANNEL_TYPE_MONO, SAMPLE_RATE);

	// Prepare buffers to store PCM data
	__pByteBuffer1 = new ByteBuffer();
	__pByteBuffer1->Construct(MAX_BUFFER_SIZE);
	__pByteBuffer2 = new ByteBuffer();
	__pByteBuffer2->Construct(MAX_BUFFER_SIZE);
	__pByteBuffer3 = new ByteBuffer();
	__pByteBuffer3->Construct(MAX_BUFFER_SIZE);
	__pByteBuffer4 = new ByteBuffer();
	__pByteBuffer4->Construct(MAX_BUFFER_SIZE);

	// Add buffers
	__pAudioInInstance->AddBuffer(__pByteBuffer1);
	__pAudioInInstance->AddBuffer(__pByteBuffer2);
	__pAudioInInstance->AddBuffer(__pByteBuffer3);
	__pAudioInInstance->AddBuffer(__pByteBuffer4);
	// Start
	__pAudioInInstance->Start();
}

void AudioManager::StopRecord(void) {
	if (__pAudioInInstance != null) {

		try {
			__pAudioInInstance->Stop();
		} catch (int w) {
			//it might be stopped before
		}
		try {
			__pAudioInInstance->Unprepare();
		} catch (int w) {
			//it might be stopped before
		}
	}
	if (__pByteBuffer1 != null) {
		delete __pByteBuffer1;
		__pByteBuffer1 = null;
	}
	if (__pByteBuffer2 != null) {
		delete __pByteBuffer2;
		__pByteBuffer2 = null;
	}
	if (__pByteBuffer3 != null) {
		delete __pByteBuffer3;
		__pByteBuffer3 = null;
	}
	if (__pByteBuffer4 != null) {
		delete __pByteBuffer4;
		__pByteBuffer4 = null;
	}

	if (__pAudioInInstance != null) {
		delete __pAudioInInstance;
		__pAudioInInstance = null;
	}

}

void AudioListener::OnAudioInBufferIsFilled(Osp::Base::ByteBuffer* pData) {

	double frequence = __pitchDetection->ProcessData(pData);
	if (frequence != 0)
		_pTunerView->Update(frequence);

	pData->Clear();
	__pAudioInInstance->AddBuffer(pData);

}

void AudioListener::OnAudioInErrorOccurred(result r) {
	this->_pManager->StopRecord();
}

void AudioListener::OnAudioInInterrupted(void) {
	this->_pManager->StopRecord();
}

void AudioListener::OnAudioInReleased(void) {
	this->_pManager->StartRecord();
}

