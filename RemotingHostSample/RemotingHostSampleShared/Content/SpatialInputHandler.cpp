//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "pch.h"
#include "SpatialInputHandler.h"
#include <functional>

using namespace RemotingHostSample;

using namespace Windows::Foundation;
using namespace Windows::UI::Input::Spatial;
using namespace std::placeholders;

// Creates and initializes a GestureRecognizer that listens to a Person.
SpatialInputHandler::SpatialInputHandler()
{
    // The interaction manager provides an event that informs the app when
    // spatial interactions are detected.
    m_interactionManager = SpatialInteractionManager::GetForCurrentView();
    m_gestureRecognizer = ref new SpatialGestureRecognizer(SpatialGestureSettings::Tap |
                                                           SpatialGestureSettings::ManipulationTranslate);

    m_interactionDetectedEventToken =
        m_interactionManager->InteractionDetected +=
        ref new TypedEventHandler<SpatialInteractionManager^, SpatialInteractionDetectedEventArgs^>(
            [this](SpatialInteractionManager^, SpatialInteractionDetectedEventArgs^ args)
    { m_gestureRecognizer->CaptureInteraction(args->Interaction); });

    m_tappedEventToken =
        m_gestureRecognizer->Tapped +=
        ref new TypedEventHandler<SpatialGestureRecognizer^, SpatialTappedEventArgs^>(
            [this](SpatialGestureRecognizer^, SpatialTappedEventArgs^ args)
    { auto lock = m_manipulationStateLock.Lock(); m_tapped = args; });

    m_manipulationStartedEventToken =
        m_gestureRecognizer->ManipulationStarted +=
        ref new TypedEventHandler<SpatialGestureRecognizer^, SpatialManipulationStartedEventArgs^>(
            [this](SpatialGestureRecognizer^, SpatialManipulationStartedEventArgs^ args)
    { auto lock = m_manipulationStateLock.Lock(); m_manipulationStarted = args; });

    m_manipulationUpdatedEventToken =
        m_gestureRecognizer->ManipulationUpdated +=
        ref new TypedEventHandler<SpatialGestureRecognizer^, SpatialManipulationUpdatedEventArgs^>(
            [this](SpatialGestureRecognizer^, SpatialManipulationUpdatedEventArgs^ args)
    { auto lock = m_manipulationStateLock.Lock(); m_manipulationUpdated = args; });

    m_manipulationCompletedEventToken =
        m_gestureRecognizer->ManipulationCompleted +=
        ref new TypedEventHandler<SpatialGestureRecognizer^, SpatialManipulationCompletedEventArgs^>(
            [this](SpatialGestureRecognizer^, SpatialManipulationCompletedEventArgs^)
    { auto lock = m_manipulationStateLock.Lock(); m_manipulationResult = ManipulationResult::Completed; });

    m_manipulationCanceledEventToken =
        m_gestureRecognizer->ManipulationCanceled +=
        ref new TypedEventHandler<SpatialGestureRecognizer^, SpatialManipulationCanceledEventArgs^>(
            [this](SpatialGestureRecognizer^, SpatialManipulationCanceledEventArgs^)
    { auto lock = m_manipulationStateLock.Lock(); m_manipulationResult = ManipulationResult::Canceled; });
}

SpatialInputHandler::~SpatialInputHandler()
{
    // Unregister our handler for the OnSourcePressed event.
    m_interactionManager->InteractionDetected -= m_interactionDetectedEventToken;
    m_gestureRecognizer->ManipulationStarted -= m_manipulationStartedEventToken;
    m_gestureRecognizer->ManipulationUpdated -= m_manipulationUpdatedEventToken;
    m_gestureRecognizer->ManipulationCompleted -= m_manipulationCompletedEventToken;
    m_gestureRecognizer->ManipulationCanceled -= m_manipulationCanceledEventToken;
}

// Checks if the user performed an input gesture since the last call to this method.
// Allows the main update loop to check for asynchronous changes to the user
// input state.
Windows::UI::Input::Spatial::SpatialTappedEventArgs ^ RemotingHostSample::SpatialInputHandler::CheckForTapped()
{
    auto lock = m_manipulationStateLock.Lock();
    auto tapped = m_tapped;
    m_tapped = nullptr;
    return tapped;
}

SpatialManipulationStartedEventArgs^ SpatialInputHandler::CheckForManipulationStarted()
{
    auto lock = m_manipulationStateLock.Lock();
    auto manipulationStarted = m_manipulationStarted;
    m_manipulationStarted = nullptr;
    return manipulationStarted;
}

SpatialManipulationUpdatedEventArgs^ SpatialInputHandler::CheckForManipulationUpdated()
{
    auto lock = m_manipulationStateLock.Lock();
    auto manipulationUpdated = m_manipulationUpdated;
    m_manipulationUpdated = nullptr;
    return manipulationUpdated;
}

SpatialInputHandler::ManipulationResult SpatialInputHandler::CheckForManipulationResult()
{
    auto lock = m_manipulationStateLock.Lock();
    auto manipulationResult = m_manipulationResult;
    m_manipulationResult = ManipulationResult::Unknown;
    return manipulationResult;
}
