// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Microsoft.Kinect;
using System;
using System.IO;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;

namespace KinectIPD
{
    public static class Extensions
    {

        public static double CalculateAverageLuma(this WriteableBitmap bitmapSource, System.Windows.Int32Rect srcRect)
        {
            if (srcRect.X < 0 || srcRect.Y < 0 || srcRect.X + srcRect.Width >= bitmapSource.Width || srcRect.Y + srcRect.Height >= bitmapSource.Height)
                return 0;

            byte[] pixelArray = bitmapSource.ToArray(srcRect);
            return CalculateAverageLuma(pixelArray, bitmapSource.Format.BitsPerPixel / 8);
        }

        public static double CalculateAverageLuma(byte[] pixelArray, int bytesPerPixel = 3)
        {
            try
            {
                double lum = 0;

                for (int i = 0; i < pixelArray.Length; i += bytesPerPixel)
                {
                    lum += (0.299 * pixelArray[i + 2] + 0.587 * pixelArray[i + 1] + 0.114 * pixelArray[i]);
                }

                var avgLum = lum / (pixelArray.Length / bytesPerPixel);
                return avgLum / 255.0;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("********: " + ex.ToString());
                return 0;
            }
        }

        public static System.Drawing.Color GetPixel(this WriteableBitmap bitmapSource, int x, int y)
        {
            using (var bitmap = bitmapSource.ToBitmap())
            {
                return bitmap.GetPixel(x, y);
            }
        }

        public static byte[] ToArray(this WriteableBitmap bitmapSource, System.Windows.Int32Rect srcRect)
        {
            if (srcRect.IsEmpty)
                return new byte[1];
            try
            {
                // Stride = (width) x (bytes per pixel)
                int stride = (int)srcRect.Width * (bitmapSource.Format.BitsPerPixel / 8);
                byte[] pixels = new byte[(int)srcRect.Height * stride];

                bitmapSource.CopyPixels(srcRect, pixels, stride, 0);

                return pixels;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("********: " + ex.ToString());
                return new byte[1];
            }
        }

        public static Vector3D ToVector3D(this CameraSpacePoint point)
        {
            return new Vector3D(point.X, point.Y, point.Z);
        }
        
        public static void ToBitmap(this ColorFrame frame, ref WriteableBitmap colorBitmap)
        {

            FrameDescription colorFrameDescription = frame.FrameDescription;

            using (KinectBuffer colorBuffer = frame.LockRawImageBuffer())
            {
                colorBitmap.Lock();

                // verify data and write the new color frame data to the display bitmap
                if ((colorFrameDescription.Width == colorBitmap.PixelWidth) && (colorFrameDescription.Height == colorBitmap.PixelHeight))
                {
                    frame.CopyConvertedFrameDataToIntPtr(
                        colorBitmap.BackBuffer,
                        (uint)(colorFrameDescription.Width * colorFrameDescription.Height * 4),
                        ColorImageFormat.Bgra);

                    colorBitmap.AddDirtyRect(new System.Windows.Int32Rect(0, 0, colorBitmap.PixelWidth, colorBitmap.PixelHeight));
                }

                colorBitmap.Unlock();
            }
        }
        
        public static System.Drawing.Bitmap ToBitmap(this WriteableBitmap writeBmp)
        {
            System.Drawing.Bitmap bmp;


            using (MemoryStream outStream = new MemoryStream())
            {
                BitmapEncoder enc = new BmpBitmapEncoder();
                enc.Frames.Add(BitmapFrame.Create((BitmapSource)writeBmp));
                enc.Save(outStream);
                bmp = new System.Drawing.Bitmap(outStream);
            }
            return bmp;
        }       
    }
}
