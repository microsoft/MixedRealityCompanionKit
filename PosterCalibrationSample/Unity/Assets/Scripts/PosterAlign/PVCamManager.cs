using UnityEngine;
using UnityEngine.VR.WSA.WebCam;

using System;
using System.Collections.Generic;
using System.Linq;

public class PVCamManager : ImprovedSingletonBehavior<PVCamManager>
{
    public bool UseInEditor = false;
    public bool IsAvailableOnPlatform { get; set; }

    public int MinWidth = 1000;

    // Camera Capture
    private PhotoCapture PhotoCap;
    private volatile PhotoCaptureFrame mFrameToProcess = null;
    private volatile PhotoCaptureFrame mFrameBeingProcessed = null;
    public UnityEngine.Resolution PhotoCapCamResolution;

    // Camera setup state
    private bool asyncCameraSetupStarted = false;
    private bool cameraReady = false;
    private bool asyncCameraCaptureStarted = false;
    private bool releaseRequested = false;
    private bool asyncCameraReleaseStarted = false;
    private int consumersSignedDoneWithFrame = 0;

    // TODO: get bug fix for camera release bug
    // Don't try to release the camera more then a few times before giving up
    private int maxReleaseAttempts = 15;
    private int releaseAttempts = 0;

    // Frame processing
    /// <summary>
    /// 
    /// </summary>
    /// <param name="frame"></param>
    /// <returns>
    /// true if finished with the frame
    /// </returns>
    public delegate bool FrameProcessDelegate(PhotoCaptureFrame frame);
    private List<FrameProcessDelegate> frameProcessingCallbacks = new List<FrameProcessDelegate>();

    public override void InitializeInternal()
    {

    }

    public void RegisterForFrames(FrameProcessDelegate frameProcessingCallback)
    {
        if (frameProcessingCallback == null)
        {
            return;
        }

        this.frameProcessingCallbacks.Add(frameProcessingCallback);

        this.EnsureCameraIsSetup();
    }

    public void UnregisterForFrames(FrameProcessDelegate frameProcessingCallback)
    {
        if (this.frameProcessingCallbacks.Contains(frameProcessingCallback))
        {
            this.frameProcessingCallbacks.Remove(frameProcessingCallback);
        }
        
        if (this.frameProcessingCallbacks.Count == 0)
        {
            this.RequestReleaseCamera();
        }
    }

    public void SignalFinishedWithFrame()
    {
        ++consumersSignedDoneWithFrame;
    }

    private bool EnsureCameraIsSetup()
    {
#if UNITY_WSA && !UNITY_EDITOR
        // setup as normal
        this.IsAvailableOnPlatform = true;
#else
        if (!this.UseInEditor) return false;
#endif
        if (asyncCameraSetupStarted)
        {
            return true; // done!
        }

        try
        {
            var reses = PhotoCapture.SupportedResolutions.OrderByDescending(k => k.width); 
            if (reses.Count() > 0)
            {
                PhotoCapCamResolution = reses.First();
                foreach (var res in reses)
                {
                    if (res.width <= MinWidth)
                        break;
                    PhotoCapCamResolution = res;
                }                

                if (PhotoCap == null)
                {
                    PhotoCapture.CreateAsync(false, delegate (PhotoCapture captureObject)
                    {
                        PhotoCap = captureObject;
                        
                        asyncCameraSetupStarted = false;
                        this.EnsureCameraIsSetup();
                    });
                }
                else
                {
                    CameraParameters cameraParameters = new CameraParameters();
                    cameraParameters.hologramOpacity = 0.0f;
                    cameraParameters.cameraResolutionWidth = PhotoCapCamResolution.width;
                    cameraParameters.cameraResolutionHeight = PhotoCapCamResolution.height;
                    cameraParameters.pixelFormat = CapturePixelFormat.BGRA32;

                    PhotoCap.StartPhotoModeAsync(cameraParameters, delegate (PhotoCapture.PhotoCaptureResult result)
                    {
                        if (result.success)
                        {
                            cameraReady = true;
                            Debug.Log("PhotoCapture created...");
                        }

                        asyncCameraSetupStarted = false;
                    });
                }

                asyncCameraSetupStarted = true;
                return true;
            }

            return false;
        }
        catch (System.Exception ex)
        {
            Debug.Log(ex);
            return false;
        }
    }

    void Update()
    {
        if (this.mFrameToProcess != null)
        {
            var frame = this.mFrameToProcess;
            this.mFrameToProcess = null;

            foreach (var frameProcessingCallback in this.frameProcessingCallbacks)
            {
                if (frameProcessingCallback(frame))
                {
                    ++this.consumersSignedDoneWithFrame;
                }
            }

            this.mFrameBeingProcessed = frame;
            return; // give it a frame to rest, and let it upload to the GPU
        }

        if (this.mFrameBeingProcessed != null)
        {
            if (this.consumersSignedDoneWithFrame >= this.frameProcessingCallbacks.Count)
            {
                this.mFrameBeingProcessed.Dispose();
                this.consumersSignedDoneWithFrame = 0;
                this.mFrameBeingProcessed = null;
            }
            else
            {
                return;
            }
        }

        if (this.releaseRequested)
        {
            this.releaseRequested = false;
            this.ReleaseCamera();
            return;
        }

        if (this.frameProcessingCallbacks.Count != 0)
        {
            this.EnsureCameraIsSetup();
        }
        else
        {
            return;
        }

        if (asyncCameraReleaseStarted)
        {
            return;
        }

        if (!cameraReady)
        {
            return;
        }

        if (asyncCameraCaptureStarted)
        {
            return;
        }

        if (PhotoCap != null && this.frameProcessingCallbacks.Count != 0)
        {
            var start = DateTime.Now;
            asyncCameraCaptureStarted = true;
            PhotoCap.TakePhotoAsync((result, frame) =>
            {
                var elapsed = DateTime.Now.Subtract(start).TotalMilliseconds;
                Debug.Log("TakePhotoAsync (" + PhotoCapCamResolution.width + " - " + PhotoCapCamResolution.height + " - " + PhotoCapCamResolution.refreshRate + "): " + elapsed.ToString("0.0") + "ms");

                this.mFrameToProcess = frame;

                this.asyncCameraCaptureStarted = false;
            });
        }
    }

    private void RequestReleaseCamera()
    {
        this.releaseRequested = true;
    }

    private void ReleaseCamera()
    {
        if (PhotoCap == null || asyncCameraReleaseStarted)
        {
            return;
        }

        asyncCameraReleaseStarted = true;

        PhotoCap.StopPhotoModeAsync((PhotoCapture.PhotoCaptureResult result) =>
        {
            asyncCameraReleaseStarted = false;

            if (result.success)
            {
                PhotoCap.Dispose();
                PhotoCap = null;
                cameraReady = false;
                asyncCameraSetupStarted = false;
                this.releaseAttempts = 0;
            }
            else
            {
                if (this.releaseAttempts <= this.maxReleaseAttempts)
                {
                    this.RequestReleaseCamera();
                }

                ++this.releaseAttempts;
            }
        });
    }
}
