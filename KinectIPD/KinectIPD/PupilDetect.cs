// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;

namespace KinectIPD
{
    #region Interop

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct MPupilInfo
    {
        public float CenterX { get; private set; }
        public float CenterY { get; private set; }
    }

    public struct MPupilDetectSettings
    {
        public int ScaleInput { get; set; }
        public int FastEyeWidth { get; set; }
        public int WeightBlurSize { get; set; }
        public bool EnableWeight { get; set; }
        public float WeightDivisor { get; set; }
        public double GradientThreshold { get; set; }
        public bool EnablePostProcess { get; set; }
        public float PostProcessThreshold { get; set; }
        public float MaxMag { get; set; }
    };
    #endregion
    public class PupilDetect
    {
        #region DLLImports

        [DllImport(@"PupilDetectDLL.dll")]
        private static extern IntPtr CreatePupilDetect();

        [DllImport(@"PupilDetectDLL.dll")]
        private static extern void DestroyPupilDetect(IntPtr instance);

        [DllImport(@"PupilDetectDLL.dll")]
        private static extern void SetSettings(IntPtr instance, IntPtr settings);
        [DllImport(@"PupilDetectDLL.dll")]
        private static extern IntPtr GetSettings(IntPtr instance);

        [DllImport(@"PupilDetectDLL.dll")]
        private static extern int FindCenter(IntPtr instance, IntPtr pixels, int width, int height, out IntPtr PupilInfo, bool writeFiles, string fileNamePrefix);

        #endregion


        private float _ignoreBorderWidth = 5.0f;
        private MPupilDetectSettings _settings;
        private IntPtr _pupilDetectDLL = IntPtr.Zero;

        public PupilDetect()
        {
            _pupilDetectDLL = CreatePupilDetect();
            var result = GetSettings(_pupilDetectDLL);
            _settings = (MPupilDetectSettings)Marshal.PtrToStructure(result, typeof(MPupilDetectSettings));
            _settings.EnableWeight = true;
            _settings.EnablePostProcess = true;
            _settings.FastEyeWidth = 75;
            _settings.WeightBlurSize = 7; // odd numbers
            _settings.WeightDivisor = 1;
            _settings.GradientThreshold = 50;
            _settings.PostProcessThreshold = .95f;
            _settings.MaxMag = 50;
            UpdatePupilDetectSettings();
        }

        public struct PupilDetectResult
        {
            public Rect rectangle;
            public Point center;
        }

        public Task<PupilDetectResult> FindPupil(ref WriteableBitmap colorBitmap, Rect eyeRect, bool writeFiles = false, string fileNamePrefix = "")
        {
            if (eyeRect.Width == 0 || eyeRect.Height == 0)
            {
                return null;
            }
            byte[] array = colorBitmap.ToArray(new Int32Rect((int)eyeRect.X, (int)eyeRect.Y, (int)eyeRect.Width, (int)eyeRect.Height));
            return FindPupil(ref array, (int)eyeRect.Width, (int)eyeRect.Height);
        }

        public Task<PupilDetectResult> FindPupil(ref byte[] array, int width, int height, bool writeFiles = false, string fileNamePrefix = "")
        {
            if (width == 0 || height == 0)
            {
                return null;
            }

            int size = Marshal.SizeOf(array[0]) * array.Length;
            IntPtr pnt = Marshal.AllocHGlobal(size);

            // Copy the array to unmanaged memory.
            Marshal.Copy(array, 0, pnt, array.Length);

            Task<PupilDetectResult> t = Task.Run(() =>
            {
                var pPupilInfo = IntPtr.Zero;
                var result = FindCenter(_pupilDetectDLL, pnt, width, height, out pPupilInfo, writeFiles, fileNamePrefix);
                Marshal.FreeHGlobal(pnt);

                var pupilResult = new PupilDetectResult();
                if (result == 1)
                {
                    var PupilInfo = (MPupilInfo)Marshal.PtrToStructure(pPupilInfo, typeof(MPupilInfo));
                    // Reject the result if the pupil was found along the border - usually not valid.
                    if (PupilInfo.CenterX > _ignoreBorderWidth && PupilInfo.CenterX < width - _ignoreBorderWidth &&
                    PupilInfo.CenterY > _ignoreBorderWidth && PupilInfo.CenterY < height - _ignoreBorderWidth)
                    {
                        pupilResult.center = new Point(PupilInfo.CenterX, PupilInfo.CenterY);
                        pupilResult.rectangle = new Rect(PupilInfo.CenterX - 2, PupilInfo.CenterY - 2, 4, 4);
                    }
                }
                return pupilResult;
            });

            return t;
        }

        private void UpdatePupilDetectSettings()
        {
            var pPupilSettings = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(MPupilDetectSettings)));
            Marshal.StructureToPtr(_settings, pPupilSettings, true);
            SetSettings(_pupilDetectDLL, pPupilSettings);
        }
    }
}
