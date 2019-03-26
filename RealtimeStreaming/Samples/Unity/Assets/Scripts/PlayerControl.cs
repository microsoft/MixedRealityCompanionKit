using RealtimeStreaming;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerControl : MonoBehaviour
{
    public Text ipAddressInput;
    public RealtimeVideoPlayer rtPlayer;
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
            StartPlayer();
        }
        else if (Input.GetKey(KeyCode.Q))
        {
            rtPlayer.Shutdown();
        }
    }
#endif

    public void StartPlayer()
    {
        rtPlayer.ConnectTo = this.ipAddressInput.text;
        rtPlayer.ConnectPlayer(true);
    }

    private void RtPlayer_PlayerStateChanged(object sender, StateChangedEventArgs<RealtimeVideoPlayer.PlaybackState> e)
    {
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
