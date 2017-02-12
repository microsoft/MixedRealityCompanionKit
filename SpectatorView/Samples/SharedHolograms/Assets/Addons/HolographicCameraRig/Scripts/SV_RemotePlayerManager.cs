// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using HoloToolkit.Unity;
using HoloToolkit.Unity.SpatialMapping;
using HoloToolkit.Sharing;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;
using UnityEngine.VR.WSA;

namespace SpectatorView
{
    /// <summary>
    /// Adds and updates the head transforms of remote users.  
    /// Head transforms are sent and received in the local coordinate space of the gameobject
    /// this component is on.  
    /// </summary>
    public class SV_RemotePlayerManager : SpectatorView.SV_Singleton<SV_RemotePlayerManager>
    {
#if !UNITY_EDITOR
        [DllImport("SpatialPerceptionHelper")]
        private static extern bool GetHeadTransform(IntPtr unityCoordinateSystem, int nsPast,
            out float rotX, out float rotY, out float rotZ, out float rotW,
            out float posX, out float posY, out float posZ);
#else
        [DllImport("UnityCompositorInterface")]
        private static extern long GetCurrentUnityTime();

        [DllImport("UnityCompositorInterface")]
        private static extern long GetColorDuration();

        [DllImport("UnityCompositorInterface")]
        private static extern void SetNetworkLatency(long networkLatency);
#endif

        private SpectatorView.SV_CustomMessages customMessages = null;

        public class RemoteHeadInfo
        {
            public long UserID;
            public string IP = string.Empty;
            public GameObject HeadObject;
        }

        /// <summary>
        /// Keep a list of the remote heads, indexed by XTools userID
        /// </summary>
        Dictionary<long, RemoteHeadInfo> remoteHeads = new Dictionary<long, RemoteHeadInfo>();


        public long NetworkTime { get; private set; }
        //TODO: If network traffic is limited, restrict this to a longer snapshop time.
        private float networkTimeSnapshotInterval = 0;
        private float timeSinceLastNetworkTimeSnapshot = 0;

#if !UNITY_EDITOR
        private long colorFrameDurationNS = (long)((1.0f / 30.0f) * 1000000000);
        long prevTimeOffsetNS = 0;
        float colorFrameReceivedTimeS = 0;
#else
        NetworkMovement networkMovement = null;
#endif

        bool registeredSharingStageCallbacks = false;

        void Start()
        {
            customMessages = SpectatorView.SV_CustomMessages.Instance;

            if (customMessages != null)
            {
                RegisterCustomMessages();
            }
        }

        protected override void OnDestroy()
        {
            registeredSharingStageCallbacks = false;

            if (SharingStage.Instance != null)
            {
                SharingStage.Instance.SessionUsersTracker.UserJoined -= SessionUsersTracker_UserJoined;
                SharingStage.Instance.SessionUsersTracker.UserLeft -= SessionUsersTracker_UserLeft;
            }

            base.OnDestroy();
        }

        void RegisterCustomMessages()
        {
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.HeadTransform] = this.UpdateHeadTransform;
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.NetworkRoundTripTime] = OnNetworkRoundTripTime;

#if UNITY_EDITOR
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.SpatialMapping] = OnSpatialMapping;
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.UpdateSpectatorViewIP] = OnUpdateSpectatorViewIP;
#else
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.SetNetworkTime] = OnNetworkTimeSet;
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.SetColorDuration] = OnColorDuration;
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.SpatialMappingRequest] = OnSpatialMappingRequest;
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.TimeOffset] = OnTimeOffset;
            customMessages.MessageHandlers[SpectatorView.SV_CustomMessages.TestMessageID.EditorUser] = OnEditorUser;
#endif
        }

        void OnUpdateSpectatorViewIP(HoloToolkit.Sharing.NetworkInMessage msg)
        {
            msg.ReadInt64();
            string _SpectatorViewIP = SV_CustomMessages.Instance.ReadIP(msg);

            if (SpectatorViewManager.Instance == null || SpectatorView.HolographicCameraManager.Instance == null)
            {
                return;
            }

            if (_SpectatorViewIP != SpectatorViewManager.Instance.SpectatorViewIP)
            {
                Debug.Log("Updating spectator view ip to: " + SpectatorViewManager.Instance.SpectatorViewIP);
                SpectatorViewManager.Instance.SpectatorViewIP = _SpectatorViewIP;
                SpectatorView.HolographicCameraManager.Instance.HolographicCameraIP = SpectatorViewManager.Instance.SpectatorViewIP;
            }
        }

        void OnEditorUser(HoloToolkit.Sharing.NetworkInMessage msg)
        {
            msg.ReadInt64();

            long ipSize = msg.ReadInt64();
            byte[] ipData = new byte[(uint)ipSize];
            msg.ReadArray(ipData, (uint)ipSize);
            string editorIP = System.Text.Encoding.ASCII.GetString(ipData).Trim();

            long editorUserID = msg.ReadInt64();

            if (HolographicCameraManager.Instance.LocalComputerIP.Trim() == String.Empty ||
                HolographicCameraManager.Instance.LocalComputerIP.Trim() == editorIP)
            {
                User user = SharingStage.Instance.SessionUsersTracker.GetUserById((int)editorUserID);
                if (user != null)
                {
                    HolographicCameraManager.Instance.editorUser = user;
                }
            }
        }

        void OnSpatialMappingRequest(HoloToolkit.Sharing.NetworkInMessage msg)
        {
            msg.ReadInt64();

            HolographicCameraManager.Instance.requestSpatialMappingData = true;
        }

        void RequestCurrentNetworkLatency()
        {
#if UNITY_EDITOR
            if (SpectatorView.SV_CustomMessages.Instance != null &&
                SpectatorView.SV_CustomMessages.Instance.Initialized &&
                SharingStage.Instance != null)
            {
                SpectatorView.SV_CustomMessages.Instance.SendNetworkRoundTripTime(GetCurrentUnityTime());
            }
#endif
        }

        void OnTimeOffset(HoloToolkit.Sharing.NetworkInMessage msg)
        {
#if !UNITY_EDITOR
            float eventTime = Time.time;
            msg.ReadInt64();

            int timeOffset = msg.ReadInt32();
            if (timeOffset > 0) { timeOffset *= -1; }

            colorFrameReceivedTimeS = eventTime;
            prevTimeOffsetNS = timeOffset;

            Vector3 pos;
            Quaternion rot;

            if (GetHeadTransform(WorldManager.GetNativeISpatialCoordinateSystemPtr(), 
                (int)timeOffset, 
                out rot.x, out rot.y, out rot.z, out rot.w, 
                out pos.x, out pos.y, out pos.z))
            {
                // Transform the head position and rotation from world space into local space
                Vector3 HeadPos = this.transform.InverseTransformPoint(pos);
                Quaternion HeadRot = Quaternion.Inverse(this.transform.rotation) * rot;
                SpectatorView.SV_CustomMessages.Instance.SendHeadTransform(HeadPos, HeadRot);
            }
#endif
        }

        void OnNetworkRoundTripTime(HoloToolkit.Sharing.NetworkInMessage msg)
        {
            long userID = msg.ReadInt64();

            long time = msg.ReadInt64();

#if !UNITY_EDITOR
            SpectatorView.SV_CustomMessages.Instance.SendNetworkRoundTripTime(time);
#else
            if (HolographicCameraManager.Instance != null &&
                HolographicCameraManager.Instance.tppcUser != null &&
                HolographicCameraManager.Instance.tppcUser.IsValid() &&
                userID == HolographicCameraManager.Instance.tppcUser.GetID())
            {
                NetworkTime = GetCurrentUnityTime() - time;

                SetNetworkLatency(NetworkTime);
                SpectatorView.SV_CustomMessages.Instance.SendTPPCNetworkTime(NetworkTime);
            }
#endif
        }

#if !UNITY_EDITOR
        void OnNetworkTimeSet(HoloToolkit.Sharing.NetworkInMessage msg)
        {
            msg.ReadInt64();

            long time = msg.ReadInt64();
            NetworkTime = time;
        }

        void OnColorDuration(HoloToolkit.Sharing.NetworkInMessage msg)
        {
            msg.ReadInt64();

            long duration = msg.ReadInt64();
            colorFrameDurationNS = duration * 100;
        }
#endif

        void Update()
        {
            if (!registeredSharingStageCallbacks && SharingStage.Instance != null &&
                HolographicCameraManager.Instance != null && SharingStage.Instance.SessionUsersTracker != null)
            {
                registeredSharingStageCallbacks = true;

                SharingStage.Instance.SessionUsersTracker.UserJoined += SessionUsersTracker_UserJoined;
                SharingStage.Instance.SessionUsersTracker.UserLeft += SessionUsersTracker_UserLeft;
            }

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

#if !UNITY_EDITOR
            SendSpatialMappingData();

            Vector3 pos;
            Quaternion rot;

            long timeSinceLastColorFrameNS = (long)((Time.time - colorFrameReceivedTimeS) * 1000000000);

            // If the elapsed time is greater than a color frame duration, decrease the offset time by that number of frames.
            if (timeSinceLastColorFrameNS >= colorFrameDurationNS && colorFrameDurationNS != 0) 
            {
                int numOffsets = (int)(timeSinceLastColorFrameNS / colorFrameDurationNS);

                timeSinceLastColorFrameNS -= (numOffsets * colorFrameDurationNS);
                if (timeSinceLastColorFrameNS < 0) { timeSinceLastColorFrameNS = 0; }
            }

            try
            {
                if (GetHeadTransform(WorldManager.GetNativeISpatialCoordinateSystemPtr(),
                    (int)((-1 * timeSinceLastColorFrameNS) + prevTimeOffsetNS),
                    out rot.x, out rot.y, out rot.z, out rot.w,
                    out pos.x, out pos.y, out pos.z))
                {
                    // Transform the head position and rotation from world space into local space
                    Vector3 HeadPos = this.transform.InverseTransformPoint(pos);
                    Quaternion HeadRot = Quaternion.Inverse(this.transform.rotation) * rot;
                    customMessages.SendHeadTransform(HeadPos, HeadRot);
                }
            }
            catch (Exception ex)
            {
                Debug.LogException(ex);
            }
#else
            timeSinceLastNetworkTimeSnapshot += Time.deltaTime;
            if (timeSinceLastNetworkTimeSnapshot >= networkTimeSnapshotInterval)
            {
                timeSinceLastNetworkTimeSnapshot = 0;

                RequestCurrentNetworkLatency();
                customMessages.SendTPPCColorDuration(GetColorDuration());
            }
#endif
        }

        /// <summary>
        /// Called when a new user is leaving.
        /// </summary>
        private void SessionUsersTracker_UserLeft(User user)
        {
            if (remoteHeads.ContainsKey(user.GetID()))
            {
#if UNITY_EDITOR
                if (HolographicCameraManager.Instance != null &&
                    remoteHeads[user.GetID()].IP != null && 
                    remoteHeads[user.GetID()].IP == HolographicCameraManager.Instance.HolographicCameraIP.Trim() &&
                    HolographicCameraManager.Instance.HolographicCameraIP.Trim() != String.Empty)
                {
                    networkMovement = null;
                    HolographicCameraManager.Instance.ResetHolographicCamera();
                }
#endif

                RemoveRemoteHead(remoteHeads[user.GetID()].HeadObject);
                remoteHeads.Remove(user.GetID());
            }
        }

        /// <summary>
        /// Called when a user is joining.
        /// </summary>
        private void SessionUsersTracker_UserJoined(User user)
        {
            GetRemoteHeadInfo(user.GetID());

#if UNITY_EDITOR
            if (SpectatorView.SceneManager.Instance != null)
            {
                SpectatorView.SceneManager.Instance.SendCurrentScene();
            }

            if ((SpectatorView.HolographicCameraManager.Instance.LocalComputerIP.Trim() == string.Empty) ||
                SpectatorView.HolographicCameraManager.Instance.localIPs.Contains(
                    SpectatorView.HolographicCameraManager.Instance.LocalComputerIP.Trim()))
            {
                customMessages.SendUnityUserID(customMessages.localUserID);
            }
#endif
        }

        /// <summary>
        /// Gets the data structure for the remote users' head position.
        /// </summary>
        /// <param name="userID"></param>
        /// <returns></returns>
        public RemoteHeadInfo GetRemoteHeadInfo(long userID)
        {
            RemoteHeadInfo headInfo;

            // Get the head info if its already in the list, otherwise add it
            if (!this.remoteHeads.TryGetValue(userID, out headInfo))
            {
                headInfo = new RemoteHeadInfo();
                headInfo.UserID = userID;
                headInfo.HeadObject = CreateRemoteHead(userID);
                this.remoteHeads.Add(userID, headInfo);
            }

#if UNITY_EDITOR
            if (headInfo.IP != null && HolographicCameraManager.Instance != null && 
                !HolographicCameraManager.Instance.IsCurrentlyActive && SharingStage.Instance != null && 
                SharingStage.Instance.SessionUsersTracker != null)
            {
                if (headInfo.IP == HolographicCameraManager.Instance.HolographicCameraIP.Trim() &&
                    HolographicCameraManager.Instance.HolographicCameraIP.Trim() != String.Empty)
                {
                    headInfo.HeadObject.name = "SpectatorViewCamera";
                    HolographicCameraManager.Instance.EnableHolographicCamera(headInfo.HeadObject.transform);
                    User user = SharingStage.Instance.SessionUsersTracker.GetUserById((int)userID);
                    if (user != null)
                    {
                        HolographicCameraManager.Instance.tppcUser = user;
                    }
                }
            }
#endif

            return headInfo;
        }

        /// <summary>
        /// Called when a remote user sends a head transform.
        /// </summary>
        /// <param name="msg"></param>
        void UpdateHeadTransform(NetworkInMessage msg)
        {
            // Parse the message
            long userID = msg.ReadInt64();
            long ipSize = msg.ReadInt64();
            byte[] ipData = new byte[(uint)ipSize];
            msg.ReadArray(ipData, (uint)ipSize);

            Vector3 headPos = SpectatorView.SV_CustomMessages.Instance.ReadVector3(msg);
            Quaternion headRot = SpectatorView.SV_CustomMessages.Instance.ReadQuaternion(msg);

            RemoteHeadInfo headInfo = GetRemoteHeadInfo(userID);

            if (headInfo.HeadObject != null)
            {
#if UNITY_EDITOR
                if (SpectatorView.HolographicCameraManager.Instance != null &&
                    SpectatorView.HolographicCameraManager.Instance.tppcUser != null &&
                    SpectatorView.HolographicCameraManager.Instance.tppcUser.GetID() == userID)
                {
                    if (networkMovement == null)
                    {
                        networkMovement = headInfo.HeadObject.AddComponent<NetworkMovement>();
                    }

                    networkMovement.AddTransform(
                        new NetworkMovement.NetworkTransform()
                        {
                            Position = headPos,
                            Rotation = headRot
                        }
                    );
                }
                else
#endif
                {
                    headInfo.HeadObject.transform.localPosition = headPos;
                    headInfo.HeadObject.transform.localRotation = headRot;
                }

                if (string.IsNullOrEmpty(headInfo.IP))
                {
                    headInfo.IP = System.Text.Encoding.ASCII.GetString(ipData);
                }
            }
        }

        /// <summary>
        /// Creates a new game object to represent the user's head.
        /// </summary>
        /// <returns></returns>
        GameObject CreateRemoteHead(long userID)
        {
            GameObject newHeadObj = GameObject.CreatePrimitive(PrimitiveType.Cube);
            newHeadObj.name = userID.ToString();

            newHeadObj.transform.parent = this.gameObject.transform;
            newHeadObj.transform.localScale = Vector3.one;

            newHeadObj.GetComponent<Renderer>().enabled = false;
            newHeadObj.GetComponent<Collider>().enabled = false;

            return newHeadObj;
        }

        /// <summary>
        /// When a user has left the session this will cleanup their
        /// head data.
        /// </summary>
        /// <param name="remoteHeadObject"></param>
        void RemoveRemoteHead(GameObject remoteHeadObject)
        {
            DestroyImmediate(remoteHeadObject);
        }

        private void SendSpatialMappingData()
        {
            if (SpatialMappingManager.Instance == null ||
                HolographicCameraManager.Instance == null ||
                !HolographicCameraManager.Instance.requestSpatialMappingData)
            {
                return;
            }

            HolographicCameraManager.Instance.requestSpatialMappingData = false;

            List<MeshFilter> meshFilters = SpatialMappingManager.Instance.GetMeshFilters();
            foreach (MeshFilter filter in meshFilters)
            {
                // GetMeshFilters ensures that both filter and filter.sharedMesh are not null.
                Mesh mesh = filter.sharedMesh;

                Vector3[] vertices = mesh.vertices;
                for (int v = 0; v < vertices.Length; v++)
                {
                    vertices[v] = filter.transform.TransformPoint(vertices[v]);
                }

                Vector3[] normals = mesh.normals;
                int[] indices = mesh.triangles;

                SpectatorView.SV_CustomMessages.Instance.SendSpatialMapping(vertices, normals, indices);
            }
        }

        void OnSpatialMapping(HoloToolkit.Sharing.NetworkInMessage msg)
        {
#if UNITY_EDITOR
            if (SpatialMappingManager.Instance == null)
            {
                return;
            }

            SpectatorView.RemoteSpatialMappingSource rsms = SpatialMappingManager.Instance.GetComponent<SpectatorView.RemoteSpatialMappingSource>();
            if (rsms == null)
            {
                return;
            }

            msg.ReadInt64();

            List<Vector3> vertices = new List<Vector3>();
            List<Vector3> normals = new List<Vector3>();
            List<int> triangles = new List<int>();

            int vertexCount = msg.ReadInt32();
            int normalCount = msg.ReadInt32();
            int triangleCount = msg.ReadInt32();

            for (int i = 0; i < vertexCount; i++)
            {
                Vector3 vertex = SpectatorView.SV_CustomMessages.Instance.ReadVector3(msg);
                vertices.Add(vertex);
            }

            for (int i = 0; i < normalCount; i++)
            {
                Vector3 normal = SpectatorView.SV_CustomMessages.Instance.ReadVector3(msg);
                normals.Add(normal);
            }

            for (int i = 0; i < triangleCount; i++)
            {
                int index = msg.ReadInt32();
                triangles.Add(index);
            }

            SpatialMappingManager.Instance.transform.parent = transform;
            SpatialMappingManager.Instance.transform.localPosition = Vector3.zero;
            SpatialMappingManager.Instance.transform.localRotation = Quaternion.identity;

            rsms.AddSurface(vertices, normals, triangles);
#endif
        }
    }
}