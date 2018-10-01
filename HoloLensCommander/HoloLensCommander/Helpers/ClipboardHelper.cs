using System;
using System.Threading.Tasks;
using Windows.ApplicationModel.DataTransfer;

namespace HoloLensCommander
{
    public static class ClipboardHelper
    {
        private static readonly DataPackage dataPackage = new DataPackage();

        public static void CopyToClipboard(string content)
        {
            dataPackage.RequestedOperation = DataPackageOperation.Copy;
            dataPackage.SetText(content);
            Clipboard.SetContent(dataPackage);
        }

        public static async Task<string> GetContentFromClipboardAsync()
        {
            var dataPackageView = Clipboard.GetContent();
            if (dataPackageView.Contains(StandardDataFormats.Text))
            {
                var text = await dataPackageView.GetTextAsync();
                return text;
            }
            else
            {
                return "";
            }
        }
    }
}
