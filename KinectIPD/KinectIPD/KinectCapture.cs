// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using Microsoft.Kinect;
using Microsoft.Kinect.Face;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;

namespace KinectIPD
{
    class KinectCapture
    {
        public event EventHandler FrameUpdated;
        public void PauseCapture() { _isPaused = true; }
        public void ResumeCapture() { _isPaused = false; }
        public bool IsPaused() { return _isPaused; }

        public WriteableBitmap ColorBitmap;
        public DateTime LastBodySeenTime = DateTime.MinValue;
        public int EyeBoxVerticalOffset = 0;        // And adjustment value to shift the eyes boxes up/down (to account for transient face detection problems).
        public double HeadDist;                     // How far aways is the currently tracked head
        public Point HeadPoint;                     // The X/Y coordinate for the head in the camera frame
        public Rect LeftEyeRect;                    // The rect for the left eye in the camera frame
        public Rect RightEyeRect;                   // The rect for the right eye in the camera frame

        public bool HasBody { get { return _currentBody != null; } }
        public bool HasFrame { get { return _frameCount > 0; } }

        private float _maxBodyDist = 1500f;     // How far away should we track a body.
        private float _maxAngle = 25;           // Maximum angle a face can be turned to be considered valid.

        private bool _isPaused = false;
        private KinectSensor _sensor = null;
        private HighDefinitionFaceFrameSource _faceSource = null;
        private HighDefinitionFaceFrameReader _faceReader = null;
        private BodyFrameSource _bodySource = null;
        private BodyFrameReader _bodyReader = null;
        private MultiSourceFrameReader _reader;
        private Body _currentBody = null;
        private DateTime _lastFaceFrame = DateTime.Now;

        private FaceAlignment _faceAlignment = null;
        private FaceModel _faceModel = null;
        private IReadOnlyList<CameraSpacePoint> _faceVertices;

        private int _frameCount = 0;

        private CameraSpacePoint[] _cameraSpacePoints = new CameraSpacePoint[1920 * 1080];
        private ushort[] _depthData = null;
        private int _depthWidth;
        
        public KinectCapture()
        {
            InitializeKinect();
        }

        private void InitializeKinect()
        {
            _sensor = KinectSensor.GetDefault();
            
            if (_sensor != null)
            {
                _reader = _sensor.OpenMultiSourceFrameReader(FrameSourceTypes.Color | FrameSourceTypes.Depth);// | FrameSourceTypes.LongExposureInfrared);
                _reader.MultiSourceFrameArrived += Reader_MultiSourceFrameArrived;

                FrameDescription colorFrameDescription = _sensor.ColorFrameSource.CreateFrameDescription(ColorImageFormat.Bgra);
                ColorBitmap = new WriteableBitmap(colorFrameDescription.Width, colorFrameDescription.Height, 96.0, 96.0, PixelFormats.Bgr32, null);

                _bodySource = _sensor.BodyFrameSource;
                _bodyReader = _bodySource.OpenReader();
                _bodyReader.FrameArrived += BodyReader_FrameArrived;

                _faceSource = new HighDefinitionFaceFrameSource(_sensor);
                _faceReader = _faceSource.OpenReader();
                _faceSource.TrackingQuality = FaceAlignmentQuality.Low;
                _faceReader.FrameArrived += FaceReader_FrameArrived;

                _faceModel = new FaceModel();
                _faceAlignment = new FaceAlignment();

                _sensor.Open();
            }
        }

        public bool IsFacingCamera
        {
            get
            {
                var faceOrientation = _faceAlignment.FaceOrientation;
                var faceOrientationQ = new Quaternion(faceOrientation.X, faceOrientation.Y, faceOrientation.Z, faceOrientation.W);

                return faceOrientationQ.Angle < _maxAngle;
            }
        }

        public bool HasFace
        {
            get
            {
                return
                    _faceVertices != null && _faceVertices.Count > 0 &&
                    _faceSource.IsTrackingIdValid &&
                    DateTime.Now.Subtract(_lastFaceFrame).TotalMilliseconds < 2500;
            }
        }

        private Point MapCameraPointToCurrentSpace(CameraSpacePoint cameraSpacePoint)
        {
            Point mappedPoint = new Point();
            ColorSpacePoint point = _sensor.CoordinateMapper.MapCameraPointToColorSpace(cameraSpacePoint);
            mappedPoint.X = point.X;
            mappedPoint.Y = point.Y;

            return mappedPoint;
        }

        private void BodyReader_FrameArrived(object sender, BodyFrameArrivedEventArgs e)
        {
            if (_isPaused) return;
            using (var frame = e.FrameReference.AcquireFrame())
            {
                if (frame != null)
                {
                    Body[] bodies = new Body[frame.BodyCount];
                    frame.GetAndRefreshBodyData(bodies);

                    var trackedBodies = bodies.Where(b => b.IsTracked);

                    _currentBody = null;
                    foreach (var b in trackedBodies)
                    {
                        if (Math.Abs(b.Joints[JointType.Head].Position.X) < .2 &&
                            Math.Abs(b.Joints[JointType.Head].Position.Z * 1000) < _maxBodyDist)
                        {
                            _currentBody = b;
                            LastBodySeenTime = DateTime.Now;
                            break;
                        }
                    }
                    if (_currentBody != null && _faceSource.TrackingId != _currentBody.TrackingId)
                    {
                        _faceSource.TrackingId = _currentBody.TrackingId;
                    }
                    else if (_currentBody == null)
                    {
                        _faceSource.TrackingId = 0;
                    }                    
                }
            }
        }
        
        private void FaceReader_FrameArrived(object sender, HighDefinitionFaceFrameArrivedEventArgs e)
        {
            if (_isPaused) return;
            using (var frame = e.FrameReference.AcquireFrame())
            {
                if (frame != null && frame.IsFaceTracked)
                {
                    _lastFaceFrame = DateTime.Now;
                    frame.GetAndRefreshFaceAlignmentResult(_faceAlignment);
                    _faceVertices = _faceModel.CalculateVerticesForAlignment(_faceAlignment);
                }
            }
        }

        void Reader_MultiSourceFrameArrived(object sender, MultiSourceFrameArrivedEventArgs e)
        {
            _frameCount++;

            // hackhack: allows slow CPUs to process frames - run for a few frames then pause for a few
            _faceReader.IsPaused = _frameCount % 6 < 3 ? false : true;
            
            if (!_isPaused)
            {
                var reference = e.FrameReference.AcquireFrame();

                // Color
                using (var frame = reference.ColorFrameReference.AcquireFrame())
                {
                    if (frame != null)
                    {
                        frame.ToBitmap(ref ColorBitmap);
                    }
                }

                //Depth
                using (var frame = reference.DepthFrameReference.AcquireFrame())
                {
                    if (frame != null)
                    {
                        _depthWidth = frame.FrameDescription.Width;
                        int height = frame.FrameDescription.Height;
                        if (_depthData == null)
                            _depthData = new ushort[_depthWidth * height];
                        frame.CopyFrameDataToArray(_depthData);
                        _sensor.CoordinateMapper.MapColorFrameToCameraSpace(_depthData, _cameraSpacePoints);
                    }
                }
            }

            ComputePositions();

            if (FrameUpdated != null)
                FrameUpdated(this, new EventArgs());
        }

        private void ComputePositions()
        {
            if (_faceVertices == null)
                return;

            var rightEyeOuterPoint = MapCameraPointToCurrentSpace(_faceVertices[(int)HighDetailFacePoints.RighteyeOutercorner]);
            var rightEyeInnerPoint = MapCameraPointToCurrentSpace(_faceVertices[(int)HighDetailFacePoints.RighteyeInnercorner]);
            var rightEyeUpperPoint = MapCameraPointToCurrentSpace(_faceVertices[(int)HighDetailFacePoints.RighteyeMidtop]);
            var rightEyeLowerPoint = MapCameraPointToCurrentSpace(_faceVertices[(int)HighDetailFacePoints.RighteyeMidbottom]);
            if (!double.IsInfinity(rightEyeOuterPoint.X))
            {
                RightEyeRect.X = (int)rightEyeInnerPoint.X - 10;
                RightEyeRect.Y = (int)rightEyeUpperPoint.Y - 10 - EyeBoxVerticalOffset;
                RightEyeRect.Width = (int)Math.Abs(rightEyeOuterPoint.X - rightEyeInnerPoint.X) + 20;
                RightEyeRect.Height = (int)Math.Abs(rightEyeLowerPoint.Y - rightEyeUpperPoint.Y) + 20;
            }

            var leftEyeOuterPoint = MapCameraPointToCurrentSpace(_faceVertices[(int)HighDetailFacePoints.LefteyeOutercorner]);
            var leftEyeInnerPoint = MapCameraPointToCurrentSpace(_faceVertices[(int)HighDetailFacePoints.LefteyeInnercorner]);
            var leftEyeUpperPoint = MapCameraPointToCurrentSpace(_faceVertices[(int)HighDetailFacePoints.LefteyeMidtop]);
            var leftEyeLowerPoint = MapCameraPointToCurrentSpace(_faceVertices[(int)HighDetailFacePoints.LefteyeMidbottom]);
            if (!double.IsInfinity(leftEyeOuterPoint.X))
            {
                LeftEyeRect.X = (int)leftEyeOuterPoint.X - 10;
                LeftEyeRect.Y = (int)leftEyeUpperPoint.Y - 10 - EyeBoxVerticalOffset;
                LeftEyeRect.Width = (int)Math.Abs(leftEyeInnerPoint.X - leftEyeOuterPoint.X) + 20;
                LeftEyeRect.Height = (int)Math.Abs(leftEyeLowerPoint.Y - leftEyeUpperPoint.Y) + 20;
            }

            if (_currentBody != null)
            {
                HeadDist = _currentBody.Joints[JointType.Head].Position.ToVector3D().Length * 1000;
                var headPos = _currentBody.Joints[JointType.Head].Position;
                HeadPoint = MapCameraPointToCurrentSpace(headPos);
            }
        }
        
        public Vector3D GetWeightedAverageCSP(double centerX, double centerY, double dist, Point[] poissonDisk)
        {
            try
            {
                Vector3D result1 = GetAverageCSP(_cameraSpacePoints, (int)(centerX), (int)(centerY), dist, poissonDisk);
                Vector3D result2 = GetAverageCSP(_cameraSpacePoints, (int)(centerX + 1), (int)(centerY + 1), dist, poissonDisk);
                Vector3D diff = result2 - result1;

                var remainderX = centerX - (int)centerX;
                var remainderY = centerY - (int)centerY;
                var remainderZ = (remainderX + remainderY) / 2;
                Vector3D finalResult = result1;
                finalResult.X += diff.X * remainderX;
                finalResult.Y += diff.X * remainderY;
                finalResult.Z += diff.Z * remainderZ;
                return finalResult;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return new Vector3D(0, 0, 0);
            }
        }

        public Vector3D GetAverageCSP(CameraSpacePoint[] cameraSpacePoints, double centerX, double centerY, double dist, Point[] poissonDisk)
        {
            if (centerX <= 0 || centerY <= 0)
                return new Vector3D();
            int colorX = (int)(centerX);
            int colorY = (int)(centerY);
            int count = 1;
            long colorIndex = (long)(colorY * 1920 + colorX);
            if (colorIndex < 0  || colorIndex >= cameraSpacePoints.Length)
                return new Vector3D();

            var result = cameraSpacePoints[colorIndex].ToVector3D();
            for (int i = 0; i < poissonDisk.Length; i++)
            {
                colorX = (int)(centerX + poissonDisk[i].X * dist);
                colorY = (int)(centerY + poissonDisk[i].Y * dist);
                colorIndex = (long)(colorY * 1920 + colorX);
                if (colorIndex > -1 && colorIndex < cameraSpacePoints.Length)
                {
                    var point = cameraSpacePoints[colorIndex].ToVector3D();
                    if (!double.IsInfinity(point.X) && !double.IsInfinity(point.Y) && !double.IsInfinity(point.Z))
                    {
                        result += point;
                        count++;
                    }
                }
            }
            return (result / count);
        }
    }
}
