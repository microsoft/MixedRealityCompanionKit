// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ListManager : MenuBase
{
    public delegate void ListUpdatedDelegate();
    public event ListUpdatedDelegate ListUpdated;
    public GameObject DataCardPrefab;
    public List<Transform> CardPositions;
    public List<Transform> ChromeObjects;
    public Clickable LeftArrow;
    public Clickable RightArrow;
    private List<MenuBundleCard> cards = new List<MenuBundleCard>();

    private DateTime lastUpdate = DateTime.MinValue;
    private AssetInfoList currentAssetInfoList = new AssetInfoList();

    private int currentPage = 0;
    private int totalPages = 1;
    private int itemsPerPage = 0;

    void Start()
    {
        itemsPerPage = CardPositions.Count;
        for (var i = 0; i < CardPositions.Count; ++i)
        {
            var dataCard = Instantiate<GameObject>(DataCardPrefab).GetComponent<MenuBundleCard>();
            dataCard.SetInfo(new AssetInfo());
            dataCard.transform.SetParent(CardPositions[i], false);
            dataCard.ButtonClicked += DataCard_ButtonClicked;
            cards.Add(dataCard);
        }
        LeftArrow.ButtonClicked += LeftArrow_ButtonClicked;
        RightArrow.ButtonClicked += RightArrow_ButtonClicked;
    }

    private void RightArrow_ButtonClicked(Clickable source)
    {
        SetPage(currentPage + 1);
    }

    private void LeftArrow_ButtonClicked(Clickable source)
    {
        SetPage(currentPage - 1);
    }

    void Update()
    {
        if ((DateTime.Now - lastUpdate).TotalMilliseconds > 15000)
        {
            StartCoroutine(UpdateList(MasterController.Instance.ServerManager.DataUrl, false));
        }
    }

    public AssetInfo GetAssetInfoByID(string id)
    {
        if (string.IsNullOrEmpty(id))
        {
            return new AssetInfo();
        }

        for (var i = 0; i < currentAssetInfoList.items.Length; ++i)
        {
            if (currentAssetInfoList.items[i].Id == id)
            {
                return currentAssetInfoList.items[i];
            }
        }
        return new AssetInfo();
    }

    IEnumerator UpdateList(string sourceUrl, bool clearList = false)
    {
        if (clearList)
        {
            currentAssetInfoList.items = new AssetInfo[0];
            totalPages = 0;
            SetPage(0);
            MasterController.Instance.StageManager.ClearStage();
        }
        lastUpdate = DateTime.Now;
        WWW www = new WWW(sourceUrl.Replace(" ", "%20"));
        yield return www;

        if (www.error == null)
        {
            currentAssetInfoList = AssetInfoList.CreateFromJSON(www.text);

            totalPages = currentAssetInfoList.items.Length / itemsPerPage + 1;

            SetPage(currentPage);

            if (ListUpdated != null)
            {
                ListUpdated();
            }
        }
        else
        {
            Debug.LogError("WWW error: " + www.error);
        }
    }

    private void SetPage(int newPage)
    {
        StartCoroutine("DoSetPage", newPage);
    }

    private IEnumerator DoSetPage(int newPage)
    {
        newPage = Math.Max(0, Math.Min(newPage, totalPages - 1));
        var changedPage = (newPage != currentPage);
        currentPage = newPage;
        if (changedPage)
        {
            ToggleItems(false);
            yield return new WaitForSeconds(.5f);
        }

        for (var i = 0; i < cards.Count; ++i)
        {
            var dataIndex = (itemsPerPage * currentPage) + i;
            AssetInfo assetInfo = new AssetInfo();
            if (dataIndex < currentAssetInfoList.items.Length)
            {
                assetInfo = currentAssetInfoList.items[dataIndex];
            }
            cards[i].SetInfo(assetInfo);
            cards[i].UpdateThumb();
        }

        if (changedPage)
        {
            ToggleItems(true);
        }
    }

    public override void HideMenu()
    {
        if (!isVisible)
        {
            return;
        }

        base.HideMenu();
        ToggleItems(false);
    }

    public override void ShowMenu()
    {
        if (isVisible)
        {
            return;
        }
        base.ShowMenu();
        ToggleItems(true);
    }

    private void ToggleItems(bool show)
    {
        foreach (var go in ChromeObjects)
        {
            go.gameObject.SetActive(show);
        }

        for (var i = 0; i < cards.Count; ++i)
        {
            StartCoroutine(ShowCard(cards[i], show, i * .033f));
        }
    }

    private IEnumerator ShowCard(MenuBundleCard dataCard, bool show, float delay = 0)
    {
        if (delay > 0)
        {
            yield return new WaitForSeconds(delay);
        }

        dataCard.SetState(show ? MenuBundleCard.CardState.show : MenuBundleCard.CardState.hide);
    }

    private void DataCard_ButtonClicked(Clickable source)
    {
        var menuBundleCard = source as MenuBundleCard;
        Debug.Log("Clicked: " + menuBundleCard.AssetInfo.Name);
        MasterController.Instance.LoadBundle(menuBundleCard.AssetInfo);
    }

    public override void UpdateMenu()
    {
        base.UpdateMenu();
        StartCoroutine(UpdateList(MasterController.Instance.ServerManager.DataUrl, true));
    }
}
