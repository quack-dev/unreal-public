// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "Misc/FrameRate.h"
#include "Misc/Timecode.h"

#include "TimecodeProvider.generated.h"

/**
 * Possible states of TimecodeProvider.
 */
UENUM()
enum class ETimecodeProviderSynchronizationState
{
	/** TimecodeProvider has not been initialized or has been shutdown. */
	Closed,

	/** Unrecoverable error occurred during Synchronization. */
	Error,

	/** TimecodeProvider is currently synchronized with the source. */
	Synchronized,

	/** TimecodeProvider is initialized and being prepared for synchronization. */
	Synchronizing,
};

/**
 * A class responsible of fetching a timecode from a source.
 * Note, FApp::GetTimecode and FApp::GetTimecodeFramerate should be used to retrieve
 * the current system Timecode and Framerate.
 */
UCLASS(abstract)
class ENGINE_API UTimecodeProvider : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Number of frames to subtract from the current timecode when GetDelayedTimecode is called.
	 * @see GetDelayedTimecode
	 */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Settings")
	int32 FrameDelay = 0;

	/**
	 * Return the Timecode at that moment. It may not be in sync with the current frame.
	 * Only valid when GetSynchronizationState() is Synchronized.
	 */
	UFUNCTION(BlueprintCallable, Category = "Provider")
	virtual FTimecode GetTimecode() const PURE_VIRTUAL(UTimecodeProvider::GetTimecode, return FTimecode(););

	/**
	 * Return the Timecode with the Frame Delay applied. It may not be in sync with the current frame.
	 * Only valid when GetSynchronizationState() is Synchronized.
	 */
	UFUNCTION(BlueprintCallable, Category = "Provider")
	FTimecode GetDelayedTimecode() const;
	
	/**
	 * Return the frame rate.
	 * Depending on the implementation, it may or may not be valid only when GetSynchronizationState() is Synchronized.
	 */
	UFUNCTION(BlueprintCallable, Category = "Provider")
	virtual FFrameRate GetFrameRate() const PURE_VIRTUAL(UTimecodeProvider::GetFrameRate, return FFrameRate(););

	/** The state of the TimecodeProvider and if it's currently synchronized and the Timecode and FrameRate are valid. */
	UFUNCTION(BlueprintCallable, Category = "Provider")
	virtual ETimecodeProviderSynchronizationState GetSynchronizationState() const PURE_VIRTUAL(UTimecodeProvider::IsSynchronized, return ETimecodeProviderSynchronizationState::Closed;);

public:
	/** This Provider became the Engine's Provider. */
	virtual bool Initialize(class UEngine* InEngine) PURE_VIRTUAL(UTimecodeProvider::Initialize, return false;);

	/** This Provider stopped being the Engine's Provider. */
	virtual void Shutdown(class UEngine* InEngine) PURE_VIRTUAL(UTimecodeProvider::Shutdown, );
};
