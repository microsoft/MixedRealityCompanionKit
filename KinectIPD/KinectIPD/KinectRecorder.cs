// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Microsoft.Kinect.Tools;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace KinectIPD
{
    class KinectRecorder
    {
        private TimeSpan _maxRecordingDuration = TimeSpan.FromSeconds(20);
        public bool IsRecording { get { return _recording; } }
        public bool IsPlaying { get { return _playing; } }
        private volatile bool _recording = false;
        private volatile bool _playing = false;
        private volatile bool _stopRequested = false;

        public void StopRecording()
        {
            _stopRequested = true;
        }

        public void StopPlayback()
        {
            _stopRequested = true;
        }

        public Task PlayClip(string filePath, uint loopCount = 0)
        {
            if (IsRecording || IsPlaying)
                return null;

            _stopRequested = false;
            Task t = Task.Run(() =>
            {
                _playing = true;
                using (KStudioClient client = KStudio.CreateClient())
                {
                    client.ConnectToService();

                    using (KStudioPlayback playback = client.CreatePlayback(filePath))
                    {
                        playback.LoopCount = loopCount;
                        playback.Start();
                        while (playback.State == KStudioPlaybackState.Playing)
                        {
                            if (_stopRequested)
                                playback.Stop();
                            Thread.Sleep(50);
                        }
                    }

                    client.DisconnectFromService();
                }
                Thread.Sleep(100);
                _playing = false;
            });
            return t;
        }

        public Task RecordClip(string filePath)
        {
            return RecordClip(filePath, TimeSpan.Zero);
        }

        public Task RecordClip(string filePath, TimeSpan duration)
        {
            if (IsRecording || IsPlaying)
                return null;

            if (duration == TimeSpan.Zero)
                duration = _maxRecordingDuration;

            _stopRequested = false;
            Task t = Task.Run(() =>
            {
                _recording = true;
                using (KStudioClient client = KStudio.CreateClient())
                {
                    client.ConnectToService();

                    KStudioEventStreamSelectorCollection streamCollection = new KStudioEventStreamSelectorCollection();
                    //streamCollection.Add(KStudioEventStreamDataTypeIds.Ir);
                    streamCollection.Add(KStudioEventStreamDataTypeIds.Depth);
                    streamCollection.Add(KStudioEventStreamDataTypeIds.Body);
                    streamCollection.Add(KStudioEventStreamDataTypeIds.BodyIndex);
                    streamCollection.Add(KStudioEventStreamDataTypeIds.UncompressedColor);

                    using (KStudioRecording recording = client.CreateRecording(filePath, streamCollection))
                    {
                        recording.StartTimed(duration);
                        while (recording.State == KStudioRecordingState.Recording)
                        {
                            if (_stopRequested)
                                recording.Stop();
                            Thread.Sleep(50);
                        }

                        if (recording.State == KStudioRecordingState.Error)
                        {
                            throw new InvalidOperationException("Error: Recording failed!");
                        }
                    }

                    client.DisconnectFromService();
                }
                Thread.Sleep(500);
                _recording = false;
            });
            return t;
        }
    }
}
