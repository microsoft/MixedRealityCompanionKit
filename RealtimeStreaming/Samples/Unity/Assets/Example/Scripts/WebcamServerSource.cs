// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using RealtimeStreaming;
using System.Diagnostics;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;

public class WebcamServerSource : RealtimeServerSource
{
    public RawImage debugImg;

    private WebCamTexture webcam;

    private Color32[] webcam_interop;
    private byte[] frameBuffer;

    private float timer = 0.0f;
    private float WRITE_FPS = 1.0f / 30.0f;
    private Stopwatch stopwatch = new Stopwatch();

    private void Start()
    {
        if (Server == null)
        {
            Server = GetComponent<RealtimeVideoServer>();
        }

        this.Server.StartListening();

        StartWebCam();
    }

    private void Update()
    {
        if (this.Server.CurrentState == RealtimeVideoServer.ServerState.Ready)
        {
            timer += Time.deltaTime;

            if (timer > WRITE_FPS)
            {
                timer = 0;
                this.CaptureWebcam();
            }
        }
    }

    private void StartWebCam()
    {
        if (webcam != null)
        {
            return;
        }

        webcam = new WebCamTexture((int)Server.OutputWidth, (int)Server.OutputHeight);
        debugImg.texture = webcam;
        debugImg.material.mainTexture = webcam;
        webcam.Play();

        UnityEngine.Debug.Log("Webcam Playing at " + webcam.width + " x " + webcam.height);

        if (webcam.width != Server.OutputWidth || webcam.height != Server.OutputHeight)
        {
            UnityEngine.Debug.LogWarning("Requested webcam resolution (" + webcam.width + " x " + webcam.height + ") does not match server output " + "(" + Server.OutputWidth + " x " + Server.OutputHeight + ")");
            UnityEngine.Debug.LogWarning("Updating server to match it...");
            Server.OutputWidth = (uint)webcam.width;
            Server.OutputHeight = (uint)webcam.height;
        }

        webcam_interop = new Color32[webcam.width * webcam.height];
        frameBuffer = new byte[webcam.width * webcam.height * 4];

        stopwatch.Start();
    }

    private void CaptureWebcam()
    {
        if (webcam == null && !webcam.isPlaying)
        {
            return;
        }

        webcam.GetPixels32(webcam_interop);

        // Parrelize copy of Colo32 webcam data into framebuffer
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
        this.FrameRate = 1000.0f / stopwatch.ElapsedMilliseconds;
        stopwatch.Reset();
        stopwatch.Start();
    }
}
