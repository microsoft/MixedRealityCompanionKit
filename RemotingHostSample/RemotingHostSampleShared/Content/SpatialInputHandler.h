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

#pragma once
#include <vector>

namespace RemotingHostSample
{
    // Sample gesture handler.
    // Hooks up events to recognize a tap gesture, and keeps track of input using a boolean value.
    class SpatialInputHandler
    {
    public:
        SpatialInputHandler();
        ~SpatialInputHandler();

        enum class ManipulationResult
        {
            Unknown = 0,
            Completed,
            Canceled,
        };

        Windows::UI::Input::Spatial::SpatialTappedEventArgs^ CheckForTapped();
        Windows::UI::Input::Spatial::SpatialManipulationStartedEventArgs^ CheckForManipulationStarted();
        Windows::UI::Input::Spatial::SpatialManipulationUpdatedEventArgs^ CheckForManipulationUpdated();
        ManipulationResult CheckForManipulationResult();

    private:
        // API objects used to process gesture input, and generate gesture events.
        Windows::UI::Input::Spatial::SpatialInteractionManager^             m_interactionManager;
        Windows::UI::Input::Spatial::SpatialGestureRecognizer^              m_gestureRecognizer;

        // Event registration token.
        Windows::Foundation::EventRegistrationToken                         m_interactionDetectedEventToken;
        Windows::Foundation::EventRegistrationToken                         m_tappedEventToken;
        Windows::Foundation::EventRegistrationToken                         m_manipulationStartedEventToken;
        Windows::Foundation::EventRegistrationToken                         m_manipulationUpdatedEventToken;
        Windows::Foundation::EventRegistrationToken                         m_manipulationCompletedEventToken;
        Windows::Foundation::EventRegistrationToken                         m_manipulationCanceledEventToken;

        // Used to indicate that a Pressed input event was received this frame.
        Microsoft::WRL::Wrappers::CriticalSection                           m_manipulationStateLock;
        Windows::UI::Input::Spatial::SpatialTappedEventArgs^                m_tapped = nullptr;
        Windows::UI::Input::Spatial::SpatialManipulationStartedEventArgs^   m_manipulationStarted = nullptr;
        Windows::UI::Input::Spatial::SpatialManipulationUpdatedEventArgs^   m_manipulationUpdated = nullptr;
        ManipulationResult                                                  m_manipulationResult = ManipulationResult::Unknown;
    };
}
