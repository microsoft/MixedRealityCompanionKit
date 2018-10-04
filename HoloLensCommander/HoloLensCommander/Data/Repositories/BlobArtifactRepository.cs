using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using HoloLensCommander.Models.Entities;
using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Blob;

namespace HoloLensCommander.Data.Repositories
{
    public interface IBlobArtifactRepository
    {
        Action<IArtifactEntity> OnGetEachArtifact { get; set; }

        Task<byte[]> DownloadArtifactAsync(string containerName, string blobName, long length);
        Task DownloadArtifactAsync(string containerName, string blobName, Stream stream);
        Task<IArtifactEntity[]> ListArtifactsAsync(string containerName);
        void Cancel();
    }

    public class BlobArtifactRepository : IBlobArtifactRepository
    {
        public Action<IArtifactEntity> OnGetEachArtifact { get; set; }
        private string blobStorageConnection;
        private ConcurrentDictionary<int, CancellationTokenSource> cancellationDictionary = new ConcurrentDictionary<int, CancellationTokenSource>();

        public BlobArtifactRepository(string blobStorageConnection)
        {
            this.blobStorageConnection = blobStorageConnection;
        }

        public async Task<byte[]> DownloadArtifactAsync(string containerName, string blobName, long length)
        {
            var storageClient = CloudStorageAccount.Parse(blobStorageConnection);
            var blobClient = storageClient.CreateCloudBlobClient();
            var container = blobClient.GetContainerReference(containerName);
            var blob = container.GetBlockBlobReference(blobName);
            var bytes = new byte[length];
            await blob.DownloadToByteArrayAsync(bytes, 0);
            return bytes;
        }

        public async Task DownloadArtifactAsync(string containerName, string blobName, Stream stream)
        {
            var storageClient = CloudStorageAccount.Parse(blobStorageConnection);
            var blobClient = storageClient.CreateCloudBlobClient();
            var container = blobClient.GetContainerReference(containerName);
            var blob = container.GetBlockBlobReference(blobName);
            await blob.DownloadToStreamAsync(stream);
        }

        public async Task<IArtifactEntity[]> ListArtifactsAsync(string containerName)
        {
            var storageClient = CloudStorageAccount.Parse(blobStorageConnection);
            var blobClient = storageClient.CreateCloudBlobClient();
            var container = blobClient.GetContainerReference(containerName);

            // project
            var artifactList = new List<IArtifactEntity>();
            var directories = await ListItemsAsync<CloudBlobDirectory>(container, null);
            foreach (var directory in directories)
            {
                // branch
                var branchArtifactList = new List<IBranchArtifactEntity>();
                var branches = await ListItemsAsync<CloudBlobDirectory>(container, directory.Prefix);
                await Task.WhenAll(branches.Select(async xs =>
                {
                    // blob
                    var artifactDetailList = new List<IArtifactDetailEntity>();
                    var details = await ListItemsAsync<CloudBlockBlob>(container, xs.Prefix);
                    foreach (var detail in details)
                    {
                        artifactDetailList.Add(new BlobArtifactDetailEntity(detail.Name, detail.Uri.Segments.Last(), detail.Uri, detail.Properties.Length, detail.Properties.ContentMD5, detail.Properties.LeaseState));
                    }

                    var branchName = xs.Prefix.Substring(directory.Prefix.Length, xs.Prefix.Length - directory.Prefix.Length - 1);
                    branchArtifactList.Add(new BlobBranchArtifactEntity(branchName, artifactDetailList.ToArray()));
                }));

                var projectName = directory.Prefix.Substring(0, directory.Prefix.Length - 1);
                var artifact = new BlobArtifactEntity(projectName, branchArtifactList.ToArray());
                artifactList.Add(artifact);
                OnGetEachArtifact?.Invoke(artifact);
            };

            return artifactList.ToArray();
        }

        public void Cancel()
        {
            foreach (var cts in cancellationDictionary)
            {
                cts.Value.Cancel();
            }
            cancellationDictionary.Clear();
        }

        /// <summary>
        /// T should be CloudBlockBlob or CloudBlobDirectory
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="container"></param>
        /// <param name="directoryName"></param>
        /// <param name="useFlatBlobListing"></param>
        /// <returns></returns>
        private async Task<List<T>> ListItemsAsync<T>(CloudBlobContainer container, string directoryName, bool useFlatBlobListing = false) where T : IListBlobItem
        {
            var list = new List<T>();
            BlobContinuationToken blobContinuationToken = null;
            do
            {
                using (var cts = new CancellationTokenSource())
                {
                    // for cancellation
                    var hash = cts.GetHashCode();
                    cancellationDictionary.TryAdd(hash, cts);

                    var results = await container.ListBlobsSegmentedAsync(directoryName, useFlatBlobListing, BlobListingDetails.None, 100, blobContinuationToken, null, null, cts.Token);
                    // Get the value of the continuation token returned by the listing call.
                    blobContinuationToken = results.ContinuationToken;
                    foreach (var item in results.Results)
                    {
                        list.Add((T)item);
                    }

                    // no need cancellation for this cts
                    cancellationDictionary.TryRemove(hash, out var _);
                }
            } while (blobContinuationToken != null);
            return list;
        }
    }
}
