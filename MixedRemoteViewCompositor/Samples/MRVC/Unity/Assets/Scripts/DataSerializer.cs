// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.IO;
using System.Runtime.Serialization;
using System.Text;
using System.Xml;

[Serializable]
public class StateDataSerializer<T>
{
    public static byte[] ToByteArray(T state)
    {
        byte[] buffer = null;

        using (MemoryStream memoryStream = new MemoryStream())
        {
            DataContractSerializer serializer = new DataContractSerializer(state.GetType());
            serializer.WriteObject(memoryStream, state);
            buffer = memoryStream.ToArray();
        }

        return buffer;
    }

    public static void FromByteArray(byte[] buffer, ref T state)
    {
        using (System.IO.MemoryStream stream = new System.IO.MemoryStream(buffer))
        {
            XmlDictionaryReader reader = XmlDictionaryReader.CreateTextReader(stream, Encoding.UTF8, new XmlDictionaryReaderQuotas(), null);
            DataContractSerializer serializer = new DataContractSerializer(typeof(T));
            state = (T)serializer.ReadObject(reader);
        }
    }
}
