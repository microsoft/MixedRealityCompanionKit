// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System.ComponentModel;
using System.Threading.Tasks;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the GetAppInstallFilesDialog fsobject.
    /// </summary>
    partial class GetAppInstallFilesDialogViewModel : INotifyPropertyChanged
    {
        /// <summary>
        /// Event that is notified when a property value has changed.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="GetAppInstallFilesDialogViewModel" /> class.
        /// </summary>
        public GetAppInstallFilesDialogViewModel()
        {
            this.RegisterCommands();
        }

        /// <summary>
        /// Update's the user selected install files data.
        /// </summary>
        /// <param name="installFiles">The application install files.</param>
        internal void UpdateUserData(AppInstallFiles installFiles)
        {
            installFiles.AppPackageFileName = AppPackageFile;
        }

        /// <summary>
        /// Sends the PropertyChanged events to registered handlers.
        /// </summary>
        /// <param name="propertyName">The name of property that has changed.</param>
        private void NotifyPropertyChanged(string propertyName)
        {
            this.PropertyChanged?.Invoke(
                this, 
                new PropertyChangedEventArgs(propertyName));
        }

        /// <summary>
        /// Registers the commands supported by this object.
        /// </summary>
        private void RegisterCommands()
        {
            Task t;

            this.BrowseForAppPackageCommand = new Command(
                (parameter) =>
                {
                    // Assigning the return value of BrowseForAppPackagesAsync to a Task object to avoid 
                    // warning 4014 (call is not awaited).
                    t = this.BrowseForAppPackageAsync();
                });
        }
    }
}
