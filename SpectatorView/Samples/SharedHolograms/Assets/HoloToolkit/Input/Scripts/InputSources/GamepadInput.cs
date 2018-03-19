// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using UnityEngine;
#if UNITY_2017_2_OR_NEWER
using UnityEngine.XR.WSA.Input;
#else
using UnityEngine.VR.WSA.Input;
#endif

namespace HoloToolkit.Unity.InputModule
{
    public class GamepadInput : BaseInputSource
    {
        [Tooltip("Game pad button to press for air tap.")]
        public string GamePadButtonA = "Fire1";
        [Tooltip("Change this value to give a different source id to your controller.")]
        public uint GamePadId = 50000;

        protected override void Start()
        {
            base.Start();
        }

        private void Update()
        {
            // TODO: Should this handle Submit from Edit > ProjectSettings > Input ?
            // Raise the air tap event when A button is pressed.
            if (Input.GetButtonDown(GamePadButtonA))
            {
                Debug.Log("Gamepad A worked!");
                inputManager.RaiseInputClicked(this, GamePadId, 1);
            }
        }

        public override SupportedInputInfo GetSupportedInputInfo(uint sourceId)
        {
            // Since the game pad is not a 3dof or 6dof controller.
            return SupportedInputInfo.None;
        }

        public override bool TryGetOrientation(uint sourceId, out Quaternion orientation)
        {
            throw new NotImplementedException();
        }

        public override bool TryGetPosition(uint sourceId, out Vector3 position)
        {
            throw new NotImplementedException();
        }
    }
}