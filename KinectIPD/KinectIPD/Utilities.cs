// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Microsoft.Kinect;
using System;
using System.Windows.Media.Media3D;

namespace KinectIPD
{
    class Utilities
    {
        public static double Distance(System.Windows.Point p1, System.Windows.Point p2)
        {
            double distance = Math.Sqrt(Math.Pow(p2.X - p1.X, 2) + Math.Pow(p2.Y - p1.Y, 2));
            return distance;
        }

        public static double VectorLength(CameraSpacePoint point1, CameraSpacePoint point2)
        {
            double xd = point2.X - point1.X;
            double yd = point2.Y - point1.Y;
            double zd = point2.Z - point1.Z;

            var result = Math.Pow(xd, 2) + Math.Pow(yd, 2) + Math.Pow(zd, 2);

            result = Math.Sqrt(result);

            return result;
        }

        public static double VectorLength(Vector3D point1, Vector3D point2)
        {
            double xd = point2.X - point1.X;
            double yd = point2.Y - point1.Y;
            double zd = point2.Z - point1.Z;

            var result = Math.Pow(xd, 2) + Math.Pow(yd, 2) + Math.Pow(zd, 2);

            result = Math.Sqrt(result);

            return result;
        }


        public static double Lerp(double x, double y, double s)
        {
            if (double.IsNaN(x))
                return y;
            if (double.IsNaN(y))
                return x;
            return x + s * (y - x);
        }
    }
}
