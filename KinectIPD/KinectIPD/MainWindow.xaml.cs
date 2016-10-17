// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.ComponentModel;
using System.Linq;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using System.Windows.Shapes;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Threading.Tasks;
using System.IO;

namespace KinectIPD
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private PupilDetect _pupilDetect;
        private KinectExposure _exposure;
        private KinectCapture _kinect;
        private KinectRecorder _recorder;

        private HoloDeviceManager _holoDeviceManager;

        private MediaPlayer _doneSound = new MediaPlayer();

        private volatile bool _hasSavedIPDToDevice = false;
        private double _finalIPD = 0.0f;

        private float _minDistance = 600;   // minimum distance the user has to be from the kinect
        private float _maxDistance = 1200;   // maximum distance the user has to be from the kinect
        private float _currentZoom = 3;     // used to zoom in/out on user's face when tracking (in debug mode)

        private int _resetStateAfterMs = 7500;              // how long to wait (after '_lastDoneTime') to reset (for the next user) after finding an IPD
        private int _minIpds = 20;                          // minimum number of IPD scans to average together before displaying final IPD
        private int _ipdAverageCount = 30;                  // number of IPD's to use for running average
        private int _pupilUpdateFrequencyMs = 200;          // how often the kinect should scan the face to find the users pupils
        private double _acceptableStdDevStart = 1;          // standard deviation of IPD scans must be under this value before displaying measurement to user
        private double _currentAcceptableStdDev = 1;        // current standard deviation
        private double _acceptableStdDevIncreasePerSec = .1;// how much to increase the acceptable stdDev per second after the minimum number of scans
        private double _maxStdDev = 10;                     // maximum standard deviation that is expected
        private int _maxMeasureTime = 25000;                // how long should sampling continue before giving up

        private DateTime _currentMeasureTimeStart = DateTime.Now;   // when did we start measuring for this session
        private DateTime _lastPupilUpdate = DateTime.Now;           // last time the the pupil detection code ran
        private DateTime _lastDoneTime = DateTime.Now;              // last time we completed a detection (use to leave message on-screen for a period of time)

        private bool _isDebugMode = false;
        private bool _hasValidIPD = false;
        private bool _showOverlay = true;
        private bool _isPaused = false;
        private bool _foundPupils = false;

        private State _currentState = State.Attract;

        private Mode _currentMode = Mode.DisplayOnly;

        private Point _leftPupilCenterImageReco;
        private Point _rightPupilCenterImageReco;
        private Rect _leftPupilRectImageReco;
        private Rect _rightPupilRectImageReco;

        private List<double> _ipds = new List<double>();
        // Used for averaging depth to avoid problems at pixel borders
        private Point[] _poissonDisk = new Point[64];

        #region OnscreenShapes

        private Ellipse _headVisual = new Ellipse()
        {
            Width = 5.0,
            Height = 5.0,
            Fill = new SolidColorBrush(System.Windows.Media.Color.FromArgb(50, 255, 255, 255))
        };
        private Ellipse _leftPupilImageRecoVisual = new Ellipse()
        {
            Width = 5.0,
            Height = 5.0,
            Fill = new SolidColorBrush(System.Windows.Media.Color.FromArgb(70, 0, 255, 255))
        };
        private Ellipse _rightPupilImageRecoVisual = new Ellipse()
        {
            Width = 5.0,
            Height = 5.0,
            Fill = new SolidColorBrush(System.Windows.Media.Color.FromArgb(70, 0, 255, 255))
        };

        private System.Windows.Shapes.Rectangle _leftEyeRectVisual = new System.Windows.Shapes.Rectangle()
        {
            Width = 12.0,
            Height = 12.0,
            Stroke = new SolidColorBrush(System.Windows.Media.Color.FromArgb(50, 255, 0, 0)),
            Fill = new SolidColorBrush(System.Windows.Media.Color.FromArgb(30, 0, 0, 0))
        };
        private System.Windows.Shapes.Rectangle _rightEyeRectVisual = new System.Windows.Shapes.Rectangle()
        {
            Width = 12.0,
            Height = 12.0,
            Stroke = new SolidColorBrush(System.Windows.Media.Color.FromArgb(50, 255, 0, 0)),
            Fill = new SolidColorBrush(System.Windows.Media.Color.FromArgb(30, 0, 0, 0))
        };
        #endregion

        #region BoundProperties
        public event PropertyChangedEventHandler PropertyChanged;

        private string ipdText = "Ready.";
        public string IPDText
        {
            get
            {
                return this.ipdText;
            }

            set
            {
                if (this.ipdText != value)
                {
                    this.ipdText = value;
                    if (this.PropertyChanged != null)
                    {
                        this.PropertyChanged(this, new PropertyChangedEventArgs("IPDText"));
                    }
                }
            }
        }

        private string distText = "0";
        public string DistanceText
        {
            get
            {
                return this.distText;
            }

            set
            {
                if (this.distText != value)
                {
                    this.distText = value;
                    if (this.PropertyChanged != null)
                    {
                        this.PropertyChanged(this, new PropertyChangedEventArgs("DistanceText"));
                    }
                }
            }
        }

        private string instructionOverlayText = "0";
        public string InstructionOverlayText
        {
            get
            {
                return this.instructionOverlayText;
            }

            set
            {
                if (this.instructionOverlayText != value)
                {
                    this.instructionOverlayText = value;
                    if (this.PropertyChanged != null)
                    {
                        this.PropertyChanged(this, new PropertyChangedEventArgs("InstructionOverlayText"));
                    }
                }
            }
        }

        public ImageSource ImageSource
        {
            get
            {
                return _kinect.ColorBitmap;
            }
        }
        #endregion

        public MainWindow()
        {
            Logger.Log("Start");

            InitializeKinect();

            InitializeComponent();
            DataContext = this;

            _pupilDetect = new PupilDetect();
            _exposure = new KinectExposure();
            _recorder = new KinectRecorder();
        }

        private async void ChangeIPDMode(Mode newMode)
        {
            switch (_currentMode)
            {
                case Mode.DeviceList:
                case Mode.NFCReader:
                    _holoDeviceManager.CurrentDeviceChanged -= _holoDeviceManager_CurrentDeviceChanged;
                    _holoDeviceManager.ShutDown();
                    break;
            }

            _currentMode = newMode;

            switch (_currentMode)
            {
                case Mode.DeviceList:
                    _holoDeviceManager = new HoloDeviceManager();
                    DeviceList.ItemsSource = _holoDeviceManager.Devices;
                    SelectDeviceList.ItemsSource = _holoDeviceManager.Devices;

                    // Enable this to automatically add a LocalHost hololens with a known username and password
                    if (false)
                    {
                        var newDevice = new HoloDevice("LocalHost", "http://127.0.0.1:10080", "user", "password");
                        await newDevice.Initialize(false);
                        if (newDevice.Portal != null && newDevice.Portal.ConnectionHttpStatusCode != System.Net.HttpStatusCode.OK)
                        {
                            MessageBox.Show("Failed to connect to HoloLens: \n" + newDevice.Portal.ConnectionFailedDescription);
                        }

                        _holoDeviceManager.AddDevice(newDevice);
                    }
                    break;
                case Mode.NFCReader:
                    _holoDeviceManager = new NFCHoloDeviceManager();
                    break;
            }
            if (_holoDeviceManager != null)
            {
                _holoDeviceManager.CurrentDeviceChanged += _holoDeviceManager_CurrentDeviceChanged;
                _holoDeviceManager_CurrentDeviceChanged(_holoDeviceManager, _holoDeviceManager.CurrentDevice);
            }

            DeviceListContainer.Visibility = _currentMode == Mode.DeviceList ? Visibility.Visible : Visibility.Hidden;
            CurrentDeviceContainer.Visibility = _currentMode != Mode.DisplayOnly ? Visibility.Visible : Visibility.Hidden;
            SelectDeviceList.Visibility = _currentMode == Mode.DeviceList ? Visibility.Visible : Visibility.Hidden;
        }

        private void _holoDeviceManager_CurrentDeviceChanged(object sender, HoloDevice currentDevice)
        {
            if (currentDevice != null)
            {
                Dispatcher.Invoke(new Action(() =>
                {
                    CurrentDeviceNameText.Text = currentDevice.ToString();
                }));
            }
            switch (_currentMode)
            {
                case Mode.DeviceList:
                    SelectDeviceList.SelectedItem = currentDevice;
                    break;
                case Mode.NFCReader:
                    if (currentDevice == null)
                    {
                        Dispatcher.Invoke(new Action(() =>
                        {
                            CurrentDeviceNameText.Text = "Waiting for NFC tag.";
                        }));
                    }
                    else if (!currentDevice.IsValid)
                    {
                        Dispatcher.Invoke(new Action(async () =>
                        {
                            if (MessageBox.Show("Press OK to program the NFC tag.", "Format NFC", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                            {
                                await currentDevice.Initialize(true);
                                if (!currentDevice.Save())
                                {
                                    MessageBox.Show("Couldn't write the data to the NFC tag.", "Error", MessageBoxButton.OK);
                                }
                                _holoDeviceManager.SetCurrentDevice(currentDevice);
                            }
                        }));
                    }
                    break;
            }
        }

        private void resetNFCTag_Click(object sender, RoutedEventArgs e)
        {
            if (_currentMode != Mode.NFCReader || _holoDeviceManager.CurrentDevice == null)
            {
                MessageBox.Show("Enable NFC mode and place a tag on the NFC reader");
            }
            switch (_currentMode)
            {
                case Mode.NFCReader:
                    if (_holoDeviceManager.CurrentDevice != null)
                    {
                        _holoDeviceManager.CurrentDevice.ResetDevice();
                        _holoDeviceManager.SetCurrentDevice(_holoDeviceManager.CurrentDevice);
                    }
                    break;
            }
        }

        private void InitializeKinect()
        {
            try
            {
                _kinect = new KinectCapture();
                _kinect.FrameUpdated += _kinect_FrameUpdated;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to initialize Kinect. \n" + ex.Message);
                Close();
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            _doneSound.Open(new Uri(@"Success.wav", UriKind.Relative));
            _doneSound.Volume = 1;

            //MouseDown += MainWindow_MouseDown;
            KeyDown += MainWindow_KeyDown;
            eyeBoxOffset.ValueChanged += EyeBoxOffset_ValueChanged;

            canvas.Children.Add(_leftPupilImageRecoVisual);
            canvas.Children.Add(_rightPupilImageRecoVisual);
            canvas.Children.Add(_leftEyeRectVisual);
            canvas.Children.Add(_rightEyeRectVisual);
            canvas.Children.Add(_headVisual);

            camera.Visibility = Visibility.Collapsed;
            testingOverlay.Visibility = Visibility.Collapsed;

            HideInstructionMessage();

            LoadSettings();

            #region PoissonDisk

            _poissonDisk[0] = new Point(-0.613392, 0.617481);
            _poissonDisk[1] = new Point(0.170019, -0.040254);
            _poissonDisk[2] = new Point(-0.299417, 0.791925);
            _poissonDisk[3] = new Point(0.645680, 0.493210);
            _poissonDisk[4] = new Point(-0.651784, 0.717887);
            _poissonDisk[5] = new Point(0.421003, 0.027070);
            _poissonDisk[6] = new Point(-0.817194, -0.271096);
            _poissonDisk[7] = new Point(-0.705374, -0.668203);
            _poissonDisk[8] = new Point(0.977050, -0.108615);
            _poissonDisk[9] = new Point(0.063326, 0.142369);
            _poissonDisk[10] = new Point(0.203528, 0.214331);
            _poissonDisk[11] = new Point(-0.667531, 0.326090);
            _poissonDisk[12] = new Point(-0.098422, -0.295755);
            _poissonDisk[13] = new Point(-0.885922, 0.215369);
            _poissonDisk[14] = new Point(0.566637, 0.605213);
            _poissonDisk[15] = new Point(0.039766, -0.396100);

            #endregion
        }

        private void Window_Closing(object sender, CancelEventArgs e)
        {
            ChangeIPDMode(Mode.DisplayOnly);
        }

        #region UI Event Handlers

        private void clearButton_Click(object sender, RoutedEventArgs e)
        {
            ResetState();
        }

        private void pauseButton_Click(object sender, RoutedEventArgs e)
        {
            TogglePause();
        }

        private void MainWindow_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            ToggleDebugMode();
        }

        private void EyeBoxOffset_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            _kinect.EyeBoxVerticalOffset = (int)eyeBoxOffset.Value;
            FindPupils();
        }

        private void MainWindow_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            switch (e.Key)
            {
                case System.Windows.Input.Key.PageUp:
                case System.Windows.Input.Key.PageDown:
                    if (e.Key == System.Windows.Input.Key.PageUp)
                        _currentZoom += 1;
                    if (e.Key == System.Windows.Input.Key.PageDown)
                        _currentZoom -= 1;

                    _currentZoom = Math.Min(Math.Max(_currentZoom, 1), 12);
                    break;
                case System.Windows.Input.Key.X:
                case System.Windows.Input.Key.Space:
                    ToggleDebugMode();
                    break;
                case System.Windows.Input.Key.C:
                    ResetState();
                    break;
                case System.Windows.Input.Key.P:
                    TogglePause();
                    break;
                case System.Windows.Input.Key.O:
                    _showOverlay = !_showOverlay;
                    instructionOverlay.Opacity = _showOverlay ? 1 : 0;
                    break;
                case System.Windows.Input.Key.Escape:
                    ResizeMode = ResizeMode.CanResize;
                    WindowState = WindowState.Normal;
                    WindowStyle = WindowStyle.SingleBorderWindow;
                    Topmost = false;
                    break;
                case System.Windows.Input.Key.Up:
                case System.Windows.Input.Key.Down:
                    if (e.Key == System.Windows.Input.Key.Up)
                        eyeBoxOffset.Value += 5;
                    else
                        eyeBoxOffset.Value -= 5;
                    FindPupils();
                    break;
                case System.Windows.Input.Key.R:

                    recordXEF_Click(this, null);
                    break;
                case System.Windows.Input.Key.E:
                    playXEF_Click(this, null);
                    break;
            }
        }

        private async void submitIpd_Click(object sender, RoutedEventArgs e)
        {
            if (_ipds.Count > 0 && _holoDeviceManager.CurrentDevice != null)
            {
                if (_holoDeviceManager.CurrentDevice.IsSaving)
                {
                    Log("Still saving previous result.");
                }
                else
                {
                    var result = await _holoDeviceManager.CurrentDevice.SubmitIPD((float)_ipds.Average());
                    Log("Submit IPD: " + result);
                    if (!result)
                        MessageBox.Show("Couldn't save IPD to HoloLens.");
                }
            }
            else if (!_hasValidIPD)
            {
                MessageBox.Show("IPD is not valid.");
            }
            else if (_holoDeviceManager.CurrentDevice == null)
            {
                MessageBox.Show("No HoloLens selected.");
            }
        }

        private async void setCredentials_Click(object sender, RoutedEventArgs e)
        {
            var newDevice = new HoloDevice();
            var dialogResult = await newDevice.Initialize(true);
            if (dialogResult)
            {
                if (newDevice.Portal != null && newDevice.Portal.ConnectionHttpStatusCode != System.Net.HttpStatusCode.OK)
                {
                    MessageBox.Show("Failed to connect to HoloLens: \n" + newDevice.Portal.ConnectionFailedDescription);
                }
                else
                {
                    _holoDeviceManager.AddDevice(newDevice);
                }
            }
        }

        #endregion

        private void ResetState()
        {
            _currentState = State.Attract;
            _finalIPD = 0.0f;
            _hasSavedIPDToDevice = false;
            _hasValidIPD = false;
            _currentAcceptableStdDev = _acceptableStdDevStart;
            _ipds = new List<double>();
            if (!_isDebugMode)
                debugMessage.Text = "";
            IPDText = string.Empty;
            DistanceText = "";
        }

        private void TogglePause(bool? forcedValue = null)
        {
            if (!_isDebugMode)
            {
                _isPaused = false;
                return;
            }

            _isPaused = forcedValue != null ? (bool)forcedValue : !_isPaused;

            if (_isPaused)
            {
                _kinect.PauseCapture();
                pauseButton.Content = "Resume";
                FindPupils();
            }
            else
            {
                _kinect.ResumeCapture();
                pauseButton.Content = "Pause";
            }
        }

        private void ToggleDebugMode(bool? forcedValue = null)
        {
            _isDebugMode = forcedValue != null ? (bool)forcedValue : !_isDebugMode;
            if (!_isDebugMode)
            {
                TogglePause(false);
                camera.Visibility = Visibility.Collapsed;
                ToolsPanel.Visibility = Visibility.Collapsed;
                InfoPanel.Visibility = Visibility.Collapsed;
                debugMessage.Visibility = Visibility.Collapsed;
            }
            else
            {
                HideInstructionMessage();
                testingOverlay.Visibility = Visibility.Collapsed;
                camera.Visibility = Visibility.Visible;
                ToolsPanel.Visibility = Visibility.Visible;
                InfoPanel.Visibility = Visibility.Visible;
                debugMessage.Visibility = Visibility.Visible;
            }
        }

        private void ShowInstructionMessage(string message)
        {
            InstructionOverlayText = message;
            if (instructionOverlay.Visibility != Visibility.Visible)
            {
                instructionOverlay.Visibility = Visibility.Visible;
            }
        }

        private void HideInstructionMessage()
        {
            InstructionOverlayText = string.Empty;
            if (instructionOverlay.Visibility != Visibility.Collapsed)
            {
                instructionOverlay.Visibility = Visibility.Collapsed;
            }
        }

        private void LoadSettings()
        {
            ShowIPDAfterMeasurement.IsChecked = Settings.Default.ShowIPDAfterMeasurement;
            Exposure.Value = Settings.Default.Exposure;
            TargetLuma.Value = Settings.Default.TargetLuma;
            AutoExposure.IsChecked = Settings.Default.AutoExposure;
            ModeList_Display.IsChecked = true;
            Task t = Task.Run(() =>
            {
                while (!_kinect.HasFrame)
                    Task.Delay(100).Wait();
                Dispatcher.Invoke(new Action(() =>
                {
                    if (!(_recorder.IsRecording || _recorder.IsPlaying))
                        _exposure.SetExposure((float)Exposure.Value);
                    Logger.Log("Exposure loaded/set: " + Exposure.Value);

                    ExposureText.Text = "Exposure: " + ((float)Exposure.Value).ToString("0.00");
                    LumaText.Text = "Target Luma: " + ((float)TargetLuma.Value).ToString("0.00");
                }));
            });
        }

        private void Settings_Changed(object sender, RoutedEventArgs e)
        {
            if (!_kinect.HasFrame)
                return;
            if (!(_recorder.IsRecording || _recorder.IsPlaying))
                _exposure.SetExposure((float)Exposure.Value);
            Logger.Log("Exposure set: " + Exposure.Value);
            Settings.Default.Exposure = (float)Exposure.Value;
            Settings.Default.ShowIPDAfterMeasurement = (bool)ShowIPDAfterMeasurement.IsChecked;
            Settings.Default.AutoExposure = (bool)AutoExposure.IsChecked;
            Settings.Default.TargetLuma = (float)TargetLuma.Value;
            Settings.Default.Save();

            ExposureText.Text = "Exposure: " + ((float)Exposure.Value).ToString("0.00");
            LumaText.Text = "Target Luma: " + ((float)TargetLuma.Value).ToString("0.00");
        }

        private void Zoom(Point point, double scale)
        {
            scaleTransform.ScaleX = Utilities.Lerp(scaleTransform.ScaleX, scale, .05);
            scaleTransform.ScaleY = Utilities.Lerp(scaleTransform.ScaleY, scale, .05);
            translateTransform.X = Utilities.Lerp(translateTransform.X, _kinect.ColorBitmap.Width / 2 - point.X, .025);
            translateTransform.Y = Utilities.Lerp(translateTransform.Y, _kinect.ColorBitmap.Height / 2 - point.Y, .025);
        }

        private void LerpShapePosition(Shape element, Point targetPoint, float amount)
        {
            LerpShapePosition(element, new Rect(targetPoint, element.RenderSize), amount);
        }

        private void LerpShapePosition(Shape element, Rect targetRect, float amount)
        {
            if (!double.IsInfinity(targetRect.X) && !double.IsInfinity(targetRect.Y))
            {
                var currentLeft = Canvas.GetLeft(element);
                var currentTop = Canvas.GetTop(element);
                Canvas.SetLeft(element, Utilities.Lerp(currentLeft, targetRect.X, amount));
                Canvas.SetTop(element, Utilities.Lerp(currentTop, targetRect.Y, amount));
                element.Width = targetRect.Width;
                element.Height = targetRect.Height;
            }
        }

        private void UpdateDebugOverlay()
        {
            LerpShapePosition(_rightEyeRectVisual, _kinect.RightEyeRect, .25f);
            LerpShapePosition(_leftEyeRectVisual, _kinect.LeftEyeRect, .25f);
            LerpShapePosition(_rightPupilImageRecoVisual, _rightPupilRectImageReco, .25f);
            LerpShapePosition(_leftPupilImageRecoVisual, _leftPupilRectImageReco, .25f);
            LerpShapePosition(_headVisual, _kinect.HeadPoint, .25f);

            if (!_isPaused)
            {
                _leftEyeRectVisual.Visibility =
                _rightEyeRectVisual.Visibility =
                _rightPupilImageRecoVisual.Visibility =
                _leftPupilImageRecoVisual.Visibility = Visibility.Hidden;

                if (_kinect.HasFace)
                {
                    DistanceText = _kinect.HeadDist.ToString("0.0");
                    if (_kinect.HeadDist < _minDistance || _kinect.HeadDist > _maxDistance)
                    {
                        DistanceText += (_kinect.HeadDist < _minDistance ? " - too close" : " - too far");
                    }
                    else if (!_kinect.IsFacingCamera)
                    {
                        DistanceText = "Face the camera";
                    }
                    else
                    {
                        _leftEyeRectVisual.Visibility =
                        _rightEyeRectVisual.Visibility =
                        _rightPupilImageRecoVisual.Visibility =
                        _leftPupilImageRecoVisual.Visibility = Visibility.Visible;
                    }
                }
                else
                {
                    DistanceText = "Step into view of the camera";
                }

                IPDText = (_ipds.Count > 0 ? _ipds.Average().ToString("0.0") : "-");
            }
        }

        private async void UpdateState()
        {
            testingOverlay.Visibility = Visibility.Collapsed;

            switch (_currentState)
            {
                // show attract message until we have a user (and face)
                case State.Attract:
                    if (_kinect.HasFace)
                    {
                        _currentState = State.HasUser;
                        _currentMeasureTimeStart = DateTime.Now;
                    }
                    ShowInstructionMessage("Step into view of the camera");
                    break;

                case State.HasUser:
                case State.Measuring:

                    if (!_isPaused && !_kinect.HasBody && DateTime.Now.Subtract(_kinect.LastBodySeenTime).TotalSeconds > 5)
                    {
                        // If we've lost the user, reset the state.
                        ResetState();
                    }
                    else if (!_isPaused && _kinect.HasBody && !_hasValidIPD && DateTime.Now.Subtract(_currentMeasureTimeStart).TotalMilliseconds > _maxMeasureTime)
                    {
                        _lastDoneTime = DateTime.Now;
                        _currentState = State.MeasuringFailed;
                    }
                    else if (!_isPaused && _kinect.HasBody && _hasValidIPD)// && DateTime.Now.Subtract(_kinect.LastBodySeenTime).TotalSeconds < 5)
                    {
                        _lastDoneTime = DateTime.Now;
                        _finalIPD = _ipds.Average();
                        if (_currentMode != Mode.DisplayOnly)
                        {
                            // If we still have a user and have a valid IPD, start the programming.
                            _currentState = State.Programming;
                        }
                        else
                        {
                            // No device, so just switch to the done state.
                            _currentState = State.Done;
                        }
                    }
                    else
                    {
                        // Otherwise we have a user so should start the testing process.
                        HideInstructionMessage();
                        _currentState = State.Measuring;
                        if (_kinect.HeadDist > _minDistance && _kinect.HeadDist < _maxDistance)
                        {
                            if (!_isPaused && !(_kinect.HasFace && _kinect.IsFacingCamera))
                            {
                                ShowInstructionMessage("Face the camera");
                            }
                            else if (!_isPaused)
                            {
                                if (DateTime.Now.Second % 10 < 5)
                                    testingInstructionText.Text = "Eyes wide";
                                else
                                    testingInstructionText.Text = "Look here";
                                //testingInstructionText.Text += "(" + _currentAcceptableStdDev.ToString("0.00") + ")";
                                testingIPDText.Text = (_ipds.Count > 0 ? _ipds.Average().ToString("0.0") : "-");
                                testingOverlay.Visibility = Visibility.Visible;
                            }
                        }
                        else
                        {
                            if (!_isPaused)
                                ShowInstructionMessage("Move " + (_kinect.HeadDist < _minDistance ? "further away" : "closer"));
                        }

                        IPDText = (_ipds.Count > 0 ? _ipds.Average().ToString("0.0") : "-");
                    }
                    break;
                case State.Programming:
                    ShowInstructionMessage("Saving to HoloLens");
                    if (_holoDeviceManager.CurrentDevice != null)
                    {
                        if (_currentMode != Mode.DisplayOnly && _holoDeviceManager.CurrentDevice != null && !_holoDeviceManager.CurrentDevice.IsSaving)
                        {
                            Log("-- Saving IPD to device");
                            _hasSavedIPDToDevice = await _holoDeviceManager.CurrentDevice.SubmitIPD((float)_finalIPD);
                            if (_hasSavedIPDToDevice)
                            {
                                _currentState = State.Done;
                                _doneSound.Position = TimeSpan.Zero;
                                _doneSound.Play();

                            }
                            else
                            {
                                Log("** SubmitIPD failed");
                                _currentState = State.DeviceError;
                            }
                        }
                    }
                    else
                    {
                        Log("** _holoDevice = null");
                        _currentState = State.DeviceError;
                    }

                    break;
                case State.MeasuringFailed:
                    ShowInstructionMessage("Couldn't find a stable IPD.");
                    if (DateTime.Now.Subtract(_lastDoneTime).TotalMilliseconds > _resetStateAfterMs)
                        ResetState();
                    break;
                case State.DeviceError:
                case State.Done:
                    var message = "Measurement complete.";
                    //message += "(std dev: " + _currentAcceptableStdDev.ToString("0.00") + ")";
                    if (Settings.Default.ShowIPDAfterMeasurement || _currentMode == Mode.DisplayOnly)
                        message += "\nIPD is " + _finalIPD.ToString("0.0");
                    if (_currentState == State.DeviceError)
                    {
                        message += "\nCouldn't connect to the HoloLens. Ensure it is turned on and that the IP address hasn't changed.";
                    }
                    else
                    {
                        if (_hasSavedIPDToDevice)
                            message += "\nIPD saved to HoloLens";
                    }

                    ShowInstructionMessage(message);

                    if (DateTime.Now.Subtract(_lastDoneTime).TotalMilliseconds > _resetStateAfterMs)
                        ResetState();

                    break;
            }
        }

        private void _kinect_FrameUpdated(object sender, EventArgs e)
        {
            UpdateFrame();
        }

        void UpdateFrame()
        {
            if (_kinect.HasFace && _kinect.IsFacingCamera &&
                DateTime.Now.Subtract(_lastPupilUpdate).TotalMilliseconds > _pupilUpdateFrequencyMs &&
                _kinect.HeadDist > _minDistance && _kinect.HeadDist < _maxDistance)
            {
                if (!_isPaused && (_currentState == State.Measuring || _isDebugMode))
                    FindPupils();
                _lastPupilUpdate = DateTime.Now;
            }

            if (!_isPaused)
            {
                if (_kinect.HasFace)
                {
                    canvas.Visibility = Visibility.Visible;
                    Zoom(_kinect.HeadPoint, _currentZoom);
                }
                else
                {
                    canvas.Visibility = Visibility.Collapsed;
                    scaleTransform.ScaleX = Utilities.Lerp(scaleTransform.ScaleX, 1, .05);
                    scaleTransform.ScaleY = Utilities.Lerp(scaleTransform.ScaleY, 1, .05);
                    translateTransform.X = Utilities.Lerp(translateTransform.X, 0, .02);
                    translateTransform.Y = Utilities.Lerp(translateTransform.Y, 0, .02);
                }
            }

            if (_isDebugMode)
            {
                UpdateDebugOverlay();
            }
            else
            {
                UpdateState();
            }

            RecordingIndicator.Visibility = _recorder.IsRecording ? Visibility.Visible : Visibility.Collapsed;
        }

        volatile bool busy = false;
        private bool FindPupils()
        {
            if (busy)
            {
                return false;
            }

            if (_kinect.LeftEyeRect.Y < 0 || _kinect.RightEyeRect.Y < 0 || _kinect.LeftEyeRect.Bottom >= 1080 || _kinect.RightEyeRect.Bottom >= 1080)
            {
                Log("** _leftEyeRect or _rightEyeRect outside screen ");
                return false;
            }

            busy = true;

            Size leftPixelSize = new Size(_kinect.LeftEyeRect.Width, _kinect.LeftEyeRect.Height);
            byte[] leftPixelArray = _kinect.ColorBitmap.ToArray(new Int32Rect((int)_kinect.LeftEyeRect.X, (int)_kinect.LeftEyeRect.Y, (int)_kinect.LeftEyeRect.Width, (int)_kinect.LeftEyeRect.Height));

            Size rightPixelSize = new Size(_kinect.RightEyeRect.Width, _kinect.RightEyeRect.Height);
            byte[] rightPixelArray = _kinect.ColorBitmap.ToArray(new Int32Rect((int)_kinect.RightEyeRect.X, (int)_kinect.RightEyeRect.Y, (int)_kinect.RightEyeRect.Width, (int)_kinect.RightEyeRect.Height));

            if ((bool)AutoExposure.IsChecked && !(_recorder.IsRecording || _recorder.IsPlaying))
            {
                var lumRect = new Int32Rect();
                lumRect.X = (int)_kinect.LeftEyeRect.X - 20;
                lumRect.Width = (int)_kinect.RightEyeRect.Right - (int)_kinect.LeftEyeRect.X + 20;
                lumRect.Y = Math.Min((int)_kinect.LeftEyeRect.Y, (int)_kinect.RightEyeRect.Y) - 20;
                lumRect.Height = Math.Max((int)_kinect.LeftEyeRect.Bottom, (int)_kinect.RightEyeRect.Bottom) - Math.Min((int)_kinect.LeftEyeRect.Y, (int)_kinect.RightEyeRect.Y) + 20;

                double lum = _kinect.ColorBitmap.CalculateAverageLuma(lumRect);
                if (lum > 0)
                {
                    if (lum < Settings.Default.TargetLuma)
                    {
                        Dispatcher.Invoke(new Action(() =>
                        {
                            Exposure.Value += .25f;
                            _exposure.SetExposure((float)Exposure.Value);
                        }));
                    }
                    else if (lum > Settings.Default.TargetLuma + .01f)
                    {
                        Dispatcher.Invoke(new Action(() =>
                        {
                            Exposure.Value -= .25f;
                            _exposure.SetExposure((float)Exposure.Value);
                        }));
                    }
                }
            }

            Task.Run(() =>
            {
                Rect rightPupilLocalRect = Rect.Empty;
                Point rightPupilLocalCenter = new Point();
                Rect leftPupilLocalRect = Rect.Empty;
                Point leftPupilLocalCenter = new Point();

                var leftEyeTask = _pupilDetect.FindPupil(ref leftPixelArray, (int)leftPixelSize.Width, (int)leftPixelSize.Height);
                var rightEyeTask = _pupilDetect.FindPupil(ref rightPixelArray, (int)rightPixelSize.Width, (int)rightPixelSize.Height);
                if (leftEyeTask != null && rightEyeTask != null)
                {
                    Task.WaitAll(new Task[] { leftEyeTask, rightEyeTask });

                    leftPupilLocalRect = leftEyeTask.Result.rectangle;
                    leftPupilLocalCenter = leftEyeTask.Result.center;
                    rightPupilLocalRect = rightEyeTask.Result.rectangle;
                    rightPupilLocalCenter = rightEyeTask.Result.center;
                }

                _foundPupils =
                    rightPupilLocalRect.Width > 0 &&
                    rightPupilLocalRect.Height > 0 &&
                    leftPupilLocalRect.Width > 0 &&
                    leftPupilLocalRect.Height > 0;
                if (_foundPupils)
                {
                    _leftPupilRectImageReco.Width = leftPupilLocalRect.Width;
                    _leftPupilRectImageReco.Height = leftPupilLocalRect.Height;
                    _leftPupilRectImageReco.X = _kinect.LeftEyeRect.X + leftPupilLocalRect.X;
                    _leftPupilRectImageReco.Y = _kinect.LeftEyeRect.Y + leftPupilLocalRect.Y;

                    _rightPupilRectImageReco.Width = rightPupilLocalRect.Width;
                    _rightPupilRectImageReco.Height = rightPupilLocalRect.Height;
                    _rightPupilRectImageReco.X = _kinect.RightEyeRect.X + rightPupilLocalRect.X;
                    _rightPupilRectImageReco.Y = _kinect.RightEyeRect.Y + rightPupilLocalRect.Y;

                    _leftPupilCenterImageReco = new Point(_kinect.LeftEyeRect.X + leftPupilLocalCenter.X, _kinect.LeftEyeRect.Y + leftPupilLocalCenter.Y);
                    _rightPupilCenterImageReco = new Point(_kinect.RightEyeRect.X + rightPupilLocalCenter.X, _kinect.RightEyeRect.Y + rightPupilLocalCenter.Y);

                    Vector3D leftPupilImageReco3D;
                    Vector3D rightPupilImageReco3D;

                    string debug = "";
                    {
                        leftPupilImageReco3D = _kinect.GetWeightedAverageCSP(_leftPupilCenterImageReco.X, _leftPupilCenterImageReco.Y, 5, _poissonDisk);
                        debug += _leftPupilCenterImageReco.X.ToString("0.0") + "," +
                            _leftPupilCenterImageReco.Y.ToString("0.0") + " - " +
                                            (leftPupilImageReco3D.X * 1000).ToString("0.0") + ", " +
                                            (leftPupilImageReco3D.Y * 1000).ToString("0.0") + ", " +
                                            (leftPupilImageReco3D.Z * 1000).ToString("0.0") + "  -  ";
                    }
                    {
                        rightPupilImageReco3D = _kinect.GetWeightedAverageCSP(_rightPupilCenterImageReco.X, _rightPupilCenterImageReco.Y, 5, _poissonDisk);
                        debug += _rightPupilCenterImageReco.X.ToString("0.0") + "," +
                            _rightPupilCenterImageReco.Y.ToString("0.0") + " - " +
                                            (rightPupilImageReco3D.X * 1000).ToString("0.0") + ", " +
                                            (rightPupilImageReco3D.Y * 1000).ToString("0.0") + ", " +
                                            (rightPupilImageReco3D.Z * 1000).ToString("0.0");
                    }

                    var newIpd = Utilities.VectorLength(leftPupilImageReco3D, rightPupilImageReco3D) * 1000;
                    var isValidIpd = UpdateAverageIPD(newIpd);

                    debug += " = " + (newIpd).ToString("0.0") + (_ipds.Count > 0 ? "  Ave: " + _ipds.Average().ToString("0.0") : "") + (isValidIpd ? "" : " *** Rejected");
                    Log(debug);
                }
                else
                {
                    Log("** Couldn't find pupils ");
                }
                busy = false;
            });
            return true;
        }

        private bool UpdateAverageIPD(double newIpd)
        {
            if (_ipds.Count > _ipdAverageCount)
                _ipds.RemoveAt(0);

            if (double.IsNaN(newIpd) ||
                newIpd > 80 || newIpd < 50 ||
                (_ipds.Count > _minIpds && Math.Abs(_ipds.Average() - newIpd) > 5))
            {
                // rejection conditions
                return false;
            }
            else
            {
                var lastAverage = _ipds.Count > 0 ? _ipds.Average() : 0;
                _ipds.Add(newIpd);
                var newAverage = _ipds.Average();

                double stdDev = 0.0;
                foreach (var ipd in _ipds)
                {
                    stdDev += Math.Pow((newAverage - ipd), 2);
                }

                stdDev /= _ipds.Count();
                stdDev = Math.Sqrt(stdDev);
                stdDev = stdDev > _maxStdDev ? _maxStdDev : stdDev;

                if (_ipds.Count > _minIpds)
                {
                    _currentAcceptableStdDev += _acceptableStdDevIncreasePerSec / (1000 / _pupilUpdateFrequencyMs);
                }
                if (_ipds.Count > _minIpds && stdDev <= _currentAcceptableStdDev)
                    _hasValidIPD = true;

                return true;
            }
        }

        private void Log(string message)
        {
            Dispatcher.Invoke(new Action(() =>
            {
                try
                {
                    debugMessage.Text = message + "\n" + debugMessage.Text;
                    Logger.Log(message);
                }
                catch { }
            }));
        }

        private void AutoExposure_Checked(object sender, RoutedEventArgs e)
        {
            Settings_Changed(this, null);
        }

        private void SelectDeviceList_Selected(object sender, RoutedEventArgs e)
        {
            _holoDeviceManager.SetCurrentDevice((HoloDevice)SelectDeviceList.SelectedItem);
        }

        private void DeviceDeleteButton_Click(object sender, RoutedEventArgs e)
        {
            _holoDeviceManager.RemoveDevice((HoloDevice)((Button)sender).DataContext);
        }

        private void recordXEF_Click(object sender, RoutedEventArgs e)
        {
            if (!_recorder.IsPlaying)
            {
                if (_recorder.IsRecording)
                {
                    Log("StopRecording");
                    _recorder.StopRecording();
                }
                else
                {
                    var now = DateTime.Now.Year + "" + DateTime.Now.Month.ToString("00") + "" + DateTime.Now.Day.ToString("00") + "_" + DateTime.Now.Hour.ToString("00") + "" + DateTime.Now.Minute.ToString("00") + "" + DateTime.Now.Second.ToString("00");
                    if (!Directory.Exists("DebugData/ManualCapture/" + now))
                        Directory.CreateDirectory("DebugData/ManualCapture/" + now);
                    var fileName = "DebugData/ManualCapture/" + now + "/recording.xef";
                    Log("RecordClip: " + fileName);
                    _recorder.RecordClip(new DirectoryInfo(fileName).FullName);
                }
            }
        }

        private void playXEF_Click(object sender, RoutedEventArgs e)
        {
            if (!_recorder.IsRecording)
            {
                if (_recorder.IsPlaying)
                {
                    Log("StopPlaying");
                    _recorder.StopPlayback();
                }
                else
                {
                    var fileDialog = new System.Windows.Forms.OpenFileDialog();
                    fileDialog.Filter = "Kinect Studio Recording|*.xef";
                    fileDialog.InitialDirectory = new DirectoryInfo("DebugData/ManualCapture/").FullName;
                    var result = fileDialog.ShowDialog();
                    switch (result)
                    {
                        case System.Windows.Forms.DialogResult.OK:
                            var fileName = fileDialog.FileName;
                            Log("RecordClip: " + fileName);
                            _recorder.PlayClip(new DirectoryInfo(fileName).FullName);
                            break;
                        case System.Windows.Forms.DialogResult.Cancel:
                        default:
                            break;
                    }
                }
            }
        }

        private void ModeList_Checked(object sender, RoutedEventArgs e)
        {
            Mode newMode = Mode.DisplayOnly;
            if ((RadioButton)sender == ModeList_List)
                newMode = Mode.DeviceList;
            else if ((RadioButton)sender == ModeList_NFC)
                newMode = Mode.NFCReader;
            ChangeIPDMode(newMode);
        }
    }

    public enum Mode : int
    {
        DisplayOnly,
        DeviceList,
        NFCReader
    }

    public enum State : int
    {
        Attract,
        DeviceError,
        HasUser,
        Measuring,
        MeasuringFailed,
        Programming,
        Done
    }
}