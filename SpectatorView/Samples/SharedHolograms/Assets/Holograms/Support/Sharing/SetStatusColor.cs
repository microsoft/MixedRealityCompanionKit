using UnityEngine;
using System.Collections;

/// <summary>
/// Changes the color on the CursorOffHolograms
/// to give visual feedback on anchor sharing states.
/// </summary>
public class SetStatusColor : MonoBehaviour
{
    public Color InitializingColor = Color.magenta;
    public Color ImportingOrExportingColor = Color.yellow;
    public Color UploadingColor = Color.blue;
    public Color DownloadingColor = Color.green;    
    public Color FailureColor = Color.red;

    SpectatorView.SV_ImportExportAnchorManager anchorManager;
    Color startColor;
    Light pointLight;

    void Start()
    {
        pointLight = GetComponent<Light>();
        startColor = pointLight.color;

        anchorManager = SpectatorView.SV_ImportExportAnchorManager.Instance;
    }

    void Update()
    {
        if (anchorManager != null)
        {
            switch (anchorManager.currentState)
            {
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.AnchorStore_Initializing:
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.Start:
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.AnchorStore_Initialized:
                    pointLight.color = InitializingColor;
                    break;
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.Importing:
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.InitialAnchorRequired:
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.CreatingInitialAnchor:
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.DataReady:
                    pointLight.color = ImportingOrExportingColor;
                    break;
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.UploadingInitialAnchor:
                    pointLight.color = UploadingColor;
                    break;
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.DataRequested:
                    pointLight.color = DownloadingColor;
                    break;
                case SpectatorView.SV_ImportExportAnchorManager.ImportExportState.Failed:
                    pointLight.color = FailureColor;
                    break;
                default:
                    pointLight.color = startColor;
                    break;
            }
        }

    }
}
