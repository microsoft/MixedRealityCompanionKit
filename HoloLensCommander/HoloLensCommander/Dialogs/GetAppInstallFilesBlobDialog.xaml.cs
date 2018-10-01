using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// 空白ページの項目テンプレートについては、https://go.microsoft.com/fwlink/?LinkId=234238 を参照してください

namespace HoloLensCommander
{
    /// <summary>
    /// それ自体で使用できる空白ページまたはフレーム内に移動できる空白ページ。
    /// </summary>
    public sealed partial class GetAppInstallFilesBlobDialog : ContentDialog
    {
        /// <summary>
        /// Object containing the file(s) required to install an application.
        /// </summary>
        private AppInstallFiles appInstallFiles;

        public GetAppInstallFilesBlobDialog(AppInstallFiles installFiles)
        {
            this.appInstallFiles = installFiles;
            this.DataContext = new GetAppInstallFilesDialogViewModel();
            this.InitializeComponent();
        }

        /// <summary>
        /// Handle click event for the ok button.
        /// </summary>
        /// <param name="sender">The object sending the event.</param>
        /// <param name="args">Event arguments.</param>
        private void ContentDialog_OkClick(
            ContentDialog sender,
            ContentDialogButtonClickEventArgs args)
        {
            // Return the user's selections.
            ((GetAppInstallFilesDialogViewModel)this.DataContext).UpdateUserData(this.appInstallFiles);
        }

        /// <summary>
        /// Handle check event for the dependencies radio buttons.
        /// </summary>
        /// <param name="sender">The object sending the event.</param>
        /// <param name="e">Event arguments.</param>
        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            ((GetAppInstallFilesDialogViewModel)this.DataContext).ProcessorArchitectureValue = ((RadioButton)sender).Content.ToString();
        }
    }
}
