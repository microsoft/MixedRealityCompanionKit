// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SinCurve : MonoBehaviour
{
    public float speed = 2.0f;
    public float height = 4.0f;

    Vector3 StartPosition;
    float startTime = 0;

    private void Awake()
    {
        StartPosition = gameObject.transform.position;
    }

    void OnEnable()
    {
        startTime = Time.time;
    }

    private void OnDisable()
    {
        gameObject.transform.position = StartPosition;
    }

    void FixedUpdate()
    {
        gameObject.transform.position = new Vector3(
            StartPosition.x,
            StartPosition.y + (Mathf.Sin(((Time.time - startTime) * Time.fixedDeltaTime) * 60 / speed) / height),
            StartPosition.z);
    }
}
