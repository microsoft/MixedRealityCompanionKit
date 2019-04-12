using RealtimeStreaming;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerControl : MonoBehaviour
{
    public TextField IPAddressInput;
    public TextField PortInput;
    public RealtimeVideoPlayer rtPlayer;
    public MeshRenderer ConnectionIndicator;
    public MeshRenderer target;

    private void Start()
    {
        if (rtPlayer == null)
        {
            rtPlayer = GetComponent<RealtimeVideoPlayer>();
        }

        rtPlayer.PlayerStateChanged += RtPlayer_PlayerStateChanged;
    }

#if UNITY_EDITOR
    private void Update()
    {
        if (Input.GetKey(KeyCode.P))
        {
            DiscoverPlayer();
        }
        else if (Input.GetKey(KeyCode.Q))
        {
            rtPlayer.Shutdown();
        }
    }
#endif

    public void DiscoverPlayer()
    {
        Debug.Log("Discover");
        rtPlayer.ConnectPlayer(true);
    }

    public void ConnectPlayer()
    {
        rtPlayer.ConnectTo = this.IPAddressInput.Value;
        rtPlayer.Port = Convert.ToUInt16(this.PortInput.Value);
        rtPlayer.ConnectPlayer(false);
    }

    private void RtPlayer_PlayerStateChanged(object sender, StateChangedEventArgs<RealtimeVideoPlayer.PlaybackState> e)
    {
        if (ConnectionIndicator != null)
        {
            Color c = Color.red;
            switch (e.CurrentState)
            {
                case RealtimeVideoPlayer.PlaybackState.Opening:
                    c = Color.yellow;
                    break;
                case RealtimeVideoPlayer.PlaybackState.Playing:
                    c = Color.green;
                    break;
                default:
                    break;
            }

            ConnectionIndicator.material.SetColor("_InnerGlowColor", c);
        }

        if (e.CurrentState == RealtimeVideoPlayer.PlaybackState.Playing)
        {
            // TODO: Check that shader is expected
            if (this.target != null)
            {
                this.target.material.SetTexture("_MainTex_Luma", rtPlayer.Texture_Luma);
                this.target.material.SetTexture("_MainTex_Chroma", rtPlayer.Texture_Chroma);
            }
        }
    }

}
