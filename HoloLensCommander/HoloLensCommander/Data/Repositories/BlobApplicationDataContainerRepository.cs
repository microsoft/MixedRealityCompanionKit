using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage;

namespace SelectedTextSpeach.Data.Repositories
{
    public interface IApplicationDataContainerRepository
    {
        T Get<T>(string key);
        void Set<T>(string key, T value);
    }

    public class BlobApplicationDataContainerRepository : IApplicationDataContainerRepository
    {
        public string ContainerName { get; }

        private readonly ApplicationDataContainer root = ApplicationData.Current.LocalSettings;
        private readonly ApplicationDataContainer current;
        private object gate = new object();

        public BlobApplicationDataContainerRepository(string containerName)
        {
            ContainerName = containerName;
            current = root.CreateContainer(ContainerName, ApplicationDataCreateDisposition.Always);
        }

        public T Get<T>(string key)
        {
            lock (gate)
            {
                return (T)current.Values[key];
            }
        }

        public void Set<T>(string key, T value)
        {
            lock (gate)
            {
                current.Values[key] = value;
            }
        }
    }
}
