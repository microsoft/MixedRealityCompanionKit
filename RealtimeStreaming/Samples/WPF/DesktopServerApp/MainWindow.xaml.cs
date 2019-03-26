using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;

namespace DesktopServerApp
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public ServerManager ServerManager
        {
            get => _serverManager;
            set => this.SetProperty(ref this._serverManager, value);
        }
        private ServerManager _serverManager;

        public MainWindow()
        {
            InitializeComponent();

            this.Loaded += this.OnLoaded;
        }
        void OnLoaded(object sender, RoutedEventArgs e)
        {
            this.DataContext = this;
            this.ServerManager = new ServerManager(this.Dispatcher);
            this.ServerManager.InitializeGrabberAndBuffers();
        }
        void OnListenButtonClick(object sender, RoutedEventArgs e)
        {
             _serverManager.StartListening();
        }
        void FirePropertyChanged(
            [CallerMemberName] string propertyName = null)
        {
            this.PropertyChanged?.Invoke(
                this,
                new PropertyChangedEventArgs(propertyName)
            );
        }
        bool SetProperty<T>(
            ref T storage,
            T value,
            [CallerMemberName] string propertyName = null)
        {
            if (object.Equals(storage, value)) return false;

            storage = value;
            this.FirePropertyChanged(propertyName);
            return true;
        }
    }
}