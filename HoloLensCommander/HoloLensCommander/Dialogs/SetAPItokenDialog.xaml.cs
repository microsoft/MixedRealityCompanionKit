﻿using Windows.UI.Xaml.Controls;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace HoloLensCommander
{
    public sealed partial class SetAPItokenDialog : ContentDialog
    {
        private UserToken apiToken;

        public SetAPItokenDialog(UserToken APItoken)
        {
            this.apiToken = APItoken;
            this.DataContext = new SetAPItokenDialogViewModel(APItoken);
            this.InitializeComponent();
        }

        private void ContentDialog_PrimaryButtonClick(ContentDialog sender, ContentDialogButtonClickEventArgs args)
        {
            ((SetAPItokenDialogViewModel)this.DataContext).UpdateAPItoken(this.apiToken);
        }

        private void ContentDialog_SecondaryButtonClick(ContentDialog sender, ContentDialogButtonClickEventArgs args)
        {
        }
    }
}
