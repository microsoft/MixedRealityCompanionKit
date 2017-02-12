using UnityEngine;
using HoloToolkit.Unity;

/// <summary>
/// Manages local player state.
/// </summary>
public class LocalPlayerManager : SpectatorView.SV_Singleton<LocalPlayerManager>
{
    /// <summary>
    /// The selected avatar index for the player.
    /// </summary>
    public int AvatarIndex { get; private set; }

    /// <summary>
    /// Changes the user's avatar and lets everyone know.
    /// </summary>
    /// <param name="AvatarIndex"></param>
    public void SetUserAvatar(int AvatarIndex)
    {
        this.AvatarIndex = AvatarIndex;

        // Let everyone else know who we are.
        SendUserAvatar();
    }

    /// <summary>
    /// Broadcasts the user's avatar to other players.
    /// </summary>
    public void SendUserAvatar()
    {
        CustomMessages.Instance.SendUserAvatar(AvatarIndex);
    }

    // Send the user's position each frame.
    void Update()
    {
        if (SpectatorView.SV_ImportExportAnchorManager.Instance.AnchorEstablished)
        {
            // Grab the current head transform and broadcast it to all the other users in the session
            Transform headTransform = Camera.main.transform;

            // Transform the head position and rotation into local space
            Vector3 headPosition = this.transform.InverseTransformPoint(headTransform.position);
            Quaternion headRotation = Quaternion.Inverse(this.transform.rotation) * headTransform.rotation;
            CustomMessages.Instance.SendHeadTransform(headPosition, headRotation, 0x1);
        }
    }
}