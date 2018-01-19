// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using UnityEngine;

public class Spin : MonoBehaviour
{
    //TODO: as vector?
    public float X = 0;
    public float Y = 0;
    public float Z = 0;

    void Update()
    {
        if (X != 0)
        {
            this.transform.Rotate(Vector3.right * Time.deltaTime * X);
        }
        if (Y != 0)
        {
            this.transform.Rotate(Vector3.up * Time.deltaTime * Y);
        }
        if (Z != 0)
        {
            this.transform.Rotate(Vector3.forward * Time.deltaTime * Z);
        }
    }
}
