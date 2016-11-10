// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
using UnityEngine;
using System.Collections.Generic;
using System.Linq;

#if NETFX_CORE
using UnityEngine.Windows.Speech;
#endif

public class SpeechCommands : MonoBehaviour
{
    public ZoneCalibrationManager ZoneManager;
    
    Dictionary<string, System.Action> keywords = new Dictionary<string, System.Action>();

#if NETFX_CORE
    KeywordRecognizer keywordRecognizer;
#endif
    
    void Start()
    {
        

        keywords.Add("quit application", () =>
        {
#if UNITY_WSA && !UNITY_EDITOR
            Windows.ApplicationModel.Core.CoreApplication.Exit();
#else
            UnityEngine.Application.Quit();
#endif
        });
        
        keywords.Add("align poster 1", () =>
        {
            if (ZoneManager != null)
            {
                ZoneManager.AlignZone(0);
            }
        });

        keywords.Add("align poster 2", () =>
        {
            if (ZoneManager != null)
            {
                ZoneManager.AlignZone(1);
            }
        });


        keywords.Add("lock alignment", () =>
        {
            if (ZoneManager != null)
            {
                ZoneManager.LockZone(true);
            }
        });

        keywords.Add("Scale Up", () =>
        {
            if (ZoneManager.currentZone != null)
            {
                ZoneManager.currentZone.transform.localScale =
                    ZoneManager.currentZone.transform.localScale +
                    (Vector3.one * .005f);
            }
        });
        keywords.Add("Scale Down", () =>
        {
            if (ZoneManager.currentZone != null)
            {
                ZoneManager.currentZone.transform.localScale =
                    ZoneManager.currentZone.transform.localScale +
                    -(Vector3.one * .005f);
            }
        });

        keywords.Add("move Z plus", () =>
        {
            if (ZoneManager.currentZone != null)
            {
                ZoneManager.currentZone.transform.localPosition =
                ZoneManager.currentZone.transform.localPosition + Vector3.forward * .005f;
            }
        });
        keywords.Add("move Z minus", () =>
        {
            if (ZoneManager.currentZone != null)
            {
                ZoneManager.currentZone.transform.localPosition =
                ZoneManager.currentZone.transform.localPosition - Vector3.forward * .005f;
            }
        });

        keywords.Add("move X plus", () =>
        {
            if (ZoneManager.currentZone != null)
            {
                ZoneManager.currentZone.transform.localPosition =
                ZoneManager.currentZone.transform.localPosition + Vector3.right * .005f;
            }
        });
        keywords.Add("move X minus", () =>
        {
            if (ZoneManager.currentZone != null)
            {
                ZoneManager.currentZone.transform.localPosition =
                ZoneManager.currentZone.transform.localPosition - Vector3.right * .005f;
            }
        });

        keywords.Add("move Y plus", () =>
        {
            if (ZoneManager.currentZone != null)
            {
                ZoneManager.currentZone.transform.localPosition =
                ZoneManager.currentZone.transform.localPosition + Vector3.up * .005f;
            }
        });
        keywords.Add("move Y minus", () =>
        {
            if (ZoneManager.currentZone != null)
            {
                ZoneManager.currentZone.transform.localPosition =
                ZoneManager.currentZone.transform.localPosition - Vector3.up * .005f;
            }
        });

#if NETFX_CORE
        keywordRecognizer = new KeywordRecognizer(keywords.Keys.ToArray());
        keywordRecognizer.OnPhraseRecognized += KeywordRecognizer_OnPhraseRecognized;
        keywordRecognizer.Start();
#endif
    }

#if NETFX_CORE
    private void KeywordRecognizer_OnPhraseRecognized(PhraseRecognizedEventArgs args)
    {
        System.Action keywordAction;
        // if the keyword recognized is in our dictionary, call that Action.
        if (keywords.TryGetValue(args.text, out keywordAction))
        {
            keywordAction.Invoke();
        }
    }
#endif

    private void OnDestroy()
    {
#if NETFX_CORE
        if (keywordRecognizer != null)
        {
            keywordRecognizer.OnPhraseRecognized -= KeywordRecognizer_OnPhraseRecognized;
            keywordRecognizer.Stop();
            keywordRecognizer.Dispose();
            keywordRecognizer = null;
        }
#endif
    }
}
