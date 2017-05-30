using System.Windows.Input;
using Windows.UI.Xaml;

namespace HoloLensCommander
{
    /// <summary>
    /// The view model for the ConnectToDeviceDialog object.
    /// </summary>
    public partial class ConnectToDeviceDialogViewModel
    {
        public ICommand ShowHideCredentialsCommand
        { get; private set; }

        private void ShowHideCredentials()
        {
            if (this.ShowCredentials == Visibility.Collapsed)
            {
                this.ShowHideCredentialsButtonLabel = CollapseButtonLabel;
                this.ShowCredentials = Visibility.Visible;
            }
            else
            {
                this.ShowHideCredentialsButtonLabel = ExpandButtonLabel;
                this.ShowCredentials = Visibility.Collapsed;
            }
        }

        public ICommand ShowHideNetworkSettingsCommand
        { get; private set; }

        public void ShowHideNetworkSettings()
        {
            if (this.ShowNetworkSettings == Visibility.Collapsed)
            {
                this.ShowHideNetworkSettingsButtonLabel = CollapseButtonLabel;
                this.ShowNetworkSettings = Visibility.Visible;
            }
            else
            {
                this.ShowHideNetworkSettingsButtonLabel = ExpandButtonLabel;
                this.ShowNetworkSettings = Visibility.Collapsed;
            }
        }
    }
}
