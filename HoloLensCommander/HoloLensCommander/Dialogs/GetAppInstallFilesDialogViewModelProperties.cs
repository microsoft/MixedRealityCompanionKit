// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.Collections.Generic;
using System.Collections.ObjectModel;
using Windows.Storage;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the GetAppInstallFilesDialog fsobject.
    /// </summary>
    partial class GetAppInstallFilesDialogViewModel
    {
        /// <summary>
        /// Gets or sets the application certificate file.
        /// </summary>
        private StorageFile appCertificateFile = null;
        public StorageFile AppCertificateFile
        {
            get
            {
                return this.appCertificateFile;
            }

            private set
            {
                this.appCertificateFile = value;
                this.AppCertificateFileName = (this.appCertificateFile != null) ?
                    this.appCertificateFile.Path : "";
                this.NotifyPropertyChanged("AppCertificateFileName");
            }
        }

        /// <summary>
        /// Gets or sets the name of the application certificate file.
        /// </summary>
        public string AppCertificateFileName
        { get; private set; }

        /// <summary>
        /// Gets or sets the application package file.
        /// </summary>
        private StorageFile appPackageFile = null;
        public StorageFile AppPackageFile
        {
            get
            {
                return this.appPackageFile;
            }

            private set
            {
                this.appPackageFile = value;
                this.AppPackageFileName = (this.appPackageFile != null) ? 
                    this.appPackageFile.Path : "";
                this.NotifyPropertyChanged("AppPackageFileName");
            }
        }

        /// <summary>
        /// Gets or sets the name of the application package file.
        /// </summary>
        public string AppPackageFileName
        { get; private set; }

        /// <summary>
        /// Gets a value indicating whether or not the user has selected a dependency file to remove.
        /// </summary>
        private bool canRemoveDependencyFile = false;
        public bool CanRemoveDependencyFile
        {
            get
            {
                return this.canRemoveDependencyFile;
            }

            private set
            {
                if (this.canRemoveDependencyFile != value)
                {
                    this.canRemoveDependencyFile = value;
                    this.NotifyPropertyChanged("CanRemoveDependencyFile");
                }
            }
        }

        /// <summary>
        /// Gets the collection of application dependency files.
        /// </summary>
        private Dictionary<string, StorageFile> DependencyFiles;

        /// <summary>
        /// Gets the collection of application dependency file names.
        /// </summary>
        public ObservableCollection<string> DependencyFileNames
        { get; private set; }

        /// <summary>
        /// Gets the selected processor architecture for installing dependency files
        /// </summary>
        private string processorArchitectureValue;
        public string ProcessorArchitectureValue
        {
            get
            {
                return this.processorArchitectureValue;
            }

            set
            {
                this.processorArchitectureValue = value;
                this.NotifyPropertyChanged("ProcessorArchitectureValue");
            }
        }

        /// <summary>
        /// Gets the selected file in the dependency files list.
        /// </summary>
        private string selectedDependencyFile = null;
        public string SelectedDependencyFile
        {
            get
            {
                return this.selectedDependencyFile;
            }

            set
            {
                if (this.selectedDependencyFile != value)
                {
                    this.selectedDependencyFile = value;
                    this.NotifyPropertyChanged("SelectedDependencyFile");
                    this.UpdateCanRemoveDependencyFile();
                }
            }
        }

        /// <summary>
        /// Updates the value of the CanRemoveDependencyFile property.
        /// </summary>
        private void UpdateCanRemoveDependencyFile()
        {
            if ((this.DependencyFileNames.Count > 0) &&
                !string.IsNullOrWhiteSpace(this.SelectedDependencyFile))
            {
                this.CanRemoveDependencyFile = true;
            }
            else
            {
                this.CanRemoveDependencyFile = false;
            }
        }
    }
}
