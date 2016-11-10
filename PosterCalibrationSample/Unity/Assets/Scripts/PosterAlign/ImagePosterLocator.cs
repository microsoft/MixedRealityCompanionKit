// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.VR.WSA.WebCam;

/// <summary>
/// Finds Posters in the world using the PV Camera and reports them.
/// 
/// Troubleshooting:
/// 
/// 1:Ensure that the Unity project has 'Webcam' (and possibly 'Microphone') permissions,
///     in Unity-Menu / Edit / Project Settings / Player / Publishing Settings / Capabilities / WebCam
///     
/// </summary>
public class ImagePosterLocator : MonoBehaviour
{
    [Tooltip("An optional object on which to render the camera image (useful for debugging or just showing the user feedback).")]
    public MeshRenderer OptionalShowCamTextureOn = null;
    [Tooltip("An optional object on which to render the current poster being processed (useful for debugging or just showing the user feedback).")]
    public MeshRenderer OptionalShowPosterTextureOn = null;

    [HideInInspector]
    public int UpdateVersion = 0;

    private volatile bool isProcessingImage;

    private PosterLocationHandler posterLocationHandler;
    private static PosterDetectorAPI posterSystemAPI;

    private Texture2D previewTexture = null;
    private Texture2D posterTexture;
    private Color32[] posterTextureData;
    private bool posterTextureDataDirty = false;
    private int posterTextureDataWidth = 0;
    private int posterTextureDataHeight = 0;

    private static byte[] cameraImageBuffer = null; // color webcam data buffer
    private static List<byte> cameraImageListBuffer = null; // color webcam data buffer as list
    private static GCHandle cameraImageGCHandle;
    private static IntPtr cameraImagePtrToData;

    public void StartProcessing()
    {
        Debug.Log("ImagePosterLocator:StartProcessing: " + gameObject.name);
        UpdatePosterDebug();
        posterLocationHandler = null;
        PVCamManager.Instance.RegisterForFrames(this.OnCaptureCompleted);
    }

    void OnDisable()
    {
        StopProcessing();
    }

    public void StopProcessing()
    {
        Debug.Log("ImagePosterLocator:StopProcessing: " + gameObject.name);
        if (PVCamManager.Instance != null)
        {
            PVCamManager.Instance.UnregisterForFrames(this.OnCaptureCompleted);
        }
        this.isProcessingImage = false;
        posterTextureDataDirty = true;
    }

    public void SetPosterTexture(Texture2D posterTexture)
    {
        this.posterTexture = posterTexture;
        posterTextureDataDirty = true;
        int maxWidth = 512;
        int whichMip = 0;
        Debug.Log("ImagePosterLocator:SetPosterTexture Mips:" + this.posterTexture.mipmapCount);

        for (int i = 0; i < this.posterTexture.mipmapCount; i++)
        {
            whichMip = i;
            var mipWidth = Math.Max(1, this.posterTexture.width >> i);

            if (mipWidth <= maxWidth)
                break;
        }

        posterTextureData = this.posterTexture.GetPixels32(whichMip);
        posterTextureDataWidth = this.posterTexture.width >> whichMip;
        posterTextureDataHeight = this.posterTexture.height >> whichMip;

        Debug.Log("ImagePosterLocator:SetPosterTexture - using mip #" + whichMip + ":" + posterTextureDataWidth + "x" + posterTextureDataHeight);
        UpdatePosterDebug();
    }

    void UpdatePosterDebug()
    {
        if (OptionalShowPosterTextureOn != null && posterTexture != null)
        {
            OptionalShowPosterTextureOn.material.mainTexture = posterTexture;

            // Show the texture at the final aspect (which may be different depending on non-power-of-two textures).
            // Note that non-power-of-two textures may not work correctly due to the different aspect ratio to the original.
            var textureAspect = (float)posterTextureDataWidth / (float)posterTextureDataHeight;
            var currentHeight = OptionalShowPosterTextureOn.gameObject.transform.localScale.y;
            OptionalShowPosterTextureOn.gameObject.transform.localScale = new Vector2(currentHeight * textureAspect, currentHeight);
            OptionalShowPosterTextureOn.gameObject.SetActive(true);
        }
        else
        {
            OptionalShowPosterTextureOn.gameObject.SetActive(false);
        }
    }

    private void DoOnSeperateThread(Action act)
    {
#if UNITY_WSA && !UNITY_EDITOR
        System.Threading.Tasks.Task.Run(act);
#else
        var thrd = new System.Threading.Thread(() => act());
        thrd.Start();
#endif
    }

    private void UpdatePreviewTexture(byte[] buffer)
    {
        if ((OptionalShowCamTextureOn != null && OptionalShowCamTextureOn.gameObject.activeSelf))
        {
            if (previewTexture == null)
            {
                var res = PVCamManager.Instance.PhotoCapCamResolution;
                previewTexture = new Texture2D(res.width, res.height, TextureFormat.BGRA32, false);
            }

            if (this.OptionalShowCamTextureOn != null && OptionalShowCamTextureOn.material.mainTexture != previewTexture)
            {
                OptionalShowCamTextureOn.material.mainTexture = previewTexture;
            }

            if (cameraImageBuffer != null && cameraImageBuffer.Length > 0)
            {
                previewTexture.LoadRawTextureData(cameraImagePtrToData, cameraImageBuffer.Length);
                previewTexture.Apply();
            }
        }
    }

    private static Matrix4x4 TuneProjectionMatrix(PhotoCaptureFrame frame)
    {
        Matrix4x4 camProj;
        if (!frame.TryGetProjectionMatrix(0.0f, 1.0f, out camProj))
        {
            Debug.LogError("Unable to get the camera projection for this frame!");
        }
        return camProj;
    }

    private bool OnCaptureCompleted(PhotoCaptureFrame frame)
    {
        if (this.isProcessingImage)
        {
            return true;
        }

        this.isProcessingImage = true;

        Matrix4x4 cameraToWorldMatrix;
        frame.TryGetCameraToWorldMatrix(out cameraToWorldMatrix);
        cameraToWorldMatrix = cameraToWorldMatrix * Matrix4x4.Scale(new Vector3(1, 1, -1));

        // Update the preview image with the last image processed
        UpdatePreviewTexture(cameraImageBuffer);
        var res = PVCamManager.Instance.PhotoCapCamResolution;
        var w = res.width;
        var h = res.height;

        if (posterLocationHandler == null)
        {
            posterLocationHandler = new PosterLocationHandler(w, h);
        }

        Matrix4x4 cameraClipToWorld;
        Vector3 cameraPos;
        var camProj = TuneProjectionMatrix(frame);
        GetCameraClipTransformAndWorldPosition(cameraToWorldMatrix, camProj,
            out cameraClipToWorld, out cameraPos);

        posterLocationHandler.UpdateCamera(cameraToWorldMatrix, camProj, cameraPos);

        DoOnSeperateThread(() =>
        {
            // Allocate the buffers to hold the image data
            if ((cameraImageListBuffer == null) || (cameraImageListBuffer.Count != frame.dataLength))
            {
                cameraImageListBuffer = new List<byte>(frame.dataLength);
            }
            if ((cameraImageBuffer == null) || (cameraImageBuffer.Length != frame.dataLength))
            {
                cameraImageBuffer = new byte[frame.dataLength];
                cameraImageGCHandle = GCHandle.Alloc(cameraImageBuffer, GCHandleType.Pinned);
                cameraImagePtrToData = cameraImageGCHandle.AddrOfPinnedObject();
            }

            if (false)
            {
                // TODO: Throws exception. Needs more investigation.
                IntPtr framePtr = frame.GetUnsafePointerToBuffer();
                Marshal.Copy(framePtr, cameraImageBuffer, 0, frame.dataLength);
            }
            else
            {
                frame.CopyRawImageDataIntoBuffer(cameraImageListBuffer);
                // Copy the image data from the list into the byte array
                cameraImageListBuffer.CopyTo(cameraImageBuffer);
            }

            PVCamManager.Instance.SignalFinishedWithFrame();

            // Find the poster:
            this.FindPoster(w, h);

            this.isProcessingImage = false;
        });

        return false;
    }

    private void FindPoster(int pixelWidth, int pixelHeight)
    {
        bool foundPoster = false;
        try
        {
            if (posterSystemAPI == null)
            {
                posterSystemAPI = new PosterDetectorAPI();
            }

            // update the poster data to the plugin
            if (posterTextureDataDirty && posterTextureData != null && posterTextureData.Length > 0)
            {
                GCHandle handle = default(GCHandle);
                try
                {
                    handle = GCHandle.Alloc(posterTextureData, GCHandleType.Pinned);
                    IntPtr ptr = handle.AddrOfPinnedObject();
                    posterTextureDataDirty = !posterSystemAPI.SetPoster(ptr, posterTextureDataWidth, posterTextureDataHeight);
                }
                finally
                {
                    if (handle != default(GCHandle))
                    {
                        handle.Free();
                    }
                }
            }

            Vector2[] pixelPositions;
            var result = posterSystemAPI.TryDetectPoster(cameraImagePtrToData, pixelWidth, pixelHeight, out pixelPositions, false);

            if (result && pixelPositions != null)
            {
                foundPoster = true;
                for (int i = 0; i < 4; i++)
                {
                    var pixelLocation = new PosterCornerLocation();
                    pixelLocation.Id = (PosterCornerLocation.CornerLocationId)i;
                    pixelLocation.PixelX = pixelPositions[i].x;
                    pixelLocation.PixelY = pixelPositions[i].y;

                    posterLocationHandler.OnCornerLocated(pixelLocation);
                }
            }

        }
        catch (Exception ex)
        {
            Debug.Log(ex.ToString());
        }

        if (foundPoster)
        {
            this.UpdateVersion++;
        }
    }

    /// <summary>
    /// UnProject pixel space vector into directional vector
    /// </summary>
    public static Vector3 UnProjectVector(Matrix4x4 proj, Vector3 to)
    {
        Vector3 from = new Vector3(0, 0, 0);

        var axsX = proj.GetRow(0);
        var axsY = proj.GetRow(1);
        var axsZ = proj.GetRow(2);

        from.z = to.z / axsZ.z;
        from.y = (to.y - (from.z * axsY.z)) / axsY.y;
        from.x = (to.x - (from.z * axsX.z)) / axsX.x;

        return from;
    }

    public static void GetCameraClipTransformAndWorldPosition(Matrix4x4 cameraToWorld, Matrix4x4 cameraProjectionMatrix, out Matrix4x4 cameraClipToWorld, out Vector3 cameraPos)
    {
        cameraPos = cameraToWorld.MultiplyPoint(Vector3.zero);

        Matrix4x4 camProj = cameraProjectionMatrix;

        cameraClipToWorld = cameraToWorld              // PVCamera to 'face' tranform
                            * camProj.inverse;
    }

    public static float GZFlipScale = -1.0f;

    public static Ray RayFromCameraSetup(Vector2 uv, Matrix4x4 camera2World, Matrix4x4 cameraProj, Vector3 cameraPos)
    {
        // Turn the uvPoint on the PVCamera into a position on the near clip plane on our proper camera.
        var uvPointInNegOneToOneSpace = uv * 2 - Vector2.one;

        var examplePoint = new Vector3(uvPointInNegOneToOneSpace.x, -uvPointInNegOneToOneSpace.y, GZFlipScale);

        // Flip X and Y before doing unproject:
        examplePoint = new Vector3(-examplePoint.x, -examplePoint.y, examplePoint.z);

        var posAroundCamera = UnProjectVector(cameraProj, examplePoint);

        // Flip X and Y again after doing unproject:
        posAroundCamera = new Vector3(-posAroundCamera.x, -posAroundCamera.y, posAroundCamera.z);

        var posInWorld = camera2World.MultiplyPoint(posAroundCamera);

        var dirInWorld = (posInWorld - cameraPos).normalized;

        return new Ray(cameraPos, dirInWorld);
    }

    public WorldPosFromRays[] DetectedPositions
    {
        get
        {
            if (posterLocationHandler == null)
                return null;
            return posterLocationHandler.DetectedPositions;
        }
    }

    public class PosterCornerLocation
    {
        public enum CornerLocationId
        {
            UpperLeft = 0,
            UpperRight = 1,
            LowerRight = 2,
            LowerLeft = 3
        }
        public CornerLocationId Id;
        public float PixelX;
        public float PixelY;
    }

    private class PosterLocationHandler
    {
        private readonly WorldPosFromRays[] locationCorners;
        private Matrix4x4 cameraToWorld;
        private Matrix4x4 cameraProj;
        private Vector3 cameraPos;
        private readonly int imagePixelWidth;
        private readonly int imagePixelHeight;

        public PosterLocationHandler(
            int imagePixelWidth, int imagePixelHeight)
        {
            this.locationCorners = new WorldPosFromRays[4];
            this.imagePixelWidth = imagePixelWidth;
            this.imagePixelHeight = imagePixelHeight;
        }

        public void UpdateCamera(
            Matrix4x4 cam2World, Matrix4x4 camProj, Vector3 cameraPos)
        {
            this.cameraToWorld = cam2World;
            this.cameraProj = camProj;
            this.cameraPos = cameraPos;
        }

        public WorldPosFromRays[] DetectedPositions
        {
            get
            {
                return locationCorners;
            }
        }

        public void OnCornerLocated(PosterCornerLocation location)
        {
            Vector2 locationUV = GetLocationUV(location);
            Ray rayThroughUV = GetLocationRay(locationUV);

            if ((rayThroughUV.origin == Vector3.zero)
                || (rayThroughUV.direction == Vector3.zero))
            {
                // wierd position, ignore it
                return;
            }

            Debug.Log("Corner Located! Id=" + location.Id + "\nX=" + location.PixelX + " Y=" + location.PixelY);

            if (locationCorners[(int)location.Id] == null)
            {
                locationCorners[(int)location.Id] = new WorldPosFromRays((int)location.Id);
            }

            locationCorners[(int)location.Id].AddRay(rayThroughUV, this.cameraPos, this.cameraProj);
        }

        private Vector2 GetLocationUV(PosterCornerLocation location)
        {
            return new Vector2(location.PixelX / (this.imagePixelWidth - 1), location.PixelY / (this.imagePixelHeight - 1));
        }

        private Ray GetLocationRay(Vector2 uv)
        {
            return RayFromCameraSetup(uv, this.cameraToWorld, this.cameraProj, this.cameraPos);
        }
    }
}