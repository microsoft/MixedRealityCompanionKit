using System.Collections.ObjectModel;

namespace DesktopServerApp
{
    public class RealtimeServers : PropertyChangeBase
    {
        public ObservableCollection<ConnectionAndServer> Servers
        {
            get => _realtimeServers;
            set => base.SetProperty(ref _realtimeServers, value);
        }
        public int MaxServers { get; private set; }

        private ObservableCollection<ConnectionAndServer> _realtimeServers;

        public RealtimeServers(int maxServers)
        {
            MaxServers = maxServers;
            Servers = new ObservableCollection<ConnectionAndServer>();
        }
        public void AddServer(ConnectionAndServer connectionAndServer)
        {
            Servers.Add(connectionAndServer);
        }

        private void RemoveServer(ConnectionAndServer connectionAndServer)
        {
            Servers.Remove(connectionAndServer);
        }
        public ConnectionAndServer GetNextListeningServer()
        {
            foreach (var realtimeServer in _realtimeServers)
            {
                if (realtimeServer.ConnectionStatus == ConnectionStatus.Listening) return realtimeServer;
            }

            return null;
        }
    }
}
