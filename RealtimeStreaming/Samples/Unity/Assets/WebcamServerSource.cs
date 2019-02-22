using RealtimeStreaming;
using System.Diagnostics;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;

public class WebcamServerSource : MonoBehaviour
{
    public RealtimeVideoServer server;

    public RawImage debugImg;

    private WebCamTexture webcam;

    private Color32[] webcam_interop;
    private byte[] frameBuffer;
    
    private float timer = 0.0f;
    private float WRITE_FPS = 1.0f / 30.0f;

    private void Start()
    {
        if (server == null)
        {
            server = GetComponent<RealtimeVideoServer>();
        }

        this.server.ServerStateChanged += this.OnServerStateChanged;
        this.server.StartListener();

        StartWebCam();
    }

    private void Update()
    {
        if (this.server.CurrentState == RealtimeVideoServer.ServerState.Ready)
        {
            timer += Time.deltaTime;

            if (timer > WRITE_FPS)
            {
                timer = 0;
                this.CaptureWebcam();
            }
        }
    }

    private void OnServerStateChanged(object sender, StateChangedEventArgs<RealtimeVideoServer.ServerState> e)
    {
        // TODO: StartWebCam needs to occur on UnitymainThread, move to Update() method action like plugin.QueueAction()
        Plugin.ExecuteOnUnityThread(() =>
        {
            if (e.CurrentState == RealtimeVideoServer.ServerState.ListenerConnected)
            {
                UnityEngine.Debug.Log("Server State changed to ListenerConnected - Starting WebCam");
                //StartWebCam();
            }
        });
    }

    private void StartWebCam()
    {
        if (webcam != null)
        {
            return;
        }

        webcam = new WebCamTexture(1280, 720);
        debugImg.texture = webcam;
        debugImg.material.mainTexture = webcam;
        webcam.Play();

        webcam_interop = new Color32[webcam.width * webcam.height];
        frameBuffer = new byte[webcam.width * webcam.height * 4];
    }

    private void CaptureWebcam()
    {
        if (webcam == null && !webcam.isPlaying)
        {
            return;
        }

        webcam.GetPixels32(webcam_interop);

        // Parraelize copy of Colo32 webcam data into framebuffer
        Parallel.For(0, webcam_interop.Length,
            index =>
            {
                int byteIdx = index << 2; // multiply by 4
                Color32 c = webcam_interop[webcam_interop.Length - index - 1];

                frameBuffer[byteIdx] = c.b;
                frameBuffer[byteIdx + 1] = c.g;
                frameBuffer[byteIdx + 2] = c.r;
                frameBuffer[byteIdx + 3] = c.a;
            });

        this.server.WriteFrame(this.frameBuffer);
    }
}
