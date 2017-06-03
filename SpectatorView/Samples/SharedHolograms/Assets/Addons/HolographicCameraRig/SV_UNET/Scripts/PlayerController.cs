// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using UnityEngine.Networking;
using System.Runtime.InteropServices;
using UnityEngine.VR.WSA;
using UnityEngine.VR.WSA.Input;
using System.Collections.Generic;
using System;

namespace SpectatorView
{
    /// <summary>
    /// Controls player behavior (local and remote).
    /// </summary>
    public class PlayerController : NetworkBehaviour
    {
        #region DLLImports
#if !UNITY_EDITOR
        [DllImport("SpatialPerceptionHelper")]
        private static extern bool GetHeadTransform(System.IntPtr unityCoordinateSystem, int nsPast,
            out float rotX, out float rotY, out float rotZ, out float rotW,
            out float posX, out float posY, out float posZ);
#else
        [DllImport("UnityCompositorInterface")]
        private static extern bool SetHologramPose(
            float rotX,  float rotY,  float rotZ,  float rotW,
            float posX,  float posY,  float posZ);
#endif
        #endregion

        public override float GetNetworkSendInterval()
        {
            if (IsSV())
            {
                return 0;
            }

            return 0.033f;
        }


        private static PlayerController _Instance = null;
        public static PlayerController Instance
        {
            get
            {
                return _Instance;
            }
        } 

        public bool CanEstablishAnchor()
        {
#if UNITY_EDITOR
            return false;
#endif

            //TODO: differentiate between immersive devices and HoloLens devices.
            return !PlayerName.ToLower().StartsWith("editor");
        }

        /// <summary>
        /// The transform of the shared world anchor.
        /// </summary>
        private Transform sharedWorldAnchorTransform;

        private UNetAnchorManager anchorManager;

        /// <summary>
        /// The position relative to the shared world anchor.
        /// </summary>
        [SyncVar]
        private Vector3 localPosition;

        /// <summary>
        /// The rotation relative to the shared world anchor.
        /// </summary>
        [SyncVar]
        private Quaternion localRotation;

        // Nanoseconds
        [SyncVar]
        private long colorDuration = 33333333;

        [Command]
        private void CmdSetColorDuration(long value)
        {
            ServerSetColorDuration(value);
        }

        private void ServerSetColorDuration(long value)
        {
            colorDuration = value;
        }

        public void SetColorDuration(long value)
        {
            if (colorDuration != value)
            {
                ServerSetColorDuration(value);
                CmdSetColorDuration(value);
            }
        }

        /// <summary>
        /// Sets the localPosition and localRotation on clients.
        /// </summary>
        /// <param name="postion">the localPosition to set</param>
        /// <param name="rotation">the localRotation to set</param>
        [Command(channel = 1)]
        public void CmdTransform(Vector3 postion, Quaternion rotation)
        {
            localPosition = postion;
            localRotation = rotation;
        }

        [SyncVar(hook = "AnchorEstablishedChanged")]
        bool AnchorEstablished;

        [Command]
        private void CmdSendAnchorEstablished(bool Established)
        {
            AnchorEstablished = Established;
            if (Established && SharesSpatialAnchors && !isLocalPlayer)
            {
                Debug.Log("remote device likes the anchor");
                anchorManager.AnchorFoundRemotely();
            }
        }

        void AnchorEstablishedChanged(bool update)
        {
            Debug.LogFormat("AnchorEstablished for {0} was {1} is now {2}", PlayerName, AnchorEstablished,update);
            AnchorEstablished = update;

            Renderer renderer = GetComponent<MeshRenderer>();
            if (renderer != null)
            {
                renderer.enabled = update;
            }
            renderer = GetComponentInChildren<MeshRenderer>();
            if (renderer != null)
            {
                renderer.enabled = update;
            }

            if (!isLocalPlayer)
            {
                InitializeRemoteAvatar();
                InitializeSpectatorView();
            }
        }

        [SyncVar(hook = "PlayerNameChanged")]
        string PlayerName = string.Empty;

        [Command]
        private void CmdSetPlayerName(string playerName)
        {
            PlayerName = playerName;
        }

        void PlayerNameChanged(string update)
        {
            Debug.LogFormat("Player name changing from {0} to {1}", PlayerName, update);
            PlayerName = update;
#if UNITY_EDITOR
            if (PlayerName.Trim().ToLower().StartsWith("editor"))
            {
                Renderer renderer = gameObject.GetComponent<Renderer>();
                if (renderer != null)
                {
                    gameObject.SetActive(false);
                }
            }
#endif
            if (!isLocalPlayer)
            {
                InitializeRemoteAvatar();
                InitializeSpectatorView();
            }
        }

        [SyncVar(hook = "PlayerIpChanged")]
        string PlayerIp = string.Empty;

        [Command]
        private void CmdSetPlayerIp(string playerIp)
        {
            PlayerIp = playerIp;
        }

        void PlayerIpChanged(string update)
        {
            PlayerIp = update;
        }

#region Spatial Mapping
        [Command]
        public void CmdClearSpatialMappingBytes()
        {
            ServerClearSpatialMappingData();
        }

        private void ServerClearSpatialMappingData()
        {
            SpectatorView.SV_SpatialMappingManager.Instance.smBytes.Clear();
        }

        public void ClearSpatialMappingData()
        {
            ServerClearSpatialMappingData();
            CmdClearSpatialMappingBytes();
        }

        [Command]
        private void CmdSetSpatialMappingNumChunks(int value)
        {
            ServerSetSpatialMappingNumChunks(value);
        }

        private void ServerSetSpatialMappingNumChunks(int value)
        {
            SpectatorView.SV_SpatialMappingManager.Instance.spatialMappingNumChunks = value;
        }

        [Command]
        public void CmdSendSpatialMappingData(int meshIndex, int chunkIndex, int totalMeshBytes, int chunkLength, byte[] chunkBytes)
        {
            ServerSendSpatialMappingData(meshIndex, chunkIndex, totalMeshBytes, chunkLength, chunkBytes);
        }

        private void ServerSendSpatialMappingData(int meshIndex, int chunkIndex, int totalMeshBytes, int chunkLength, byte[] chunkBytes)
        {
            SpectatorView.SV_SpatialMappingManager.Instance.smBytes.Add(
                new SpectatorView.SV_SpatialMappingManager.SpatialMappingChunk(meshIndex, chunkIndex, totalMeshBytes, chunkLength, chunkBytes));
        }

        public void SendSpatialMappingData(int meshIndex, int chunkIndex, int totalMeshBytes, int chunkLength, byte[] chunkBytes)
        {
            if (isServer)
            {
                ServerSendSpatialMappingData(meshIndex, chunkIndex, totalMeshBytes, chunkLength, chunkBytes);
            }
            else
            {
                CmdSendSpatialMappingData(meshIndex, chunkIndex, totalMeshBytes, chunkLength, chunkBytes);
            }
        }

        public void SetSpatialMappingDataCompleted(int numChunks)
        {
            if (isServer)
            {
                ServerSetSpatialMappingNumChunks(numChunks);
            }
            else
            {
                CmdSetSpatialMappingNumChunks(numChunks);
            }
        }

        public void RequestSpatialMapping()
        {
            SpectatorView.SV_SpatialMappingManager.Instance.requestSpatialMappingFlag = true;
            CmdSetRequestSpatialMappingFlag();
        }

        [Command]
        public void CmdSetRequestSpatialMappingFlag()
        {
            SpectatorView.SV_SpatialMappingManager.Instance.requestSpatialMappingFlag = true;
        }

        [Command]
        public void CmdClearRequestSpatialMappingFlag()
        {
            SpectatorView.SV_SpatialMappingManager.Instance.requestSpatialMappingFlag = false;
        }
#endregion

        [SyncVar]
        string HostIP;

        [Command]
        public void CmdSetAnchorName(string anchorName)
        {
            UNetAnchorManager.Instance.AnchorName = anchorName;
        }

        [Command]
        public void CmdSetAnchorOwnerIP(string anchorOwnerIP)
        {
            if (UNetAnchorManager.Instance.AnchorOwnerIP == string.Empty)
            {
                Debug.Log("Setting anchor owner to: " + anchorOwnerIP);
                UNetAnchorManager.Instance.AnchorOwnerIP = anchorOwnerIP;
            }
            else
            {
                Debug.LogWarning("Attempted to set anchor owner to " + anchorOwnerIP + ", but anchor was already established.");
            }
        }

        [Command]
        public void CmdFindNewAnchorOwner()
        {
            FindNewAnchorOwner();
        }

        public void FindNewAnchorOwner()
        {
            UNetAnchorManager.Instance.AnchorOwnerIP = string.Empty;
            RpcIdentifyPotentialAnchorOwner();
        }

        [ClientRpc]
        public void RpcIdentifyPotentialAnchorOwner()
        {
            if (UNetAnchorManager.Instance.AnchorOwnerIP == string.Empty &&
                CanEstablishAnchor() && 
                !string.IsNullOrEmpty(PlayerIp))
            {
                CmdSetAnchorOwnerIP(PlayerIp);
            }
        }

        private bool _createAnchor = false;

        public void CreateAnchor(string value)
        {
            if (isLocalPlayer &&
                (value == networkDiscovery.LocalIp ||
                value.Trim().ToLower() == PlayerName.ToString().ToLower()) &&
                CanEstablishAnchor())
            {
                Debug.Log("Taking ownership of creating anchor.");
                _createAnchor = true;
            }
        }

        [SyncVar(hook = "SharesAnchorsChanged")]
        public bool SharesSpatialAnchors;

        [Command]
        private void CmdSetCanShareAnchors(bool canShareAnchors)
        {
            Debug.Log("CMDSetCanShare " + canShareAnchors);
            SharesSpatialAnchors = canShareAnchors;
        }

        void SharesAnchorsChanged(bool update)
        {
            SharesSpatialAnchors = update;
            Debug.LogFormat("{0} {1} share", PlayerName, SharesSpatialAnchors ? "does" : "does not");
        }

        NetworkDiscoveryWithAnchors networkDiscovery;

        private void InitializeRemoteAvatar()
        {
            if (!string.IsNullOrEmpty(PlayerName) && AnchorEstablished)
            {
                //TODO: do any application initialization logic for your remote player here.
                if (PlayerName.Trim().ToLower().StartsWith("editor"))
                {
                    Renderer renderer = gameObject.GetComponent<Renderer>();
                    if (renderer != false)
                    {
                        renderer.enabled = false;
                    }
                }
            }
        }

        private void InitializeSpectatorView()
        {
#if UNITY_EDITOR
            // Spectator view
            if (SpectatorView.SpectatorViewManager.Instance != null &&
                !SpectatorView.SpectatorViewManager.Instance.IsCurrentlyActive &&
                IsSV())
            {
                SpectatorView.SpectatorViewManager.Instance.EnableHolographicCamera(UNetAnchorManager.Instance.gameObject.transform);
            }
#endif
        }

        void Awake()
        {
            networkDiscovery = NetworkDiscoveryWithAnchors.Instance;
            anchorManager = UNetAnchorManager.Instance;
        }

        private void Start()
        {
            if (UNetAnchorManager.Instance == null)
            {
                Debug.LogError("This script requires a UNetAnchorManager in the scene");
                Destroy(this);
                return;
            }

            if (isLocalPlayer)
            {
                Debug.Log("Init from start");
                InitializeLocalPlayer();
            }
            else
            {
                Debug.Log("remote player, analyzing start state "+PlayerName);
                AnchorEstablishedChanged(AnchorEstablished);
                SharesAnchorsChanged(SharesSpatialAnchors);
            }

            if (isServer)
            {
                HostIP = networkDiscovery.LocalIp;
            }

            sharedWorldAnchorTransform = UNetAnchorManager.Instance.gameObject.transform;
            transform.SetParent(sharedWorldAnchorTransform);
        }

        private void InitializeLocalPlayer()
        {
            if (isLocalPlayer)
            {
                Debug.Log("Setting instance for local player ");
                _Instance = this;

                string name = NetworkDiscoveryWithAnchors.Instance.GetNameFromBroadcastData(networkDiscovery.broadcastData);

                Debug.LogFormat("Set local player name {0} ip {1}", name, networkDiscovery.LocalIp);
                PlayerName = name;
                PlayerIp = networkDiscovery.LocalIp;

                CmdSetPlayerName(name);
                CmdSetPlayerIp(networkDiscovery.LocalIp);
                bool canShareAnchors = CanEstablishAnchor();
                Debug.LogFormat("local player {0} share anchors ", (canShareAnchors ? "does" : "does not"));
                CmdSetCanShareAnchors(canShareAnchors);
                
                if (UNetAnchorManager.Instance.AnchorOwnerIP == string.Empty &&
                    CanEstablishAnchor())
                {
                    CmdSetAnchorOwnerIP(PlayerIp);
                }

                if (UNetAnchorManager.Instance.AnchorOwnerIP != string.Empty)
                {
                    GenericNetworkTransmitter.Instance.SetServerIP(UNetAnchorManager.Instance.AnchorOwnerIP);
                }
            }
        }

        public override void OnStartClient()
        {
            base.OnStartClient();
        }

        private void OnDestroy()
        {
#if UNITY_EDITOR
            if (!isLocalPlayer && IsSV() &&
                SpectatorView.SpectatorViewManager.Instance != null &&
                SpectatorView.SpectatorViewManager.Instance.IsCurrentlyActive)
            {
                SpectatorView.SpectatorViewManager.Instance.ResetHolographicCamera();
            }
#endif

            // Anchor owner is disconnecting, find a new anchor.
            if (UNetAnchorManager.Instance.AnchorOwnerIP == PlayerIp)
            {
                Debug.Log("Find new anchor owner.");
                UNetAnchorManager.Instance.FindAnchorOwner();
            }

            if (HostIP == PlayerIp || HostIP == PlayerName)
            {
                //TODO: auto-rejoin
                Debug.Log("Host has disconnected.");
                networkDiscovery.Restart();
                ScrollingSessionListUIController.Instance.Show();
                SpectatorView.SpectatorViewManager.Instance.ResetAutoconnect();
            }
        }

        private bool? isSVRemote = null;
        private bool? isSVLocal = null;

        public bool IsSV()
        {
            return IsSpectatorViewLocal() || IsSpectatorViewRemote();
        }

        private bool IsSpectatorViewLocal()
        {
#if UNITY_EDITOR
            return false;
#endif

            if (!isLocalPlayer)
            {
                return false;
            }

            if (isSVLocal.HasValue)
            {
                return isSVLocal.Value;
            }

            if (SpectatorView.SpectatorViewManager.Instance == null)
            {
                return false;
            }

            if (string.IsNullOrEmpty(PlayerIp) || string.IsNullOrEmpty(PlayerName))
            {
                return false;
            }

            isSVLocal =
                networkDiscovery.LocalIp.Trim() == SpectatorView.SpectatorViewManager.Instance.SpectatorViewID.Trim() ||
                PlayerIp.ToLower().Trim() == SpectatorView.SpectatorViewManager.Instance.SpectatorViewID.ToLower().Trim() ||
                PlayerName.ToLower().Trim() == SpectatorView.SpectatorViewManager.Instance.SpectatorViewID.ToLower().Trim();

            return isSVLocal.Value;
        }

        private bool IsSpectatorViewRemote()
        {
            if (isLocalPlayer)
            {
                return false;
            }

            if (isSVRemote.HasValue)
            {
                return isSVRemote.Value;
            }

            if (SpectatorView.SpectatorViewManager.Instance == null)
            {
                return false;
            }

            if (string.IsNullOrEmpty(PlayerIp) || string.IsNullOrEmpty(PlayerName))
            {
                return false;
            }

            isSVRemote =
                networkDiscovery.LocalIp.Trim() == SpectatorView.SpectatorViewManager.Instance.SpectatorViewID.Trim() ||
                PlayerIp.ToLower().Trim() == SpectatorView.SpectatorViewManager.Instance.SpectatorViewID.ToLower().Trim() ||
                PlayerName.ToLower().Trim() == SpectatorView.SpectatorViewManager.Instance.SpectatorViewID.ToLower().Trim();

            return isSVRemote.Value;
        }

        private void RetryFindingAnchorOwner()
        {
            if (UNetAnchorManager.Instance.AnchorOwnerIP == string.Empty)
            {
                FindNewAnchorOwner();
            }
        }

        private void Update()
        {
            if (_createAnchor &&
                SV_WorldAnchorManager.Instance != null &&
                SV_WorldAnchorManager.Instance.AnchorStore != null)
            {
                Debug.Log("Creating Anchor.");
                _createAnchor = false;
                UNetAnchorManager.Instance.CreateAnchor();
            }

            if (isServer && isLocalPlayer)
            {
                if (UNetAnchorManager.Instance.AnchorOwnerIP == string.Empty)
                {
                    Invoke("RetryFindingAnchorOwner", 5.0f);
                }
            }

#region Spatial Mapping
            if (isLocalPlayer && IsSpectatorViewLocal() &&
                SpectatorView.SV_SpatialMappingManager.Instance != null &&
                SpectatorView.SV_SpatialMappingManager.Instance.requestSpatialMappingFlag)
            {
                SpectatorView.SV_SpatialMappingManager.Instance.requestSpatialMappingFlag = false;
                CmdClearRequestSpatialMappingFlag();
                ServerClearSpatialMappingData();
                CmdClearSpatialMappingBytes();

                SpectatorView.SV_SpatialMappingManager.Instance.GetSpatialMappingData();
            }
#endregion

            // If we aren't the local player, we just need to make sure that the position of this object is set properly
            // so that we properly render their avatar in our world.
            if (!isLocalPlayer && string.IsNullOrEmpty(PlayerName) == false)
            {
                transform.localPosition = Vector3.Lerp(transform.localPosition, localPosition, 0.3f);
                transform.localRotation = localRotation;

#if UNITY_EDITOR
                if (IsSV())
                {
                    SetHologramPose(localRotation.x, localRotation.y, localRotation.z, localRotation.w,
                        localPosition.x, localPosition.y, localPosition.z);
                }
#endif
                return;
            }

            if (!isLocalPlayer)
            {
                return;
            }

            if (AnchorEstablished != anchorManager.AnchorEstablished)
            {
                CmdSendAnchorEstablished(anchorManager.AnchorEstablished);
            }

            if (AnchorEstablished == false)
            {
                return;
            }

#if !UNITY_EDITOR
            Vector3 pos = Vector3.zero;
            Quaternion rot = Quaternion.identity;

            if (IsSV())
            {
                if (GetHeadTransform(WorldManager.GetNativeISpatialCoordinateSystemPtr(),
                    -1 * (int)colorDuration,
                    out rot.x, out rot.y, out rot.z, out rot.w,
                    out pos.x, out pos.y, out pos.z))
                {
                    transform.position = pos;
                    transform.rotation = rot;
                }
            }
            else
#endif
            {
                // if we are the remote player then we need to update our worldPosition and then set our 
                // local (to the shared world anchor) position for other clients to update our position in their world.
                transform.position = Camera.main.transform.position;
                transform.rotation = Camera.main.transform.rotation;
            }

            // For UNET we use a command to signal the host to update our local position
            // and rotation
            CmdTransform(transform.localPosition, transform.localRotation);
        }

        /// <summary>
        /// Called when the local player starts.  In general the side effect should not be noticed
        /// as the players' avatar is always rendered on top of their head.
        /// </summary>
        public override void OnStartLocalPlayer()
        {
            Renderer renderer = GetComponent<Renderer>();
            if (renderer != null)
            {
                renderer.enabled = false;
            }

            renderer = GetComponentInChildren<MeshRenderer>();
            if (renderer != null)
            {
                renderer.enabled = false;
            }
        }

        [Command]
        private void CmdSendSharedTransform(GameObject target, Vector3 pos, Quaternion rot)
        {
            UNetSharedHologram ush = target.GetComponent<UNetSharedHologram>();
            ush.CmdTransform(pos, rot);
        }

        /// <summary>
        /// For sending transforms for holograms which do not frequently change.
        /// </summary>
        /// <param name="target">The shared hologram (must have a </param>
        /// <param name="pos"></param>
        /// <param name="rot"></param>
        public void SendSharedTransform(GameObject target, Vector3 pos, Quaternion rot)
        {
            if (isLocalPlayer)
            {
                CmdSendSharedTransform(target, pos, rot);
            }
        }
    }
}
