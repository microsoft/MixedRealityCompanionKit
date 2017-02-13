// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using HoloToolkit.Unity;
using HoloToolkit.Sharing;

namespace SpectatorView
{
    public partial class SceneManager : SpectatorView.SV_Singleton<SceneManager>
    {
        public GameObject[] objectsToHideInPicture;

        private Vector3 lastPosition;
        private Quaternion lastRotation;
        private Vector3 lastScale;

        private SpectatorView.SV_CustomMessages customMessages = null;

        void Start()
        {
            customMessages = SpectatorView.SV_CustomMessages.Instance;
            if (customMessages != null)
            {
                RegisterCustomMessages();
            }

            lastPosition = transform.localPosition;
            lastRotation = transform.localRotation;
            lastScale = transform.localScale;

#if UNITY_EDITOR
            HideObjects();
#endif
        }

        void RegisterCustomMessages()
        {
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.SceneTransform] = OnSceneTransform;
        }

#if UNITY_EDITOR
        void OnValidate()
        {
            HideObjects();
        }

        private void HideObjects()
        {
            if (objectsToHideInPicture == null)
            {
                return;
            }

            foreach (GameObject go in objectsToHideInPicture)
            {
                if (go != null)
                {
                    Renderer[] renderers = go.GetComponentsInChildren<Renderer>();
                    if (renderers.Length > 0)
                    {
                        foreach (Renderer renderer in renderers)
                        {
                            renderer.enabled = false;
                        }
                    }
                    else
                    {
                        go.SetActive(false);
                    }
                }
            }
        }
#endif

#if UNITY_EDITOR
        void Update()
        {
            if (customMessages == null)
            {
                customMessages = SpectatorView.SV_CustomMessages.Instance;
                if (customMessages != null)
                {
                    RegisterCustomMessages();
                }
            }

            if (customMessages == null)
            {
                return;
            }

            if (!lastPosition.Equals(transform.localPosition) || !lastRotation.Equals(transform.localRotation) || !lastScale.Equals(transform.localScale))
            {
                customMessages.SendSceneTransform(transform.localPosition, transform.localRotation, transform.localScale);

                lastPosition = transform.localPosition;
                lastRotation = transform.localRotation;
                lastScale = transform.localScale;
            }
        }
#endif

        /// <summary>
        /// When a remote system has a transform for us, we'll get it here.
        /// </summary>
        /// <param name="msg"></param>
        void OnSceneTransform(HoloToolkit.Sharing.NetworkInMessage msg)
        {
            msg.ReadInt64();

            transform.localPosition = SpectatorView.SV_CustomMessages.Instance.ReadVector3(msg);
            transform.localRotation = SpectatorView.SV_CustomMessages.Instance.ReadQuaternion(msg);
            transform.localScale = SpectatorView.SV_CustomMessages.Instance.ReadVector3(msg);
        }

        public void SendCurrentScene()
        {
            if (customMessages == null || transform == null)
            {
                return;
            }

            customMessages.SendSceneTransform(transform.localPosition, transform.localRotation, transform.localScale);
        }
    }
}