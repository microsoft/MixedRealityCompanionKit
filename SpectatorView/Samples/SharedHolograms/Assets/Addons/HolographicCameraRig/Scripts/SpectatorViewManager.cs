// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity;
using HoloToolkit.Sharing;

namespace SpectatorView
{
    public class SpectatorViewManager : SV_Singleton<SpectatorViewManager>
    {
        // If any of these prefabs exist already, they will be used by default.
        // However, if an IP here is different from the prefabs in the scene, they will be recreated.
        [Header("Required")]
        [SerializeField]
        [Tooltip("Drag the HolographicCameraRig/Addon/Prefabs/HolographicCameraManager prefab here.  ")]
        GameObject HolographicCameraManager;
        [SerializeField]
        [Tooltip("Drag the application's anchor prefab here.  If one does not exist, drag the provided Anchor prefab here from the HolographicCameraRig/Addon/Prefabs directory.")]
        GameObject Anchor;
        [SerializeField]
        [Tooltip("Drag the HoloToolkit Sharing prefab here.")]
        GameObject Sharing;

        [SerializeField]
        [Tooltip("The IP of the HoloLens device mounted to the spectator view camera.")]
        public string SpectatorViewIP = string.Empty;
        [SerializeField]
        [Tooltip("The IP of the computer running the HoloToolkit Sharing Service.  This must be on the same subnet as local PC and the spectator view rig.")]
        string SharingServiceIP = string.Empty;

        [Header("Optional")]
        [SerializeField]
        // Use IP to disambiguate scenarios where multiple Unity clients could be connected to the same sharing service.
        // This could happen if you have multiple Spectator View rigs in the same experience.
        [Tooltip("Set this to the IP of the machine running Unity you wish to connect this spectator view rig to.  You can leave this field blank if you are only using 1 spectator view rig.")]
        string LocalComputerIP = string.Empty;

        [Tooltip("Comma separated IPs for other client HoloLens devices")]
        public string ClientHololensCSV = string.Empty;

        protected override void Awake()
        {
            if (SpectatorViewIP.Trim() == string.Empty)
            {
                Debug.LogWarning("If SpectatorViewIP is not populated, it must be updated at runtime.");
            }

            if (SharingServiceIP.Trim() == string.Empty)
            {
                Debug.LogError("Sharing Service IP must be populated.");
            }

            if (HolographicCameraManager == null)
            {
                Debug.LogError("HolographicCameraManager must be populated.");
            }

            if (Anchor == null)
            {
                Debug.LogError("Anchor must be populated.");
            }

            if (Sharing == null)
            {
                Debug.LogError("Sharing must be populated.");
            }

            InstantiateSharing();
        }

        public void InstantiateSharing()
        {
            SharingStage stage = SharingStage.Instance;

            if (stage == null)
            {
                GameObject sharingObj = GameObject.Find(Sharing.name);

                if (sharingObj != null)
                {
                    stage = sharingObj.GetComponent<SharingStage>();
                }
            }

            // Instantiate Sharing.
            if (stage == null)
            {
                CreateSharingStage(null);
            }
            else
            {
                // A sharing service already exists, ensure it has the correct IP.
                if (stage.ServerAddress != SharingServiceIP)
                {
                    // IP was wrong, must reconnect.
                    stage.ConnectToServer(SharingServiceIP, stage.ServerPort);
                }
            }

            Sharing.SetActive(true);
        }

        public void UpdateSpectatorViewIP()
        {
            SV_CustomMessages.Instance.SendUpdatedIPs(SpectatorViewIP);

            if (SpectatorViewIP != SpectatorView.HolographicCameraManager.Instance.HolographicCameraIP)
            {
                SpectatorView.HolographicCameraManager.Instance.HolographicCameraIP = SpectatorViewIP;
            }

#if UNITY_EDITOR
            SpectatorView.HolographicCameraManager.Instance.ResetHolographicCamera();
#endif
        }

        public void OnEnable()
        {
            EnableSpectatorView();
        }

        private void CreateSharingStage(Transform parent)
        {
            GameObject sharing = (GameObject)GameObject.Instantiate(Sharing, Vector3.zero, Quaternion.identity);
            sharing.transform.parent = parent;

            SharingStage stage = sharing.GetComponent<SharingStage>();
            if (stage == null)
            {
                stage = sharing.AddComponent<SharingStage>();
            }

            stage.ServerAddress = SharingServiceIP;
            stage.ConnectToServer(SharingServiceIP, stage.ServerPort);
        }

        private void CreateSpectatorViewRig(Transform parent)
        {
            SpectatorView.HolographicCameraManager hcm = HolographicCameraManager.GetComponent<HolographicCameraManager>();
            if (hcm == null)
            {
                hcm = HolographicCameraManager.AddComponent<HolographicCameraManager>();
            }
            hcm.HolographicCameraIP = SpectatorViewIP;
            hcm.LocalComputerIP = LocalComputerIP;

            HolographicCameraManager = (GameObject)GameObject.Instantiate(HolographicCameraManager, Vector3.zero, Quaternion.identity);
            HolographicCameraManager.transform.parent = parent;
        }

        public void EnableSpectatorView()
        {
            // Instantiate Anchor.
            if (!Anchor.activeInHierarchy)
            {
                Anchor = (GameObject)GameObject.Instantiate(Anchor, Vector3.zero, Quaternion.identity);
            }

            Anchor.SetActive(true);

            // Instantiate HolographicCameraManager.
            if (SpectatorView.HolographicCameraManager.Instance == null)
            {
                CreateSpectatorViewRig(null);
            }
            else
            {
                SpectatorView.HolographicCameraManager hcm = HolographicCameraManager.GetComponent<HolographicCameraManager>();
                Transform parent = SpectatorView.HolographicCameraManager.Instance.transform.parent;
                if (hcm == null)
                {
                    Debug.LogWarning("Recreating HolographicCameraManager prefab since HolographicCameraManager script did not exist on original.");

                    GameObject.DestroyImmediate(SpectatorView.HolographicCameraManager.Instance);
                    CreateSpectatorViewRig(parent);
                }
                else
                {
                    if (hcm.HolographicCameraIP != SpectatorViewIP ||
                        hcm.LocalComputerIP != LocalComputerIP)
                    {
                        Debug.LogWarning("Recreating HolographicCameraManager prefab since IP's were incorrect on original.");

                        // IP's are wrong, recreate rig.
                        GameObject.DestroyImmediate(SpectatorView.HolographicCameraManager.Instance);
                        CreateSpectatorViewRig(parent);
                    }
                }
            }

            HolographicCameraManager.SetActive(true);
        }
    }
}
