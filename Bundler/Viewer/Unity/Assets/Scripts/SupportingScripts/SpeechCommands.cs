// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;
using System.Collections.Generic;
using System.Linq;

#if NETFX_CORE
using UnityEngine.Windows.Speech;
#endif

namespace Persistence
{
    public class SpeechCommands : MonoBehaviour
    {
        Dictionary<string, System.Action> keywords = new Dictionary<string, System.Action>();

#if NETFX_CORE
    KeywordRecognizer keywordRecognizer;
#endif

        void Start()
        {
            keywords.Add("show f p s", () =>
            {
                MasterController.Instance.ToggleDebugInfo(true);
            });
            keywords.Add("hide f p s", () =>
            {
                MasterController.Instance.ToggleDebugInfo(false);
            });
            
            keywords.Add("reset stage position", () =>
            {
                MasterController.Instance.SetDefaultStagePosition();
            });
            
            keywords.Add("quit application", () =>
        {
#if NETFX_CORE
            Windows.ApplicationModel.Core.CoreApplication.Exit();
#else
                UnityEngine.Application.Quit();
#endif
            });


#if NETFX_CORE
            if (HololensHelper.IsDeviceHololens())
            {
                keywordRecognizer = new KeywordRecognizer(keywords.Keys.ToArray());
                keywordRecognizer.OnPhraseRecognized += KeywordRecognizer_OnPhraseRecognized;
                keywordRecognizer.Start();
            }
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
}