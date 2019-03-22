using System;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace BeaconLib
{
    /// <summary>
    /// Counterpart of the beacon, searches for beacons
    /// </summary>
    /// <remarks>
    /// The beacon list event will not be raised on your main thread!
    /// </remarks>
    public class Probe : IDisposable
    {
        /// <summary>
        /// Remove beacons older than this
        /// </summary>

        private Task task = null;
        private readonly object newBeaconLock = new object();

        private readonly UdpClient udp = new UdpClient();
        private bool running = false;
        //private BeaconLocation _newBeacon;


        public Probe(string beaconType)
        {
            udp.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);

            BeaconType = beaconType;
        
            udp.Client.Bind(new IPEndPoint(IPAddress.Any, 0));
#if (!WINDOWS_UWP)
            try
            {
                udp.AllowNatTraversal(true);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Error switching on NAT traversal: " + ex.Message);
            }
#endif
        }


        private BeaconLocation ResponseReceived(UdpReceiveResult ar)
        {
            var bytes = ar.Buffer;
            var remote = ar.RemoteEndPoint;
            Debug.WriteLine($"P: Received response from {remote.Address}");
            var typeBytes = Beacon.Encode(BeaconType).ToList();
            Debug.WriteLine(string.Join(", ", typeBytes.Select(_ => (char)_)));
            if (Beacon.HasPrefix(bytes, typeBytes))
            {
                try
                {
                    var portBytes = bytes.Skip(typeBytes.Count()).Take(2).ToArray();
                    var port = (ushort)IPAddress.NetworkToHostOrder((short)BitConverter.ToUInt16(portBytes, 0));
                    var payload = Beacon.Decode(bytes.Skip(typeBytes.Count() + 2));
                    Debug.WriteLine($"P: New beacon {remote.Address},{port}");
                    var newBeacon = new BeaconLocation(new IPEndPoint(remote.Address, port), payload, DateTime.Now);
                    return newBeacon;
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex);
                    
                }
            }
            return null;
        }

        public string BeaconType { get; private set; }

        private async Task BroadcastProbeAsync()
        {
            Debug.WriteLine($"P: Broadcasting probe with discovery port {Beacon.DiscoveryPort}");
            var probe = Beacon.Encode(BeaconType).ToArray();
            await udp.SendAsync(probe, probe.Length, new IPEndPoint(IPAddress.Broadcast, Beacon.DiscoveryPort));
        }

        public async Task<BeaconLocation> GetBeacon(int timeOut)
        {
            BeaconLocation newBeacon = null;

            try
            {
                await BroadcastProbeAsync();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }

            await Task.Delay(timeOut);

            try
            {
                var result = await udp.ReceiveAsync();
                newBeacon  = ResponseReceived(result);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }

            return newBeacon;
        }

        public void Dispose()
        {
        }
    }
}
