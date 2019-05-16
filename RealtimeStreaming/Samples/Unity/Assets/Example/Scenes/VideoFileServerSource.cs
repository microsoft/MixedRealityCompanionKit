// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using RealtimeStreaming;
using System.Diagnostics;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Video;

public class VideoFileServerSource : RealtimeServerSource
{
    public VideoPlayer videoPlayer;
    public RawImage debugImg;
    public Texture2D videoFrame;

    private Color32[] webcam_interop;
    private byte[] frameBuffer;
    private Stopwatch stopwatch = new Stopwatch();

    private void Start()
    {
        if (Server == null)
        {
            Server = GetComponent<RealtimeVideoServer>();
        }

        this.Server.ServerStateChanged += this.OnServerStateChanged;
        this.Server.StartListening();

        if (videoPlayer == null)
        {
            videoPlayer = GetComponent<VideoPlayer>();
        }

        // Note: Frame ready events are very expensive by turning this on
        videoPlayer.sendFrameReadyEvents = true;
        videoPlayer.frameReady += VideoPlayer_frameReady;
        videoPlayer.prepareCompleted += VideoPlayer_prepareCompleted;
        videoPlayer.Prepare();
    }

    private void OnServerStateChanged(object sender, StateChangedEventArgs<RealtimeVideoServer.ServerState> e)
    {
        PluginUtils.ExecuteOnUnityThread(() =>
        {
            if (e.CurrentState == RealtimeVideoServer.ServerState.Ready)
            {
                UnityEngine.Debug.Log("Server State changed to Ready - Starting Unity VideoPlayer");

                videoPlayer.Play();
                videoPlayer.Prepare();

                stopwatch.Start();
            }
        });
    }

    private void VideoPlayer_prepareCompleted(VideoPlayer source)
    {
        webcam_interop = new Color32[source.width * source.height];
        frameBuffer = new byte[source.width * source.height * 4];
        videoFrame = new Texture2D((int)source.width, (int)source.height);

        Server.OutputWidth = source.width;
        Server.OutputHeight = source.height;
        debugImg.texture = videoFrame;
    }

    private void VideoPlayer_frameReady(VideoPlayer source, long frameIdx)
    {
        // Texture is on GPU
        RenderTexture renderTexture = source.texture as RenderTexture;

        if (videoFrame.width != renderTexture.width || videoFrame.height != renderTexture.height)
        {
            UnityEngine.Debug.LogError("VideoPlayer texture changed resolution. Streaming does not support resize. Shutting down...");
            stopwatch.Stop();
            videoPlayer.Stop();
            Server.Shutdown();
            return;
        }

        RenderTexture.active = renderTexture;
        videoFrame.ReadPixels(new Rect(0, 0, renderTexture.width, renderTexture.height), 0, 0);
        videoFrame.Apply();
        RenderTexture.active = null;

        webcam_interop = videoFrame.GetPixels32();

        // Parraelize copy of Color32 webcam data into framebuffer
        Parallel.For(0, webcam_interop.Length,
            index =>
            {
                int byteIdx = index << 2; // multiply by 4
                Color32 c = webcam_interop[webcam_interop.Length - index - 1];

                frameBuffer[byteIdx] = c.b;
                frameBuffer[byteIdx + 1] = c.g;
                frameBuffer[byteIdx + 2] = c.r;
                frameBuffer[byteIdx + 3] = c.a;
            });

        this.Server.WriteFrame(this.frameBuffer);

        stopwatch.Stop();
        this.FrameRate =  1000.0f / stopwatch.ElapsedMilliseconds;
        stopwatch.Reset();
        stopwatch.Start();
    }
}
