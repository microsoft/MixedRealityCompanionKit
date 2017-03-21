// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.Storage;
using Windows.Storage.Pickers;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the GetAppInstallFilesDialog fsobject.
    /// </summary>
    partial class GetAppInstallFilesDialogViewModel
    {
        /// <summary>
        /// Command used to allow the user to browse for a dependency file.
        /// </summary>
        public ICommand AddDependencyFileCommand
        { get; private set; }    

        /// <summary>
        /// Command used to allow the user to browse for the application certificate file.
        /// </summary>
        public ICommand BrowseForAppCertFileCommand
        { get; private set; }    

        /// <summary>
        /// Command used to allow the user to browse for the application package file.
        /// </summary>
        public ICommand BrowseForAppPackageCommand
        { get; private set; }    

        /// <summary>
        /// Command used to allow the user to remove a dependency file.
        /// </summary>
        public ICommand RemoveDependencyFileCommand
        { get; private set; }    

        /// <summary>
        /// Implementation of the add dependency file command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task AddAppDependencyFileAsync()
        {
            FileOpenPicker filePicker = new FileOpenPicker();
            filePicker.FileTypeFilter.Add(".appx");
            filePicker.CommitButtonText = "Select";

            IReadOnlyList<StorageFile> files = await filePicker.PickMultipleFilesAsync();
            if (files.Count != 0)
            {
                foreach(StorageFile file in files)
                {
                    this.DependencyFiles.Add(file.Path, file);
                    this.DependencyFileNames.Add(file.Path);
                }
            }
        }

        /// <summary>
        /// Implementation of the browse for app certificate file command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task BrowseForAppCertFileAsync()
        {
            FileOpenPicker filePicker = new FileOpenPicker();
            filePicker.FileTypeFilter.Add(".cer");
            filePicker.CommitButtonText = "Select";

            StorageFile file = await filePicker.PickSingleFileAsync();
            if (file != null)
            {
                this.AppCertificateFile = file;
            }
        }

        /// <summary>
        /// Implementation of the browse for app package command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task BrowseForAppPackageAsync()
        {
            FileOpenPicker filePicker = new FileOpenPicker();
            filePicker.FileTypeFilter.Add(".appx");
            filePicker.FileTypeFilter.Add(".appxbundle");
            filePicker.CommitButtonText = "Select";

            StorageFile file = await filePicker.PickSingleFileAsync();
            if (file != null)
            {
                this.AppPackageFile = file;
            }
        }

        /// <summary>
        /// Implementation of the remove dependency file command.
        /// </summary>
        private void RemoveAppDependencyFile()
        {
            string dependencyFile = this.SelectedDependencyFile;
            DependencyFiles.Remove(dependencyFile);
            DependencyFileNames.Remove(dependencyFile);
        }
    }
}
