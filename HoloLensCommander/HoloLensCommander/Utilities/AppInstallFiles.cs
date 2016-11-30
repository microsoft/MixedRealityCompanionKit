// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.Generic;

namespace HoloLensCommander
{
    /// <summary>
    /// Object encapsulating the file(s) required to install an application.
    /// </summary>
    public class AppInstallFiles
    {
        /// <summary>
        /// Gets or sets the full path to the application package file.
        /// </summary>
        public string AppPackageFileName
        {
            get; set;
        }

        /// <summary>
        /// List of full paths to required dependency files.
        /// </summary>
        public List<string> AppDependencyFileNames
        {
            get; private set;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AppInstallFiles" /> class.
        /// </summary>
        public AppInstallFiles()
        {
            this.AppPackageFileName = null;
            this.AppDependencyFileNames = new List<string>();
        }
    }
}
