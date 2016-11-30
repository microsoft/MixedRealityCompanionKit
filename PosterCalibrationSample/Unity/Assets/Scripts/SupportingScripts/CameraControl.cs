// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

namespace PosterAlignment
{
    public class CameraControl : MonoBehaviour
    {
        public Vector2 PanSensitivity = new Vector2(60.0f, 60.0f);
        public Vector2 RotateSensitivity = new Vector2(120.0f, 120.0f);
        public float ZoomSensititity = 450.0f;
        public bool UseGamePad = false;

        void Update()
        {
            float sensitivityScale = Input.GetKey(KeyCode.LeftShift) ? 0.1f : 1.0f;
            float finalScale = Time.deltaTime * sensitivityScale;

            transform.Translate(new Vector3(0.0f, 0.0f, Input.GetAxis("Mouse ScrollWheel") * ZoomSensititity * finalScale));
            transform.Translate(new Vector3(Input.GetAxis("Horizontal") * 2.5f * Time.deltaTime,
                    0,
                                                Input.GetAxis("Vertical") * 2.5f * Time.deltaTime
                                                ));
            if (UseGamePad)
            {
                var transY = Input.GetAxis("Right Trigger") - Input.GetAxis("Left Trigger");
                transform.Translate(new Vector3(Input.GetAxis("Left Joystick X") * 2.5f * Time.deltaTime,
                    transY * 2.0f * Time.deltaTime,
                                                Input.GetAxis("Left Joystick Y") * 2.5f * Time.deltaTime
                                                ));

                transform.eulerAngles += new Vector3(-Input.GetAxis("Right Joystick Y") * 80.0f * Time.deltaTime,
                                                      Input.GetAxis("Right Joystick X") * 80.0f * Time.deltaTime,
                                                      0.0f);
            }
            //pan
            if (Input.GetMouseButton(2) || Input.GetKey(KeyCode.R) || Input.GetKey(KeyCode.F))
            {
                float y = Input.GetKey(KeyCode.F) ? -.01f : 0;
                y += Input.GetKey(KeyCode.R) ? .01f : 0;
                y += Input.GetAxis("Mouse Y");

                transform.Translate(new Vector3(Input.GetAxis("Mouse X") * PanSensitivity.x * finalScale,
                                                y * PanSensitivity.y * finalScale,
                                                0.0f));
            }
            //rotate
            else if (Input.GetMouseButton(1) || Input.GetKey(KeyCode.Q) || Input.GetKey(KeyCode.E))
            {
                float y = Input.GetKey(KeyCode.Q) ? -.5f : 0;
                y += Input.GetKey(KeyCode.E) ? .5f : 0;
                y += Input.GetAxis("Mouse X");

                transform.eulerAngles += new Vector3(-Input.GetAxis("Mouse Y") * RotateSensitivity.y * finalScale,
                                                      y * RotateSensitivity.x * finalScale,
                                                      0.0f);
            }
        }
    }
}