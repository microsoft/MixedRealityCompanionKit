// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.IO;
using System.Threading;
using System.Xml;

class Logger
{

    private static readonly object LockObj = new object();
    private static XmlDocument _doc = new XmlDocument();
    static Logger()
    {
        if (File.Exists("log_last.xml"))
            File.Delete("log_last.xml");

        if (File.Exists("log.xml"))
            File.Move("log.xml", "log_last.xml");

        var root = _doc.CreateElement("log");
        _doc.AppendChild(root);

    }
    public static void Log(string message)
    {
        lock (LockObj)
        {
            var el = (XmlElement)_doc.DocumentElement.AppendChild(_doc.CreateElement("log_item"));
            el.AppendChild(_doc.CreateElement("DateTime")).InnerText = DateTime.Now.ToString();
            el.AppendChild(_doc.CreateElement("Message")).InnerText = message;
            _doc.Save("log.xml");
        }
    }
}