// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

// To keep spectator view and the shared experience networking stack independent, some data will need to be shared in this adapter.
// Your version of this file may be different depending on your networking stack, but the data that needs to be shared will be the same.

public class SharingToSVAdapter : MonoBehaviour
{
    [Header("Scene Objects")]
    [Tooltip("The SpectatorViewManager gameobject.")]
    public GameObject SpectatorViewGO;

    [Tooltip("The gameobject that will be anchored.")]
    public GameObject AnchorGO;


    [Header("IPs - Rebuild if these change.")]
    [Tooltip("The IP of the shared experience server, you may not need this if your shared experience uses UDP broadcasting for network discovery.")]
    public string ServerIP;

    [Tooltip("The IP of the HoloLens running the SpectatorViewPoseProvider application.")]
    public string SpectatorViewIP;

    [Header("Port anchor is shared on in your shared experience.")]
    public int AnchorPort = 11000;

    [Header("Leave this blank to always create a new anchor with the last HoloLens to join.")]
    [Tooltip("The IP of the HoloLens in the shared experience that is currently sharing an anchor.")]
    public string AnchorOwnerIP;

    private string previousAnchorName = string.Empty;

    bool allIPsSet = false;

    void Start()
    {
        SetTransforms();
        allIPsSet = SetIPs();
    }

    // If transforms are null, populate them with default values.
    // If transform are not null, overwrite other gameobjects.
    void SetTransforms()
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

    // If IPs are valid, overwrite other gameobjects.
    // If IPs are empty, populate from other gameobjects.
    bool SetIPs()
    {
        // Check that all gameobjects we need have been initialized.
        if (SimpleSharing.SharingManager.Instance == null
            || SpectatorView.SpectatorViewManager.Instance == null
            || SimpleSharing.AnchorNetworkTransmitter.Instance == null)
        {
            return false;
        }

        // Server
        if (ServerIP == string.Empty)
        {
            ServerIP = SimpleSharing.SharingManager.Instance.ServerIP;
        }
        else
        {
            SimpleSharing.SharingManager.Instance.ServerIP = ServerIP;
        }

#if UNITY_EDITOR
        // Spectator View
        if (SpectatorViewIP == string.Empty)
        {
            SpectatorViewIP = SpectatorView.SpectatorViewManager.Instance.SpectatorViewHoloLensIP;
        }
        else
        {
            SpectatorView.SpectatorViewManager.Instance.CreateSpectatorViewConnection(SpectatorViewIP);
        }
#endif

        // Anchor
        //NOTE: Depending on your network stack, the anchor owner may be shared to you, so this script should be updated.
        if (AnchorOwnerIP == string.Empty)
        {
            AnchorOwnerIP = SimpleSharing.AnchorNetworkTransmitter.Instance.AnchorOwnerIP;
            // If we did not set our anchor owner here, then the port may be wrong too.
            AnchorPort = SimpleSharing.AnchorNetworkTransmitter.Instance.SendConnectionPort;
        }
        else
        {
            SimpleSharing.AnchorNetworkTransmitter.Instance.AnchorOwnerIP = AnchorOwnerIP;
            SimpleSharing.AnchorNetworkTransmitter.Instance.SendConnectionPort = AnchorPort;
        }

        // All IPs have been set, but we may potentially have received empty strings from other prefabs.
        ValidateIPs();

        return true;
    }

    // Log any errors for IPs that could not be established.
    // This will require user input to address.
    void ValidateIPs()
    {
        if (ServerIP == string.Empty)
        {
            Debug.LogError("ServerIP is null, but it should not be at this time.  Please check this script or the sharing manager.");
        }

        if (SpectatorViewIP == string.Empty)
        {
            Debug.LogError("SpectatorViewIP is null, but it should not be at this time.  Please check this script or the spectator view manager.");
        }

        //NOTE: Depending on your network stack, the anchor owner may be shared to you, so this script should be updated.
        if (AnchorOwnerIP == string.Empty)
        {
            Debug.LogWarning("AnchorOwnerIP is null, so new HoloLens devices to join will always create a new anchor.  This will only allow for 1 HoloLens client in your experience.  Please check this script or the AnchorNetworkTransmitter if this is not your intended behavior.");
            SimpleSharing.AnchorNetworkTransmitter.Instance.ForceCreateAnchor = true;
        }
    }

    void Update()
    {
#if UNITY_EDITOR
        if (CheckForUpdatedAnchor())
        {
            //Send a message to the SVPoseProvider that we need to import the new anchor.
            if (SpectatorView.SpectatorViewManager.Instance != null)
            {
                if (AnchorOwnerIP == string.Empty)
                {
                    AnchorOwnerIP = SimpleSharing.SharingManager.Instance.AnchorIP;
                    SimpleSharing.AnchorNetworkTransmitter.Instance.AnchorOwnerIP = AnchorOwnerIP;
                }

                SpectatorView.SpectatorViewManager.Instance.SetAnchorInformation(AnchorOwnerIP, AnchorPort, previousAnchorName);
            }
        }
#endif

        if (!allIPsSet)
        {
            allIPsSet = SetIPs();
        }
    }

    // Check if our anchor has changed so we can react to it in the SVPoseProvider.
    //NOTE: You may choose to instead use a syncvar with a hook if that works better in your network stack.
    bool CheckForUpdatedAnchor()
    {
        bool newAnchor = false;
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
                    newAnchor = false;
                }

                // Update the remote anchor.
                newAnchor = true;
            }

            string newAnchorOwnerIP = SimpleSharing.SharingManager.Instance.AnchorIP;
            if (AnchorOwnerIP != newAnchorOwnerIP)
            {
                Debug.Log("We have a new anchor owner: " + newAnchorOwnerIP);
                AnchorOwnerIP = SimpleSharing.SharingManager.Instance.AnchorIP;
                SimpleSharing.AnchorNetworkTransmitter.Instance.AnchorOwnerIP = AnchorOwnerIP;
                newAnchor = true;
            }
        }

        return newAnchor;
    }
}
