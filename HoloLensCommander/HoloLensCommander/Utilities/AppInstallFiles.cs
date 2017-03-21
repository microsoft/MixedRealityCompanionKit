// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.Generic;
using Windows.Storage;

namespace HoloLensCommander
{
    /// <summary>
    /// Object encapsulating the file(s) required to install an application.
    /// </summary>
    public class AppInstallFiles
    {
        public StorageFile AppPackageFile
        { get; set; }

        public List<StorageFile> AppDependencyFiles
        { get; private set;  }

        public StorageFile AppCertificateFile
        { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="AppInstallFiles" /> class.
        /// </summary>
        public AppInstallFiles()
        {
            this.AppPackageFile = null;
            this.AppDependencyFiles = new List<StorageFile>();
            this.AppCertificateFile = null;
        }
    }
}
    