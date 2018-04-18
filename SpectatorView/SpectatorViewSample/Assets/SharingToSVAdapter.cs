using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// To keep spectator view and the shared experience networking stack independent, some data will need to be shared in this adapter.
// Your version of this file may be different depending on your networking stack, but the data that needs to be shared will be the same.

public class SharingToSVAdapter : MonoBehaviour
{
#if UNITY_EDITOR
    [Header("Scene Objects")]
    [Tooltip("The SpectatorViewManager gameobject.")]
    public GameObject SpectatorViewGO;

    [Tooltip("The gameobject that will be anchored.")]
    public GameObject AnchorGO;


    [Header("IPs")]
    [Tooltip("The IP of the shared experience server, you may not need this if your shared experience uses UDP broadcasting for network discovery.")]
    public string ServerIP;

    [Tooltip("The IP of the HoloLens running the SpectatorViewPoseProvider application.")]
    public string SpectatorViewIP;

    [Tooltip("The IP of the HoloLens in the shared experience that is currently sharing an anchor.")]
    public string AnchorOwnderIP;

    private string previousAnchorName = string.Empty;

    void Start()
    {
        if (SpectatorViewGO == null)
        {
            SpectatorViewGO = SpectatorView.SpectatorViewManager.Instance.gameObject;
        }
        if (AnchorGO == null)
        {
            AnchorGO = SimpleSharing.AnchorManager.Instance.objectToAnchor;
        }

        SpectatorViewGO.transform.SetParent(AnchorGO.transform);
    }

    void Update()
    {
        if (CheckForUpdatedAnchor())
        {
            //TODO: Send a message to the SVPoseProvider that we need to import the new anchor.
        }
    }

    // Check if our anchor has changed so we can react to it in the SVPoseProvider.
    //NOTE: You may choose to instead use a syncvar with a hook if that works better in your network stack.
    bool CheckForUpdatedAnchor()
    {
        if (SimpleSharing.SharingManager.Instance != null)
        {
            string anchorName = SimpleSharing.SharingManager.Instance.AnchorName;
            if (anchorName != previousAnchorName)
            {
                Debug.Log("We have a new anchor: " + anchorName);
                previousAnchorName = anchorName;

                // We got a null anchor, do not update the remote anchor.
                if (anchorName.Trim() == string.Empty)
                {
                    return false;
                }

                // Update the remote anchor.
                return true;
            }
        }

        return false;
    }
#endif
}
