// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the GetAppInstallFilesDialog fsobject.
    /// </summary>
    partial class GetAppInstallFilesDialogViewModel
    {
        /// <summary>
        /// Gets or sets the full path to the application package file.
        /// </summary>
        private string appPackageFile = "";
        public string AppPackageFile
        {
            get
            {
                return this.appPackageFile;
            }

            set
            {
                if (this.appPackageFile != value)
                {
                    this.appPackageFile = value;
                    this.NotifyPropertyChanged("AppPackageFile");
                }
            }
        }
    }
}
