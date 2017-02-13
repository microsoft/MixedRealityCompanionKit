// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections;

namespace SpectatorView
{
    /// <summary>
    /// Changes the color on the CursorOffHolograms
    /// to give visual feedback on anchor sharing states.
    /// </summary>
    public class SetStatusColor : MonoBehaviour
    {
        public Color InitializingColor = Color.magenta;
        public Color ImportingOrExportingColor = Color.yellow;
        public Color UploadingColor = Color.cyan;
        public Color DownloadingColor = Color.green;
        public Color FailureColor = Color.red;

        SpectatorView.SV_ImportExportAnchorManager anchorManager;
        Color startColor;
        MeshRenderer meshRenderer;

        void Start()
        {
            meshRenderer = GetComponent<MeshRenderer>();
            startColor = meshRenderer.material.color;

            anchorManager = SpectatorView.SV_ImportExportAnchorManager.Instance;
        }

        void Update()
        {
            if (anchorManager != null)
            {
                switch (anchorManager.currentState)
                {
                    case SV_ImportExportAnchorManager.ImportExportState.AnchorStore_Initializing:
                    case SV_ImportExportAnchorManager.ImportExportState.Start:
                    case SV_ImportExportAnchorManager.ImportExportState.AnchorStore_Initialized:
                        meshRenderer.material.color = InitializingColor;
                        break;
                    case SV_ImportExportAnchorManager.ImportExportState.Importing:
                    case SV_ImportExportAnchorManager.ImportExportState.InitialAnchorRequired:
                    case SV_ImportExportAnchorManager.ImportExportState.CreatingInitialAnchor:
                    case SV_ImportExportAnchorManager.ImportExportState.DataReady:
                        meshRenderer.material.color = ImportingOrExportingColor;
                        break;
                    case SV_ImportExportAnchorManager.ImportExportState.UploadingInitialAnchor:
                        meshRenderer.material.color = UploadingColor;
                        break;
                    case SV_ImportExportAnchorManager.ImportExportState.DataRequested:
                        meshRenderer.material.color = DownloadingColor;
                        break;
                    case SV_ImportExportAnchorManager.ImportExportState.Failed:
                        meshRenderer.material.color = FailureColor;
                        break;
                    default:
                        meshRenderer.material.color = startColor;
                        break;
                }
            }

        }
    }
}
