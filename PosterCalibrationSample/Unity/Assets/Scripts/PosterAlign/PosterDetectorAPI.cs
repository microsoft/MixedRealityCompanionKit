// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using UnityEngine;
namespace PosterAlignment
{
    /// <summary>
    /// PVCamPosterDetector - takes PV cam captures, and finds poster matches.
    /// 
    /// Uses "PosterDetectorAPI.dll"
    /// </summary>
    public class PosterDetectorAPI : System.IDisposable
    {
        #region Interop

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public struct InteropVector2
        {
            public float X;
            public float Y;
        }
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public struct PosterMatchData
        {
            public bool PosterFound;
            public int Inliers;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
            public InteropVector2[] Corners;
        }

        [System.Runtime.InteropServices.DllImport("Plugin.dll", EntryPoint = "PosterDetector_GetPosterMatchData", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int PosterDetector_GetPosterMatchData(IntPtr pixels, uint width, uint height, bool flipImage, int minimumInliers,
           [MarshalAs(UnmanagedType.Struct)] ref PosterMatchData posterMatchData);

        [System.Runtime.InteropServices.DllImport("Plugin.dll", EntryPoint = "PosterDetector_SetPosterObjectData", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern bool PosterDetector_SetPosterObjectData(IntPtr pixels, uint width, uint height);


        [DllImport("Plugin")]
        private static extern void SetAssetsPath([MarshalAs(UnmanagedType.LPStr)] string path);

        #endregion

        private const int MinimumInliers = 30;

        private PosterMatchData posterMatchData = new PosterMatchData();
        private bool bDLLNotWorking = false;

        public PosterDetectorAPI()
        {
        }

        public void Dispose()
        {
        }

        ~PosterDetectorAPI()
        {
            this.Dispose();
        }

        public bool SetPoster(IntPtr pPixels, int width, int height)
        {
            if (this.bDLLNotWorking)
            {
                return false;
            }
            try
            {
                Debug.Log("SetPoster " + width + " - " + height);
                return PosterDetector_SetPosterObjectData(pPixels, (uint)width, (uint)height);
            }
            catch (System.DllNotFoundException ex)
            {
                Debug.LogError("PosterDetectorAPI: Error loading DLL. " + ex.ToString());
                this.bDLLNotWorking = true;
                return false;
            }
        }

        public bool TryDetectPoster(IntPtr pPixels, int width, int height, out Vector2[] positionList, bool normalize = false)
        {
            positionList = null;
            if (this.bDLLNotWorking)
            {
                return false;
            }
            try
            {
                var start = DateTime.Now;
                //run poster detection by invoking native code:
                PosterDetector_GetPosterMatchData(pPixels, (uint)width, (uint)height, false, MinimumInliers, ref posterMatchData);
                var elapsed = DateTime.Now.Subtract(start).TotalMilliseconds;
                Debug.Log("TryDetectPoster (" + width + " - " + height + "): " + posterMatchData.PosterFound + " - " + elapsed.ToString("0.0") + "ms");
                if (!posterMatchData.PosterFound)
                {
                    return false;
                }
            }
            catch (System.DllNotFoundException ex)
            {
                Debug.LogError("PosterDetectorAPI: Error loading DLL. " + ex.ToString());
                this.bDLLNotWorking = true;
                return false;
            }
            catch (Exception ex)
            {
                Debug.LogError("***TryDetectPoster exception:" + ex.ToString());
                return false;
            }

            positionList = new Vector2[4];
            var debugString = "posterMatchData\n";
            // we'll always get the four corners of the poster as markers
            for (int i = 0; i < 4; i++)
            {
                Vector2 newPoint = new Vector2();
                newPoint.x = posterMatchData.Corners[i].X;
                newPoint.y = posterMatchData.Corners[i].Y;

                if (normalize)
                {
                    // normalize from [0,size-1] to [-1,1] :
                    newPoint.x = (((((newPoint.x) / (width - 1))) * 2.0f) - 1.0f);
                    newPoint.y = (((((newPoint.y) / (height - 1))) * 2.0f) - 1.0f);
                }

                debugString += " - " + i + ": " + newPoint + "\n";
                positionList[i] = newPoint;
            }

            Debug.Log(debugString);
            if ((!VerifyPoints(positionList)))
            {
                Debug.Log("** failed points");
                positionList = null;
            }

            return true;
        }

        // HACK:  must be a better algorithm for this
        // just do some checks to make sure we have valid rectangular (skewed) clockwise points.
        bool VerifyPoints(Vector2[] points, bool normalized = false)
        {
            // TODO: handle normalized case
            if (normalized)
                return true;

            if (points[0].x > points[1].x ||
                points[0].y > points[3].y ||
                points[1].x - points[0].x < 100 ||
                points[3].y - points[0].y < 100)
                return false;

            return true;
        }
    }
}