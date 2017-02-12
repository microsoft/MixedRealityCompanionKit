using UnityEngine;
using HoloToolkit.Unity;
using HoloToolkit.Unity.SpatialMapping;
using HoloToolkit.Sharing;

public class ExplodeTarget : SpectatorView.SV_Singleton<ExplodeTarget>
{
    [Tooltip("Object to disable after the target explodes.")]
    public GameObject Target;

    [Tooltip("Object to enable after the target explodes.")]
    public GameObject Underworld;

    bool registeredSharingStageCallbacks = false;

    void Start()
    {
        // Attach ExplodingBlob to our target, so it will explode when hit by projectiles.
        this.transform.Find("EnergyHub/BlobOutside").gameObject.AddComponent<ExplodingBlob>();

        // Handles the ExplodeTarget message from the network.
        CustomMessages.Instance.MessageHandlers[CustomMessages.TestMessageID.ExplodeTarget] = this.OnExplodeTarget;
    }

    void Update()
    {
        if (!registeredSharingStageCallbacks && SharingStage.Instance != null && SharingStage.Instance.SessionUsersTracker != null)
        {
            registeredSharingStageCallbacks = true;

            SharingStage.Instance.SessionUsersTracker.UserJoined += SessionUsersTracker_UserJoined;
        }
    }

    protected override void OnDestroy()
    {
        registeredSharingStageCallbacks = false;

        if (SharingStage.Instance != null)
        {
            SharingStage.Instance.SessionUsersTracker.UserJoined -= SessionUsersTracker_UserJoined;
        }

        base.OnDestroy();
    }

    /// <summary>
    /// When a new user joins the session after the underworld is enabled,
    /// reset the target so that everyone is in the same game state.
    /// </summary>
    private void SessionUsersTracker_UserJoined(User user)
    {
        if (Underworld.activeSelf)
        {
            HologramPlacement.Instance.ResetStage();
        }
    }

    /// <summary>
    /// Disables target and spatial mapping after an explosion occurs, enables the underworld.
    /// </summary>
    public void OnExplode()
    {
        // Hide the target and show the underworld.
        Target.SetActive(false);
        Underworld.SetActive(true);
        Underworld.transform.localPosition = Target.transform.localPosition;

        // Disable spatial mapping so drones can fly out of the underworld and players can shoot projectiles inside.
        SpatialMappingManager.Instance.gameObject.SetActive(false);
    }

    /// <summary>
    /// When a remote system has triggered an explosion, we'll be notified here.
    /// </summary>
    void OnExplodeTarget(NetworkInMessage msg)
    {
        OnExplode();
    }
}