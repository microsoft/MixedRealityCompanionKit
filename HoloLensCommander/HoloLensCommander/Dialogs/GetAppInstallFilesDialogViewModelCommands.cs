// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.Storage;
using Windows.Storage.AccessCache;
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
        /// Command used to allow the user to browse for the folder that contains installation files.
        /// </summary>
        public ICommand BrowseForParentFolderCommand
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
                foreach (StorageFile file in files)
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
        /// Implementation of the browse for parents folder command.
        /// </summary>
        /// <returns>Task object used for tracking method completion.</returns>
        private async Task BrowseForParentFolderAsync()
        {
            FolderPicker folderPicker = new FolderPicker();
            folderPicker.ViewMode = PickerViewMode.List;
            folderPicker.SuggestedStartLocation = PickerLocationId.ComputerFolder;
            folderPicker.CommitButtonText = "Select";
            folderPicker.FileTypeFilter.Add("*");

            StorageFolder folder = await folderPicker.PickSingleFolderAsync();
            if (folder != null)
            {
                string appxFolderToken = StorageApplicationPermissions.FutureAccessList.Add(folder);
                await AutoAddInstallFiles(appxFolderToken);
            }
        }

        public async Task AutoAddInstallFiles(string appxFolderToken)
        {
            StorageFolder appPackageFolder = await StorageApplicationPermissions.FutureAccessList.GetFolderAsync(appxFolderToken);
            IReadOnlyList<StorageFile> files = await appPackageFolder.GetFilesAsync();

            Func<StorageFile, bool> appxFilter = f => f.FileType.Equals(".appx") || f.FileType.Equals(".appxbundle");
            StorageFile appxFile = files.FirstOrDefault(appxFilter);
            if (appxFile == null)
            {
                return;
            }

            this.AppPackageFile = appxFile;

            StorageFile certFile = files.FirstOrDefault(f => f.FileType.Equals(".cer"));
            if (certFile != null)
            {
                this.AppCertificateFile = certFile;
            }

            DependencyFiles.Clear();
            DependencyFileNames.Clear();

            StorageFolder dependenciesFolder = await appPackageFolder.GetFolderAsync(@"Dependencies\x86");

            if (string.Equals(this.ProcessorArchitectureValue, "x64"))
            {
                dependenciesFolder = await appPackageFolder.GetFolderAsync(@"Dependencies\x64");
            }

            IReadOnlyList<StorageFile> dependencies = await dependenciesFolder.GetFilesAsync();
            foreach (StorageFile dependency in dependencies.Where(appxFilter))
            {
                this.DependencyFiles.Add(dependency.Path, dependency);
                this.DependencyFileNames.Add(dependency.Path);
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
