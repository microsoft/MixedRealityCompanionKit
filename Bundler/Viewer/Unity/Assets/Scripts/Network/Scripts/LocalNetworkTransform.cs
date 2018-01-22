// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#if ENABLE_UNET
using System;
using UnityEngine;

namespace UnityEngine.Networking
{
    [DisallowMultipleComponent]
    [AddComponentMenu("Network/LocalNetworkTransform")]
    public class LocalNetworkTransform : NetworkBehaviour
    {
        public enum TransformSyncMode
        {
            SyncNone = 0,
            SyncTransform = 1,
            SyncCharacterController = 4
        }

        public enum AxisSyncMode
        {
            None,
            AxisX,
            AxisY,
            AxisZ,
            AxisXY,
            AxisXZ,
            AxisYZ,
            AxisXYZ
        }

        public enum CompressionSyncMode
        {
            None,
            Low,
            High
        }

        public delegate bool ClientMoveCallback3D(ref Vector3 position, ref Vector3 velocity, ref Quaternion rotation);
        public delegate bool ClientMoveCallback2D(ref Vector2 position, ref Vector2 velocity, ref float rotation);

        [SerializeField]
        TransformSyncMode m_TransformSyncMode = TransformSyncMode.SyncNone;
        [SerializeField]
        float m_SendInterval = 0.1f;
        [SerializeField]
        AxisSyncMode m_SyncRotationAxis = AxisSyncMode.AxisXYZ;
        [SerializeField]
        CompressionSyncMode m_RotationSyncCompression = CompressionSyncMode.None;
        [SerializeField]
        bool m_SyncSpin;
        [SerializeField]
        float m_MovementTheshold = 0.001f;

        [SerializeField]
        float m_SnapThreshold = 5.0f;
        [SerializeField]
        float m_InterpolateRotation = 1.0f;
        [SerializeField]
        float m_InterpolateMovement = 1.0f;
        [SerializeField]
        ClientMoveCallback3D m_ClientMoveCallback3D;
        [SerializeField]
        ClientMoveCallback2D m_ClientMoveCallback2D;

        CharacterController m_CharacterController;
        bool m_Grounded = true;

        // movement smoothing
        Vector3 m_TargetSyncPosition;
        Vector3 m_TargetSyncVelocity;

        Vector3 m_FixedPosDiff;

        Quaternion m_TargetSyncRotation3D;
        Vector3 m_TargetSyncAngularVelocity3D;

        float m_TargetSyncRotation2D;
        float m_TargetSyncAngularVelocity2D;

        float m_LastClientSyncTime; // last time client received a sync from server
        float m_LastClientSendTime; // last time client send a sync to server

        Vector3 m_PrevPosition;
        Quaternion m_PrevRotation;
        float m_PrevRotation2D;
        float m_PrevVelocity;

        const float k_LocalMovementThreshold = 0.00001f;
        const float k_LocalRotationThreshold = 0.00001f;
        const float k_LocalVelocityThreshold = 0.00001f;
        const float k_MoveAheadRatio = 0.1f;

        NetworkWriter m_LocalTransformWriter;

        // settings

        public TransformSyncMode transformSyncMode { get { return m_TransformSyncMode; } set { m_TransformSyncMode = value; } }
        public float sendInterval { get { return m_SendInterval; } set { m_SendInterval = value; } }
        public AxisSyncMode syncRotationAxis { get { return m_SyncRotationAxis; } set { m_SyncRotationAxis = value; } }
        public CompressionSyncMode rotationSyncCompression { get { return m_RotationSyncCompression; } set { m_RotationSyncCompression = value; } }
        public bool syncSpin { get { return m_SyncSpin; } set { m_SyncSpin = value; } }
        public float movementTheshold { get { return m_MovementTheshold; } set { m_MovementTheshold = value; } }
        public float snapThreshold { get { return m_SnapThreshold; } set { m_SnapThreshold = value; } }
        public float interpolateRotation { get { return m_InterpolateRotation; } set { m_InterpolateRotation = value; } }
        public float interpolateMovement { get { return m_InterpolateMovement; } set { m_InterpolateMovement = value; } }
        public ClientMoveCallback3D clientMoveCallback3D { get { return m_ClientMoveCallback3D; } set { m_ClientMoveCallback3D = value; } }
        public ClientMoveCallback2D clientMoveCallback2D { get { return m_ClientMoveCallback2D; } set { m_ClientMoveCallback2D = value; } }

        // runtime data

        public CharacterController characterContoller { get { return m_CharacterController; } }

        public float lastSyncTime { get { return m_LastClientSyncTime; } }
        public Vector3 targetSyncPosition { get { return m_TargetSyncPosition; } }
        public Vector3 targetSyncVelocity { get { return m_TargetSyncVelocity; } }
        public Quaternion targetSyncRotation3D { get { return m_TargetSyncRotation3D; } }
        public float targetSyncRotation2D { get { return m_TargetSyncRotation2D; } }
        public bool grounded { get { return m_Grounded; } set { m_Grounded = value; } }

        void OnValidate()
        {
            if (m_TransformSyncMode < TransformSyncMode.SyncNone || m_TransformSyncMode > TransformSyncMode.SyncCharacterController)
            {
                m_TransformSyncMode = TransformSyncMode.SyncTransform;
            }

            if (m_SendInterval < 0)
            {
                m_SendInterval = 0;
            }

            if (m_SyncRotationAxis < AxisSyncMode.None || m_SyncRotationAxis > AxisSyncMode.AxisXYZ)
            {
                m_SyncRotationAxis = AxisSyncMode.None;
            }

            if (m_MovementTheshold < 0)
            {
                m_MovementTheshold = 0.00f;
            }

            if (m_SnapThreshold < 0)
            {
                m_SnapThreshold = 0.01f;
            }

            if (m_InterpolateRotation < 0)
            {
                m_InterpolateRotation = 0.01f;
            }

            if (m_InterpolateMovement < 0)
            {
                m_InterpolateMovement = 0.01f;
            }
        }

        void Start()
        {
            m_CharacterController = GetComponent<CharacterController>();
            m_PrevPosition = transform.localPosition;
            m_PrevRotation = transform.localRotation;
            m_PrevVelocity = 0;

            // cache these to avoid per-frame allocations.
            if (localPlayerAuthority)
            {
                m_LocalTransformWriter = new NetworkWriter();
            }
        }

        public override void OnStartServer()
        {
            m_LastClientSyncTime = 0;
        }

        public override bool OnSerialize(NetworkWriter writer, bool initialState)
        {
            if (initialState)
            {
                // always write initial state, no dirty bits
            }
            else if (syncVarDirtyBits == 0)
            {
                writer.WritePackedUInt32(0);
                return false;
            }
            else
            {
                // dirty bits
                writer.WritePackedUInt32(1);
            }

            switch (transformSyncMode)
            {
                case TransformSyncMode.SyncNone:
                    {
                        return false;
                    }
                case TransformSyncMode.SyncTransform:
                    {
                        SerializeModeTransform(writer);
                        break;
                    }
                case TransformSyncMode.SyncCharacterController:
                    {
                        SerializeModeCharacterController(writer);
                        break;
                    }
            }
            return true;
        }

        void SerializeModeTransform(NetworkWriter writer)
        {
            // position
            writer.Write(transform.localPosition);

            // no velocity

            // rotation
            if (m_SyncRotationAxis != AxisSyncMode.None)
            {
                SerializeRotation3D(writer, transform.localRotation, syncRotationAxis, rotationSyncCompression);
            }

            // no spin

            m_PrevPosition = transform.localPosition;
            m_PrevRotation = transform.localRotation;
            m_PrevVelocity = 0;
        }

        void SerializeModeCharacterController(NetworkWriter writer)
        {
            if (isServer && m_LastClientSyncTime != 0)
            {
                // target position
                writer.Write(m_TargetSyncPosition);

                // no velocity

                if (syncRotationAxis != AxisSyncMode.None)
                {
                    // target rotation
                    SerializeRotation3D(writer, m_TargetSyncRotation3D, syncRotationAxis, rotationSyncCompression);
                }
            }
            else
            {
                // current position
                writer.Write(transform.localPosition);

                // no velocity

                if (syncRotationAxis != AxisSyncMode.None)
                {
                    // current rotation
                    SerializeRotation3D(writer, transform.localRotation, syncRotationAxis, rotationSyncCompression);
                }
            }

            // no spin
            m_PrevPosition = transform.localPosition;
            m_PrevRotation = transform.localRotation;
            m_PrevVelocity = 0;
        }

        public override void OnDeserialize(NetworkReader reader, bool initialState)
        {
            if (isServer && NetworkServer.localClientActive)
                return;

            if (!initialState)
            {
                if (reader.ReadPackedUInt32() == 0)
                {
                    return;
                }
            }

            switch (transformSyncMode)
            {
                case TransformSyncMode.SyncNone:
                    {
                        return;
                    }
                case TransformSyncMode.SyncTransform:
                    {
                        UnserializeModeTransform(reader, initialState);
                        break;
                    }
                case TransformSyncMode.SyncCharacterController:
                    {
                        UnserializeModeCharacterController(reader, initialState);
                        break;
                    }
            }
            m_LastClientSyncTime = Time.time;
        }

        void UnserializeModeTransform(NetworkReader reader, bool initialState)
        {
            if (hasAuthority)
            {
                // this component must read the data that the server wrote, even if it ignores it.
                // otherwise the NetworkReader stream will still contain that data for the next component.

                // position
                reader.ReadVector3();

                if (syncRotationAxis != AxisSyncMode.None)
                {
                    UnserializeRotation3D(reader, syncRotationAxis, rotationSyncCompression);
                }
                return;
            }

            if (isServer && m_ClientMoveCallback3D != null)
            {
                var pos = reader.ReadVector3();
                var vel = Vector3.zero;
                var rot = Quaternion.identity;
                if (syncRotationAxis != AxisSyncMode.None)
                {
                    rot = UnserializeRotation3D(reader, syncRotationAxis, rotationSyncCompression);
                }

                if (m_ClientMoveCallback3D(ref pos, ref vel, ref rot))
                {
                    transform.localPosition = pos;
                    if (syncRotationAxis != AxisSyncMode.None)
                    {
                        transform.localRotation = rot;
                    }
                }
                else
                {
                    // rejected by callback
                    return;
                }
            }
            else
            {
                // position
                transform.localPosition = reader.ReadVector3();

                // rotation
                if (syncRotationAxis != AxisSyncMode.None)
                {
                    transform.localRotation = UnserializeRotation3D(reader, syncRotationAxis, rotationSyncCompression);
                }
            }
        }

        void UnserializeModeCharacterController(NetworkReader reader, bool initialState)
        {
            if (hasAuthority)
            {
                // this component must read the data that the server wrote, even if it ignores it.
                // otherwise the NetworkReader stream will still contain that data for the next component.

                // position
                reader.ReadVector3();

                if (syncRotationAxis != AxisSyncMode.None)
                {
                    UnserializeRotation3D(reader, syncRotationAxis, rotationSyncCompression);
                }
                return;
            }

            if (isServer && m_ClientMoveCallback3D != null)
            {
                var pos = reader.ReadVector3();
                Quaternion rot = Quaternion.identity;
                if (syncRotationAxis != AxisSyncMode.None)
                {
                    rot = UnserializeRotation3D(reader, syncRotationAxis, rotationSyncCompression);
                }

                if (m_CharacterController == null)
                {
                    return;
                }

                // no velocity in packet, use current local velocity
                var vel = m_CharacterController.velocity;

                if (m_ClientMoveCallback3D(ref pos, ref vel, ref rot))
                {
                    m_TargetSyncPosition = pos;
                    m_TargetSyncVelocity = vel;
                    if (syncRotationAxis != AxisSyncMode.None)
                    {
                        m_TargetSyncRotation3D = rot;
                    }
                }
                else
                {
                    // rejected by callback
                    return;
                }
            }
            else
            {
                // position
                m_TargetSyncPosition = reader.ReadVector3();

                // rotation
                if (syncRotationAxis != AxisSyncMode.None)
                {
                    m_TargetSyncRotation3D = UnserializeRotation3D(reader, syncRotationAxis, rotationSyncCompression);
                }
            }

            if (m_CharacterController == null)
                return;

            // total distance away the target position is
            var totalDistToTarget = (m_TargetSyncPosition - transform.localPosition); // 5 units
            var perSecondDist = totalDistToTarget / GetNetworkSendInterval();
            m_FixedPosDiff = perSecondDist * Time.fixedDeltaTime;

            if (isServer && !isClient)
            {
                // dedicated server needs to apply immediately, there is no interpolation
                transform.localPosition = m_TargetSyncPosition;
                transform.localRotation = m_TargetSyncRotation3D;
                return;
            }

            // handle zero send rate
            if (GetNetworkSendInterval() == 0)
            {
                transform.localPosition = m_TargetSyncPosition;
                //m_RigidBody3D.velocity = m_TargetSyncVelocity;
                if (syncRotationAxis != AxisSyncMode.None)
                {
                    transform.localRotation = m_TargetSyncRotation3D;
                }
                return;
            }

            // handle position snap threshold
            float dist = (transform.localPosition - m_TargetSyncPosition).magnitude;
            if (dist > snapThreshold)
            {
                transform.localPosition = m_TargetSyncPosition;
            }

            // handle no rotation interpolation
            if (interpolateRotation == 0 && syncRotationAxis != AxisSyncMode.None)
            {
                transform.localRotation = m_TargetSyncRotation3D;
            }

            // handle no movement interpolation
            if (m_InterpolateMovement == 0)
            {
                transform.localPosition = m_TargetSyncPosition;
            }

            if (initialState && syncRotationAxis != AxisSyncMode.None)
            {
                transform.localRotation = m_TargetSyncRotation3D;
            }
        }

        void FixedUpdate()
        {
            if (isServer)
            {
                FixedUpdateServer();
            }
            if (isClient)
            {
                FixedUpdateClient();
            }
        }

        void FixedUpdateServer()
        {
            if ((syncVarDirtyBits != 0) || !NetworkServer.active || !isServer)
            {
                return;
            }

            // dont' auto-dirty if no send interval
            if (GetNetworkSendInterval() == 0)
            {
                return;
            }

            float distance = (transform.localPosition - m_PrevPosition).magnitude;
            if (distance < movementTheshold)
            {
                distance = Quaternion.Angle(m_PrevRotation, transform.localRotation);
                if (distance < movementTheshold)
                {
                    return;
                }
            }

            // This will cause transform to be sent
            SetDirtyBit(1);
        }

        void FixedUpdateClient()
        {
            // dont run if we haven't received any sync data
            if (m_LastClientSyncTime == 0)
            {
                return;
            }

            // dont run if network isn't active or we haven't been spawned yet
            if ((!NetworkServer.active && !NetworkClient.active) || (!isServer && !isClient))
            {
                return;
            }

            // dont run if not expecting continuous updates
            if (GetNetworkSendInterval() == 0)
            {
                return;
            }

            // dont run this if this client has authority over this player object
            if (hasAuthority)
            {
                return;
            }

            // interpolate on client
            switch (transformSyncMode)
            {
                case TransformSyncMode.SyncNone:
                    {
                        return;
                    }
                case TransformSyncMode.SyncTransform:
                    {
                        return;
                    }

                case TransformSyncMode.SyncCharacterController:
                    {
                        InterpolateTransformModeCharacterController();
                        break;
                    }
            }
        }

        void InterpolateTransformModeCharacterController()
        {
            if (m_FixedPosDiff == Vector3.zero && m_TargetSyncRotation3D == transform.localRotation)
                return;

            if (m_InterpolateMovement != 0)
            {
                m_CharacterController.Move(m_FixedPosDiff * m_InterpolateMovement);
            }

            if (interpolateRotation != 0)
            {
                transform.localRotation = Quaternion.Slerp(
                        transform.localRotation,
                        m_TargetSyncRotation3D,
                        Time.fixedDeltaTime * interpolateRotation * 10);
            }
            if (Time.time - m_LastClientSyncTime > GetNetworkSendInterval())
            {
                // turn off interpolation if we go out of the time window for a new packet
                m_FixedPosDiff = Vector3.zero;

                var diff = m_TargetSyncPosition - transform.localPosition;
                m_CharacterController.Move(diff);
            }
        }

        //local transform sync
        void Update()
        {
            if (!hasAuthority || !localPlayerAuthority || NetworkServer.active)
            {
                return;
            }

            if (Time.time - m_LastClientSendTime > GetNetworkSendInterval())
            {
                SendTransform();
                m_LastClientSendTime = Time.time;
            }
        }

        bool HasMoved()
        {
            float diff = (transform.localPosition - m_PrevPosition).magnitude;

            if (diff > k_LocalMovementThreshold)
            {
                return true;
            }

            // check if rotation has changed
            diff = Quaternion.Angle(transform.localRotation, m_PrevRotation);

            return (diff > k_LocalRotationThreshold);
        }

        [Client]
        void SendTransform()
        {
            if (!HasMoved() || ClientScene.readyConnection == null)
            {
                return;
            }

            m_LocalTransformWriter.StartMessage(99);// MsgType.LocalPlayerTransform);
            m_LocalTransformWriter.Write(netId);

            switch (transformSyncMode)
            {
                case TransformSyncMode.SyncNone:
                    {
                        return;
                    }
                case TransformSyncMode.SyncTransform:
                    {
                        SerializeModeTransform(m_LocalTransformWriter);
                        break;
                    }

                case TransformSyncMode.SyncCharacterController:
                    {
                        SerializeModeCharacterController(m_LocalTransformWriter);
                        break;
                    }
            }

            m_PrevPosition = transform.localPosition;
            m_PrevRotation = transform.localRotation;

            m_LocalTransformWriter.FinishMessage();

            ClientScene.readyConnection.SendWriter(m_LocalTransformWriter, GetNetworkChannel());
        }

        static public void HandleTransform(NetworkMessage netMsg)
        {
            NetworkInstanceId netId = netMsg.reader.ReadNetworkId();

            GameObject foundObj = NetworkServer.FindLocalObject(netId);
            if (foundObj == null)
            {
                if (LogFilter.logError) { Debug.LogError("HandleTransform no gameObject"); }
                return;
            }
            LocalNetworkTransform foundSync = foundObj.GetComponent<LocalNetworkTransform>();
            if (foundSync == null)
            {
                if (LogFilter.logError) { Debug.LogError("HandleTransform null target"); }
                return;
            }
            if (!foundSync.localPlayerAuthority)
            {
                if (LogFilter.logError) { Debug.LogError("HandleTransform no localPlayerAuthority"); }
                return;
            }
            if (netMsg.conn.clientOwnedObjects == null)
            {
                if (LogFilter.logError) { Debug.LogError("HandleTransform object not owned by connection"); }
                return;
            }

            if (netMsg.conn.clientOwnedObjects.Contains(netId))
            {
                switch (foundSync.transformSyncMode)
                {
                    case TransformSyncMode.SyncNone:
                        {
                            return;
                        }
                    case TransformSyncMode.SyncTransform:
                        {
                            foundSync.UnserializeModeTransform(netMsg.reader, false);
                            break;
                        }
                    
                    case TransformSyncMode.SyncCharacterController:
                        {
                            foundSync.UnserializeModeCharacterController(netMsg.reader, false);
                            break;
                        }
                }
                foundSync.m_LastClientSyncTime = Time.time;
                return;
            }

            if (LogFilter.logWarn)
            {
                Debug.LogWarning("HandleTransform netId:" + netId + " is not for a valid player");
            }
        }

        //Compression Helper functions
        static void WriteAngle(NetworkWriter writer, float angle, CompressionSyncMode compression)
        {
            switch (compression)
            {
                case CompressionSyncMode.None:
                    {
                        writer.Write(angle);
                        break;
                    }
                case CompressionSyncMode.Low:
                    {
                        writer.Write((short)angle);
                        break;
                    }
                case CompressionSyncMode.High:
                    {
                        writer.Write((short)angle);
                        break;
                    }
            }
        }

        static float ReadAngle(NetworkReader reader, CompressionSyncMode compression)
        {
            switch (compression)
            {
                case CompressionSyncMode.None:
                    {
                        return reader.ReadSingle();
                    }
                case CompressionSyncMode.Low:
                    {
                        return reader.ReadInt16();
                    }
                case CompressionSyncMode.High:
                    {
                        return reader.ReadInt16();
                    }
            }
            return 0;
        }

        //Serialization Helper functions        
        static public void SerializeRotation3D(NetworkWriter writer, Quaternion rot, AxisSyncMode mode, CompressionSyncMode compression)
        {
            switch (mode)
            {
                case AxisSyncMode.None:
                    break;

                case AxisSyncMode.AxisX:
                    WriteAngle(writer, rot.eulerAngles.x, compression);
                    break;

                case AxisSyncMode.AxisY:
                    WriteAngle(writer, rot.eulerAngles.y, compression);
                    break;

                case AxisSyncMode.AxisZ:
                    WriteAngle(writer, rot.eulerAngles.z, compression);
                    break;

                case AxisSyncMode.AxisXY:
                    WriteAngle(writer, rot.eulerAngles.x, compression);
                    WriteAngle(writer, rot.eulerAngles.y, compression);
                    break;

                case AxisSyncMode.AxisXZ:
                    WriteAngle(writer, rot.eulerAngles.x, compression);
                    WriteAngle(writer, rot.eulerAngles.z, compression);
                    break;

                case AxisSyncMode.AxisYZ:
                    WriteAngle(writer, rot.eulerAngles.y, compression);
                    WriteAngle(writer, rot.eulerAngles.z, compression);
                    break;

                case AxisSyncMode.AxisXYZ:
                    WriteAngle(writer, rot.eulerAngles.x, compression);
                    WriteAngle(writer, rot.eulerAngles.y, compression);
                    WriteAngle(writer, rot.eulerAngles.z, compression);
                    break;
            }
        }

        static public void SerializeRotation2D(NetworkWriter writer, float rot, CompressionSyncMode compression)
        {
            WriteAngle(writer, rot, compression);
        }

        static public void SerializeSpin3D(NetworkWriter writer, Vector3 angularVelocity, AxisSyncMode mode, CompressionSyncMode compression)
        {
            switch (mode)
            {
                case AxisSyncMode.None:
                    break;

                case AxisSyncMode.AxisX:
                    WriteAngle(writer, angularVelocity.x, compression);
                    break;

                case AxisSyncMode.AxisY:
                    WriteAngle(writer, angularVelocity.y, compression);
                    break;

                case AxisSyncMode.AxisZ:
                    WriteAngle(writer, angularVelocity.z, compression);
                    break;

                case AxisSyncMode.AxisXY:
                    WriteAngle(writer, angularVelocity.x, compression);
                    WriteAngle(writer, angularVelocity.y, compression);
                    break;

                case AxisSyncMode.AxisXZ:
                    WriteAngle(writer, angularVelocity.x, compression);
                    WriteAngle(writer, angularVelocity.z, compression);
                    break;

                case AxisSyncMode.AxisYZ:
                    WriteAngle(writer, angularVelocity.y, compression);
                    WriteAngle(writer, angularVelocity.z, compression);
                    break;

                case AxisSyncMode.AxisXYZ:
                    WriteAngle(writer, angularVelocity.x, compression);
                    WriteAngle(writer, angularVelocity.y, compression);
                    WriteAngle(writer, angularVelocity.z, compression);
                    break;
            }
        }

        static public void SerializeSpin2D(NetworkWriter writer, float angularVelocity, CompressionSyncMode compression)
        {
            WriteAngle(writer, angularVelocity, compression);
        }

        static public Vector3 UnserializeVelocity3D(NetworkReader reader, CompressionSyncMode compression)
        {
            return reader.ReadVector3();
        }

        static public Vector3 UnserializeVelocity2D(NetworkReader reader, CompressionSyncMode compression)
        {
            return reader.ReadVector2();
        }

        static public Quaternion UnserializeRotation3D(NetworkReader reader, AxisSyncMode mode, CompressionSyncMode compression)
        {
            Quaternion rotation = Quaternion.identity;
            Vector3 rotv = Vector3.zero;

            switch (mode)
            {
                case AxisSyncMode.None:
                    break;

                case AxisSyncMode.AxisX:
                    rotv.Set(ReadAngle(reader, compression), 0, 0);
                    rotation.eulerAngles = rotv;
                    break;

                case AxisSyncMode.AxisY:
                    rotv.Set(0, ReadAngle(reader, compression), 0);
                    rotation.eulerAngles = rotv;
                    break;

                case AxisSyncMode.AxisZ:
                    rotv.Set(0, 0, ReadAngle(reader, compression));
                    rotation.eulerAngles = rotv;
                    break;

                case AxisSyncMode.AxisXY:
                    rotv.Set(ReadAngle(reader, compression), ReadAngle(reader, compression), 0);
                    rotation.eulerAngles = rotv;
                    break;

                case AxisSyncMode.AxisXZ:
                    rotv.Set(ReadAngle(reader, compression), 0, ReadAngle(reader, compression));
                    rotation.eulerAngles = rotv;
                    break;

                case AxisSyncMode.AxisYZ:
                    rotv.Set(0, ReadAngle(reader, compression), ReadAngle(reader, compression));
                    rotation.eulerAngles = rotv;
                    break;

                case AxisSyncMode.AxisXYZ:
                    rotv.Set(ReadAngle(reader, compression), ReadAngle(reader, compression), ReadAngle(reader, compression));
                    rotation.eulerAngles = rotv;
                    break;
            }
            return rotation;
        }

        static public float UnserializeRotation2D(NetworkReader reader, CompressionSyncMode compression)
        {
            return ReadAngle(reader, compression);
        }

        static public Vector3 UnserializeSpin3D(NetworkReader reader, AxisSyncMode mode, CompressionSyncMode compression)
        {
            Vector3 spin = Vector3.zero;
            switch (mode)
            {
                case AxisSyncMode.None:
                    break;

                case AxisSyncMode.AxisX:
                    spin.Set(ReadAngle(reader, compression), 0, 0);
                    break;

                case AxisSyncMode.AxisY:
                    spin.Set(0, ReadAngle(reader, compression), 0);
                    break;

                case AxisSyncMode.AxisZ:
                    spin.Set(0, 0, ReadAngle(reader, compression));
                    break;

                case AxisSyncMode.AxisXY:
                    spin.Set(ReadAngle(reader, compression), ReadAngle(reader, compression), 0);
                    break;

                case AxisSyncMode.AxisXZ:
                    spin.Set(ReadAngle(reader, compression), 0, ReadAngle(reader, compression));
                    break;

                case AxisSyncMode.AxisYZ:
                    spin.Set(0, ReadAngle(reader, compression), ReadAngle(reader, compression));
                    break;

                case AxisSyncMode.AxisXYZ:
                    spin.Set(ReadAngle(reader, compression), ReadAngle(reader, compression), ReadAngle(reader, compression));
                    break;
            }
            return spin;
        }

        static public float UnserializeSpin2D(NetworkReader reader, CompressionSyncMode compression)
        {
            return ReadAngle(reader, compression);
        }

        public override int GetNetworkChannel()
        {
            return Channels.DefaultUnreliable;
        }

        public override float GetNetworkSendInterval()
        {
            return m_SendInterval;
        }

        public override void OnStartAuthority()
        {
            // must reset this timer, or the server will continue to send target position instead of current position
            m_LastClientSyncTime = 0;
        }
    }
}
#endif //ENABLE_UNET