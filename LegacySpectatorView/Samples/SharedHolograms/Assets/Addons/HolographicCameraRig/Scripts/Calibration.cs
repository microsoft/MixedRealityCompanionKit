// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using HoloToolkit.Unity;
using System.IO;
using System;

namespace SpectatorView
{
    public class Calibration : SpectatorView.SV_Singleton<Calibration>
    {
        [Tooltip("Enable this checkbox if your camera is mounted below or to the left of your camera.")]
        public bool RotateCalibration = false;
        private bool prevRotateCalibration;

        public Vector3 Translation { get; private set; }
        public Quaternion Rotation { get; private set; }
        public Vector2 DSLR_fov { get; private set; }
        public Vector4 DSLR_distortion { get; private set; }
        public Vector4 DSLR_matrix { get; private set; }

        void Start()
        {
            prevRotateCalibration = RotateCalibration;

            Translation = Vector3.zero;
            Rotation = Quaternion.identity;
            DSLR_fov = Vector2.one * 60.0f;
            DSLR_distortion = Vector4.zero;
            DSLR_matrix = Vector4.zero;

            ReadCalibrationData();
        }

        void OnValidate()
        {
            if (prevRotateCalibration != RotateCalibration)
            {
                prevRotateCalibration = RotateCalibration;

                Vector3 euler = -1 * Rotation.eulerAngles;
                Rotation = Quaternion.Euler(euler);

                Translation *= -1;

                gameObject.transform.localPosition = Translation;
                gameObject.transform.localRotation = Rotation;
            }
        }

        private void ReadCalibrationData()
        {
            string filePath = Application.dataPath + "/" + "CalibrationData.txt";

            if (!File.Exists(filePath))
            {
                Debug.LogError("CalibrationData.txt could not be found.");
                return;
            }

            using (FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read))
            {
                using (StreamReader sr = new StreamReader(fs))
                {
                    string line;
                    while ((line = sr.ReadLine()) != null)
                    {
                        // Ignore comments.
                        if (line.Trim().StartsWith("#"))
                        {
                            continue;
                        }

                        String[] tokens = line.Split(new string[] { "\n", ":", ",", " " }, StringSplitOptions.RemoveEmptyEntries);
                        string entryType = tokens[0].Trim().ToLower();

                        if (entryType == "translation")
                        {
                            Translation = new Vector3((float)Convert.ToDouble(tokens[1]),
                                (float)Convert.ToDouble(tokens[2]),
                                (float)Convert.ToDouble(tokens[3]));
                            if (!RotateCalibration)
                            {
                                Translation *= -1;
                            }

                            // Convert from OpenCV space to Unity space.
                            Translation = new Vector3(Translation.x, Translation.y, -1 * Translation.z);

                            Debug.Log("Loaded calibration translation: " + Translation.x + ", " + Translation.y + ", " + Translation.z);
                        }

                        else if (entryType == "rotation")
                        {
                            Rotation = Quaternion.LookRotation(
                                // Third column as forward direction.
                                new Vector3(
                                    (float)Convert.ToDouble(tokens[7]),
                                    (float)Convert.ToDouble(tokens[8]),
                                    (float)Convert.ToDouble(tokens[9])
                                ),
                                // Second column as up direction.
                                new Vector3(
                                    (float)Convert.ToDouble(tokens[4]),
                                    (float)Convert.ToDouble(tokens[5]),
                                    (float)Convert.ToDouble(tokens[6])
                                )
                            );

                            Vector3 euler = Rotation.eulerAngles;
                            if (!RotateCalibration)
                            {
                                euler *= -1;
                            }

                            // Convert from OpenCV space to Unity space.
                            euler.y *= -1;
                            Rotation = Quaternion.Euler(euler);

                            Debug.Log("Loaded calibration quaternion: " + Rotation.x + ", " + Rotation.y + ", " + Rotation.z + ", " + Rotation.w);
                        }

                        else if (entryType == "dslr_fov")
                        {
                            DSLR_fov = new Vector2((float)Convert.ToDouble(tokens[1]),
                                (float)Convert.ToDouble(tokens[2]));

                            Debug.Log("Loaded calibration fov: " + DSLR_fov.x + ", " + DSLR_fov.y);
                        }

                        else if (entryType == "dslr_distortion")
                        {
                            DSLR_distortion = new Vector4((float)Convert.ToDouble(tokens[1]),
                                (float)Convert.ToDouble(tokens[2]),
                                (float)Convert.ToDouble(tokens[3]),
                                (float)Convert.ToDouble(tokens[4]));

                            Debug.Log("Loaded calibration dslr distortion: " + DSLR_distortion.x + ", " + DSLR_distortion.y + ", " + DSLR_distortion.z + ", " + DSLR_distortion.w);
                        }

                        else if (entryType == "dslr_camera_matrix")
                        {
                            DSLR_matrix = new Vector4((float)Convert.ToDouble(tokens[1]),
                                (float)Convert.ToDouble(tokens[2]),
                                (float)Convert.ToDouble(tokens[3]),
                                (float)Convert.ToDouble(tokens[4]));

                            Debug.Log("Loaded calibration dslr matrix: " + DSLR_matrix.x + ", " + DSLR_matrix.y + ", " + DSLR_matrix.z + ", " + DSLR_matrix.w);
                        }
                    }
                }
            }
        }
    }
}
