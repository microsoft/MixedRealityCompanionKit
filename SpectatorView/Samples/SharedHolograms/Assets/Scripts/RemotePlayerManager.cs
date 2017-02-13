using HoloToolkit.Sharing;
using HoloToolkit.Unity;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Adds and updates the head transforms of remote users.  
/// Head transforms are sent and received in the local coordinate space of the GameObject
/// this component is on.  
/// </summary>
public class RemotePlayerManager : SpectatorView.SV_Singleton<RemotePlayerManager>
{
    public class RemoteHeadInfo
    {
        public long UserID;
        public GameObject HeadObject;
        public Vector3 headObjectPositionOffset;
        public int PlayerAvatarIndex;
        public int HitCount;
        public bool Active;
        public bool Anchored;
    }

    /// <summary>
    /// Keep a list of the remote heads
    /// </summary>
    Dictionary<long, RemoteHeadInfo> remoteHeads = new Dictionary<long, RemoteHeadInfo>();

    public IEnumerable<RemoteHeadInfo> remoteHeadInfos
    {
        get
        {
            return remoteHeads.Values;
        }
    }

    CustomMessages customMessages;

    bool registeredSharingStageCallbacks = false;

    void Start()
    {
        customMessages = CustomMessages.Instance;

        customMessages.MessageHandlers[CustomMessages.TestMessageID.HeadTransform] = this.UpdateHeadTransform;
        customMessages.MessageHandlers[CustomMessages.TestMessageID.UserAvatar] = this.UpdateUserAvatar;
        customMessages.MessageHandlers[CustomMessages.TestMessageID.UserHit] = this.ProcessUserHit;
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

    void Update()
    {
        if (!registeredSharingStageCallbacks && SharingStage.Instance != null && SharingStage.Instance.SessionUsersTracker != null)
        {
            registeredSharingStageCallbacks = true;

            SharingStage.Instance.SessionUsersTracker.UserJoined += SessionUsersTracker_UserJoined;
            SharingStage.Instance.SessionUsersTracker.UserLeft += SessionUsersTracker_UserLeft;
        }
    }
    /// <summary>
    /// Called when a new user is leaving.
    /// </summary>
    private void SessionUsersTracker_UserLeft(User user)
    {
        if (remoteHeads.ContainsKey(user.GetID()))
        {
            RemoveRemoteHead(this.remoteHeads[user.GetID()].HeadObject);
            this.remoteHeads.Remove(user.GetID());
        }
    }

    /// <summary>
    /// Called when a user is joining.
    /// </summary>
    private void SessionUsersTracker_UserJoined(User user)
    {
        GetRemoteHeadInfo(user.GetID());
    }

    /// <summary>
    /// Process user hit.
    /// </summary>
    /// <param name="msg"></param>
    void ProcessUserHit(NetworkInMessage msg)
    {
        // Parse the message
        long userID = msg.ReadInt64();
        long hitUserId = msg.ReadInt64();

        RemoteHeadInfo headInfo = GetRemoteHeadInfo(userID);
        FriendlyDrone soh = headInfo.HeadObject.GetComponentInChildren<FriendlyDrone>();
        if (soh != null)
        {
            soh.Happy();
        }
        headInfo.HitCount++;

        AudioSource remoteHeadAudio = headInfo.HeadObject.GetComponentInChildren<AudioSource>();
        PlayerAvatarParameters playerParams = PlayerAvatarStore.Instance.PlayerAvatars[headInfo.PlayerAvatarIndex].GetComponent<PlayerAvatarParameters>();
        if (hitUserId == customMessages.localUserID)
        {
            // ack they hit ME!!! 
            // Play the 'they hit me' sound.
            AudioSource localAudioSource = Camera.main.GetComponent<AudioSource>();
            PlayerAvatarParameters localPlayerParams = PlayerAvatarStore.Instance.PlayerAvatars[LocalPlayerManager.Instance.AvatarIndex].GetComponent<PlayerAvatarParameters>();
            localAudioSource.clip = localPlayerParams.SomeoneHitPlayerClip;
            localAudioSource.Play();
        }

        // Play the 'I hit someone' sound for the user who hit someone.
        remoteHeadAudio.clip = playerParams.PlayerHitSomeoneClip;
        remoteHeadAudio.Play();
    }

    /// <summary>
    /// Called when a user's avatar has changed.
    /// </summary>
    /// <param name="msg"></param>
    void UpdateUserAvatar(NetworkInMessage msg)
    {
        // Parse the message
        long userID = msg.ReadInt64();

        RemoteHeadInfo headInfo = GetRemoteHeadInfo(userID);
        headInfo.PlayerAvatarIndex = msg.ReadInt32();

        // Configure the remote user's head object
        if (headInfo.HeadObject != null)
        {
            Destroy(headInfo.HeadObject);
        }

        headInfo.HeadObject = Instantiate(PlayerAvatarStore.Instance.PlayerAvatars[headInfo.PlayerAvatarIndex]);
        headInfo.headObjectPositionOffset = headInfo.HeadObject.transform.localPosition;
        headInfo.HeadObject.transform.parent = this.transform;
        headInfo.HeadObject.GetComponent<PlayerAvatarParameters>();
        FriendlyDrone droneScript = headInfo.HeadObject.GetComponentInChildren<FriendlyDrone>();
        droneScript.OwningUserId = userID;

        // And since we've been sent an avatar, the user is now ready to play.
        headInfo.HeadObject.GetComponentInChildren<MeshRenderer>().enabled = true;
        headInfo.Active = true;
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
            LocalPlayerManager.Instance.SendUserAvatar();

            this.remoteHeads.Add(userID, headInfo);
        }

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

        Vector3 headPos = customMessages.ReadVector3(msg);

        Quaternion headRot = customMessages.ReadQuaternion(msg);

        RemoteHeadInfo headInfo = GetRemoteHeadInfo(userID);

        if (headInfo.HeadObject != null)
        {
            // If we don't have our anchor established, don't draw the remote head.
            headInfo.HeadObject.SetActive(headInfo.Anchored);

            headInfo.HeadObject.transform.localPosition = headPos + headRot * headInfo.headObjectPositionOffset;

            headInfo.HeadObject.transform.localRotation = headRot;
        }

        headInfo.Anchored = (msg.ReadByte() > 0);
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
}