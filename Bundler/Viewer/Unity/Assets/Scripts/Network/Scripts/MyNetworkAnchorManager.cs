using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Networking;

public class MyNetworkAnchorManager : NetworkBehaviour
{
    private static MyNetworkAnchorManager instance;
    public static MyNetworkAnchorManager Instance
    {
        get
        {
            return instance;
        }
    }

    /// <summary>
    /// When data arrives, this event is raised.
    /// </summary>
    public delegate void OnAnchorSourceChanged(MyNetworkAnchorManager source, SharedAnchorData args);
    public event OnAnchorSourceChanged AnchorSourceChanged;

    /// <summary>
    /// The current source of the shared anchor 
    /// </summary>
    [SyncVar(hook = "AnchorSourceHook")]
    public SharedAnchorData AnchorSource;

    /// <summary>
    /// Set the anchor source.
    /// </summary>
    public void SetAnchorSource(SharedAnchorData anchorSource)
    {
        if (isServer)
        {
            Debug.Log("Server is setting the anchor source: " + anchorSource.ToString());
            AnchorSource = anchorSource;
        }
        else
        {
            CmdSetAnchorSource(anchorSource);
        }
    }

    /// <summary>
    /// A network command to allow clients to change the anchor source.
    /// </summary>
    [Command]
    private void CmdSetAnchorSource(SharedAnchorData anchorSource)
    {
        Debug.Log("Command is setting the anchor source: " + anchorSource.ToString());
        AnchorSource = anchorSource;
    }

    /// <summary>
    /// On creation save this an the static instance. There should be only one manager.
    /// </summary>
    private void Awake()
    {
        instance = this;
    }

    /// <summary>
    /// Raise a change event so to notify listeners
    /// </summary>
    private void AnchorSourceHook(SharedAnchorData data)
    {
        if (AnchorSourceChanged != null)
        {
            AnchorSourceChanged(this, data);
        }
    }
}
