using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Blob;

namespace SelectedTextSpeach.Data.Repositories
{
    public interface IBlobSasRepository
    {
        Task<string> RequestSasUrlAsync(string containerName, string blobName);
    }

    public class BlobSasRepository : IBlobSasRepository
    {
        public static TimeSpan Begin { get; private set; } = TimeSpan.FromMinutes(-10);
        public static TimeSpan Expiry { get; private set; }

        private string storageAccountConnectionString;

        public BlobSasRepository(string storageAccountConnectionString, TimeSpan expiry)
        {
            this.storageAccountConnectionString = storageAccountConnectionString;
            Expiry = expiry;
        }

        public async Task<string> RequestSasUrlAsync(string container, string blob)
        {
            var storageAccount = CloudStorageAccount.Parse(storageAccountConnectionString);
            var client = storageAccount.CreateCloudBlobClient();
            var permission = SharedAccessBlobPermissions.Read;
            return await GenerateBlobSasTokenAsync(client.GetContainerReference(container), blob, permission);
        }

        private async Task<string> GenerateBlobSasTokenAsync(CloudBlobContainer container, string blobName, SharedAccessBlobPermissions permission, string policyName = "")
        {
            string sasBlobToken;
            await container.CreateIfNotExistsAsync();
            var blob = container.GetBlockBlobReference(blobName);
            if (string.IsNullOrWhiteSpace(policyName))
            {
                var policy = CreateAdhocPolicy(permission);
                sasBlobToken = blob.GetSharedAccessSignature(policy);
            }
            else
            {
                sasBlobToken = blob.GetSharedAccessSignature(null, policyName);
            }
            return sasBlobToken;
        }

        private SharedAccessBlobPolicy CreateAdhocPolicy(SharedAccessBlobPermissions permission)
        {
            var policy = new SharedAccessBlobPolicy()
            {
                SharedAccessStartTime = DateTime.UtcNow + Begin,
                SharedAccessExpiryTime = DateTime.UtcNow + Expiry,
                Permissions = permission,
            };
            return policy;
        }
    }
}
