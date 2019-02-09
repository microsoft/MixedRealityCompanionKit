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
        Plugin.ExecuteOnUnityThread(() =>
        {
            if (e.CurrentState == RealtimeVideoServer.ServerState.ListenerConnected)
            {
                StartWebCam();
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

        Stopwatch sw = new Stopwatch();

        sw.Start();

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

        sw.Stop();

        UnityEngine.Debug.LogFormat("Elapsed={%d}", sw.Elapsed);

        /*
        // TODO: Parrelize copy?
        int byteIdx = 0;
        for (int i = 0; i < webcam_interop.Length; i++)
        {
            // TODO: webcamtexture vertically flipped?
            Color32 c = webcam_interop[webcam_interop.Length - i - 1];

            frameBuffer[byteIdx] = c.b;
            frameBuffer[byteIdx + 1] = c.g;
            frameBuffer[byteIdx + 2] = c.r;
            frameBuffer[byteIdx + 3] = c.a;

            byteIdx += 4;
        }*/

        this.server.WriteFrame(this.frameBuffer);
    }
}
