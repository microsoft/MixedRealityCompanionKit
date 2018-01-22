// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MenuBundleCard : Clickable
{
    public GameObject ActiveRoot;
    public GameObject InactiveRoot;
    public Text NameTextBlock;
    public Text DateTextBlock;
    public Text TimeTextBlock;
    public Text VersionTextBlock;
    public Image ThumbImageObject;
    public GameObject OKIcon;
    public GameObject ProcessingIcon;
    public GameObject ErrorIcon;
    public GameObject QueuedIcon;
    public GameObject Highlight;
    public GameObject Border;
    public Color BorderNormalColor = Color.white;
    public Color BorderOverColor = Color.red;

    public AssetInfo AssetInfo { get; private set; }


    private static Dictionary<string, Sprite> ThumbnailCache = new Dictionary<string, Sprite>();

    protected override void Initialize()
    {
        base.Initialize();
        SetState(CardState.hide, false);
        SetInfo(new AssetInfo(), true);
    }

    private void OnEnable()
    {
        UpdateThumb();
    }

    public void SetInfo(AssetInfo info, bool forceRefresh = false)
    {
        if (!AssetInfo.IsEmpty() && AssetInfo.Equals(info) && !forceRefresh)
        {
            return;
        }

        this.AssetInfo = info;

        textureLoader = null;

        if (AssetInfo.IsEmpty())
        {
            ActiveRoot.SetActive(false);
            InactiveRoot.SetActive(true);
            return;
        }

        InactiveRoot.SetActive(false);
        ActiveRoot.SetActive(true);

        NameTextBlock.text = info.Name;
        var updated = Convert.ToDateTime(string.IsNullOrEmpty(info.Updated) ? info.Created : info.Updated);
        DateTextBlock.text = updated.ToString("D");
        TimeTextBlock.text = updated.ToString("T");

        VersionTextBlock.text = "Version " + (info.Version + 1);

        OKIcon.SetActive(this.AssetInfo.Status == "success");
        ProcessingIcon.SetActive(this.AssetInfo.Status.StartsWith("processing_"));
        QueuedIcon.SetActive(this.AssetInfo.Status.StartsWith("waiting"));
        ErrorIcon.SetActive(this.AssetInfo.Status.StartsWith("failed"));
    }

    public void UpdateThumb()
    {
        if (AssetInfo.IsEmpty())
        {
            textureLoader = null;
            return;
        }
        var texturePath = (MasterController.Instance.ServerManager.ServerBundleRoot + this.AssetInfo.Id + "/" + this.AssetInfo.Thumbnail).Replace(" ", "%20");


        if (ThumbImageObject.sprite == null || !ThumbnailCache.ContainsKey(texturePath))
        {
            StartCoroutine("UpdateThumbnail", texturePath);
        }
        else
        {
            ThumbImageObject.gameObject.SetActive(true);
            ThumbImageObject.sprite = ThumbnailCache[texturePath];
        }
    }
    public enum CardState
    {
        hide,
        show,
        valid,
        empty
    }

    private Animator animator;
    public void SetState(CardState state, bool playAnim = true)
    {
        if (animator == null)
        {
            animator = gameObject.GetComponent<Animator>();
        }

        animator.speed = playAnim ? 1 : 9999;
        switch (state)
        {
            case CardState.show:
                animator.Play("MenuBundleCardIntro");
                break;
            case CardState.hide:
                animator.Play("MenuBundleCardOutro");
                break;
        }
    }

    protected override void updateButton()
    {
        base.updateButton();
        if (Border != null)
        {
            Border.GetComponent<Renderer>().material.color = isOver ? BorderOverColor : BorderNormalColor;
        }
        if (Highlight != null)
        {
            Highlight.SetActive(isOver);
        }
    }

    WWW textureLoader = null;
    IEnumerator UpdateThumbnail(string texturePath)
    {
        if (textureLoader == null || textureLoader.error != null)
        {
            textureLoader = new WWW(texturePath);
        }

        Debug.Log("Updating Thumbnail:" + texturePath);

        yield return textureLoader;
        if (textureLoader.error == null)
        {
            var rect = new Rect(0, 0, textureLoader.texture.width, textureLoader.texture.height);
            ThumbImageObject.gameObject.SetActive(true);
            ThumbnailCache[texturePath] = Sprite.Create(textureLoader.texture, rect, new Vector2(0.5f, 0.5f), 100);
            ThumbImageObject.sprite = ThumbnailCache[texturePath];
        }
        else
        {
            ThumbImageObject.gameObject.SetActive(false);
            Debug.Log("textureLoader.error:" + textureLoader.error);
        }
    }
}
