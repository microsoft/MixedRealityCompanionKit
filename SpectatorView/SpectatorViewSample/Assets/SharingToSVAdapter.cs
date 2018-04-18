using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// To keep spectator view and the shared experience networking stack independent, some data will need to be shared in this adapter.
public class SharingToSVAdapter : MonoBehaviour
{
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

    }
}
