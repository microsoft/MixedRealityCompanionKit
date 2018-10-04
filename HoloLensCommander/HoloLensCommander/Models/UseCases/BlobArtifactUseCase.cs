using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Reactive.Subjects;
using System.Threading.Tasks;
using Microsoft.WindowsAzure.Storage;
using Reactive.Bindings;
using HoloLensCommander.Data.Repositories;
using HoloLensCommander.Models.Entities;
using Windows.System;
using Windows.Storage;
using System.IO;
using System.IO.Compression;
using Windows.Storage.AccessCache;

namespace HoloLensCommander.Models.UseCases
{
    public interface IBlobArtifactUseCase
    {
        ReadOnlyReactivePropertySlim<IArtifactEntity> Artifacts { get; }
        ReadOnlyReactivePropertySlim<string> RequestFailedMessage { get; }
        ReadOnlyReactivePropertySlim<string> DownloadStatus { get; }
        ReadOnlyReactivePropertySlim<string> DownloadPath { get; }
        ReactiveProperty<string> DownloadFolderAccessToken { get; }

        void CancelRequest();
        Task RequestHoloLensPackagesAsync(string blobConnectionString, string containerName);
        Task DownloadHoloLensPackagesAsync(string blobConnectionString, string containerName, string blobName, long length, string fileName);
        Task OpenFolderAsync();
        Task OpenDownloadFolderAsync();
        IArtifactEntity[] GetArtifactCache();
        IBranchArtifactEntity[] GetArtifactCache(string projectName);
        IArtifactDetailEntity[] GetArtifactCache(string projectName, string branchNam);
    }

    public class BlobArtifactUseCase : IBlobArtifactUseCase
    {
        public ReadOnlyReactivePropertySlim<IArtifactEntity> Artifacts => blobArtifactSubject.ToReadOnlyReactivePropertySlim();
        private Subject<IArtifactEntity> blobArtifactSubject = new Subject<IArtifactEntity>();
        public ReadOnlyReactivePropertySlim<string> RequestFailedMessage => blobArtifactFailedSubject.ToReadOnlyReactivePropertySlim();
        private Subject<string> blobArtifactFailedSubject = new Subject<string>();
        public ReadOnlyReactivePropertySlim<string> DownloadStatus => downloadStatusSubject.ToReadOnlyReactivePropertySlim();
        private Subject<string> downloadStatusSubject = new Subject<string>();
        public ReadOnlyReactivePropertySlim<string> DownloadPath => downloadPath.ToReadOnlyReactivePropertySlim();
        private Subject<string> downloadPath = new Subject<string>();

        public ReactiveProperty<string> DownloadFolderAccessToken { get; } = new ReactiveProperty<string>();
        private StorageFolder root;
        private string downloadDirectoryName;

        private ConcurrentBag<IArtifactEntity> caches = null;
        private IBlobArtifactRepository repository = null;

        public BlobArtifactUseCase(StorageFolder root, string downloadDirectoryName)
        {
            // ApplicationData.Current.LocalFolder
            // ApplicationData.Current.TemporaryFolder
            this.root = root;
            this.downloadDirectoryName = downloadDirectoryName;
        }

        public void CancelRequest()
        {
            repository?.Cancel();
        }

        public async Task DownloadHoloLensPackagesAsync(string blobConnectionString, string containerName, string blobName, long length, string fileName)
        {
            // await DownloadAsFileAsync(blobConnectionString, containerName, blobName, length, fileName);
            await DownloadAsStreamAsync(blobConnectionString, containerName, blobName, length, fileName);
        }

        private async Task DownloadAsFileAsync(string blobConnectionString, string containerName, string blobName, long length, string fileName)
        {
            repository = new BlobArtifactRepository(blobConnectionString);

            // download
            downloadStatusSubject.OnNext("Start downloading.");
            var result = await repository.DownloadArtifactAsync(containerName, blobName, length);

            // prepare
            var folder = await ReadyFolderAsync(root, downloadDirectoryName);
            var file = await folder.CreateFileAsync(fileName, CreationCollisionOption.GenerateUniqueName);

            // write file
            downloadStatusSubject.OnNext("Write to file.");
            await FileIO.WriteBytesAsync(file, result);

            // unzip
            downloadStatusSubject.OnNext("Begin unzip.");
            var parent = Path.GetFileNameWithoutExtension(file.Path);
            var extractFolder = await ReadyFolderAsync(folder, parent);
            await UnzipAsync(file, extractFolder);

            // clean up
            downloadStatusSubject.OnNext("clean up.");
            await file.DeleteAsync(StorageDeleteOption.Default);

            // notification
            downloadStatusSubject.OnNext("Complete.");
            downloadPath.OnNext(extractFolder.Path);
        }

        private async Task DownloadAsStreamAsync(string blobConnectionString, string containerName, string blobName, long length, string fileName)
        {
            repository = new BlobArtifactRepository(blobConnectionString);

            var bytes = new byte[length];
            using (var stream = new MemoryStream(bytes))
            {
                // download
                downloadStatusSubject.OnNext("Start downloading.");
                await repository.DownloadArtifactAsync(containerName, blobName, stream);

                // prepare
                var folder = await ReadyFolderAsync(root, downloadDirectoryName);
                var parent = Path.GetFileNameWithoutExtension(fileName);
                var extractFolder = await ReadyFolderAsync(folder, parent);
                var accessToken = GetFolderToken(extractFolder);
                var permittedExtractFoler = await ReadyFolderAsync(accessToken);

                // unzip
                downloadStatusSubject.OnNext("Begin unzip.");
                await UnzipAsync<MemoryStream>(stream, permittedExtractFoler);

                // notification
                downloadStatusSubject.OnNext("Complete.");
                downloadPath.OnNext(permittedExtractFoler.Path);
                DownloadFolderAccessToken.Value = accessToken;
            }
            bytes = null;
        }

        private async Task UnzipAsync<T>(T stream, StorageFolder extractFolder) where T : Stream
        {
            // unzip with long name validation
            using (var zip = new ZipArchive(stream, ZipArchiveMode.Read))
            {
                foreach (var entry in zip.Entries)
                {
                    var folders = entry.FullName.Split('/', StringSplitOptions.RemoveEmptyEntries);
                    var itemPath = entry.Name;
                    // entry.name == "" means it's directory.
                    if (string.IsNullOrWhiteSpace(itemPath))
                    {
                        // create folder
                        if (folders.Last() == "Dependencies")
                        {
                            await ReadyFolderAsync(extractFolder, "Dependencies");
                        }
                        else if (folders.TakeLast(2).SequenceEqual(new[] { "Dependencies", "x86" }))
                        {
                            await ReadyFolderAsync(extractFolder, Path.Combine("Dependencies", "x86"));
                        }
                        else if (folders.TakeLast(2).SequenceEqual(new[] { "Dependencies", "x64" }))
                        {
                            await ReadyFolderAsync(extractFolder, Path.Combine("Dependencies", "x64"));
                        }
                        continue;
                    }
                    else
                    {
                        // dependency item's folder path rewrite.
                        if (folders.SkipLast(1).TakeLast(2).SequenceEqual(new[] { "Dependencies", "x86" }))
                        {
                            itemPath = Path.Combine(Path.Combine("Dependencies", "x86"), entry.Name);
                        }
                        else if (folders.SkipLast(1).TakeLast(2).SequenceEqual(new[] { "Dependencies", "x64" }))
                        {
                            itemPath = Path.Combine(Path.Combine("Dependencies", "x64"), entry.Name);
                        }
                    }

                    // check file name length is over windows limitation
                    var path = Path.Combine(extractFolder.Path, itemPath);
                    if (path.Length > 280)
                    {
                        var extension = Path.GetExtension(path);
                        var leastLength = 280 - path.Length - extension.Length;
                        var newpath = string.Join("", Path.GetFileNameWithoutExtension(itemPath).Take(leastLength).ToArray()) + extension;
                        downloadStatusSubject.OnNext($"{path} is too long name. {path.Length} length. shorten to {newpath}");
                        path = newpath;
                    }
                    entry.ExtractToFile(path, true);
                }
            }
        }

        private async Task UnzipAsync(StorageFile file, StorageFolder extractFolder)
        {
            using (var stream = await file.OpenStreamForReadAsync())
            {
                await UnzipAsync(stream, extractFolder);
            }
        }

        public async Task OpenFolderAsync()
        {
            var folder = await ReadyFolderAsync(root, downloadDirectoryName);
            await Launcher.LaunchFolderAsync(folder);
        }

        public async Task OpenDownloadFolderAsync()
        {
            var token = DownloadFolderAccessToken.Value;
            StorageFolder folder;
            if (!string.IsNullOrWhiteSpace(token))
            {
                folder = await ReadyFolderAsync(token);
            }
            else
            {
                folder = await ReadyFolderAsync(root, downloadDirectoryName);
            }
            await Launcher.LaunchFolderAsync(folder);
        }

        public async Task RequestHoloLensPackagesAsync(string blobConnectionString, string containerName)
        {
            try
            {
                caches = new ConcurrentBag<IArtifactEntity>();
                repository = new BlobArtifactRepository(blobConnectionString)
                {
                    OnGetEachArtifact = artifact =>
                    {
                        blobArtifactSubject.OnNext(artifact);
                        caches.Add(artifact);
                    },
                };
                var entities = await repository.ListArtifactsAsync(containerName);
                repository = null;
            }
            catch (StorageException ex) when (ex.InnerException is TaskCanceledException)
            {
                blobArtifactFailedSubject.OnNext($"Canceled");
                PushCache();
            }
            catch (Exception ex)
            {
                blobArtifactFailedSubject.OnNext($"Error: {ex.Message}");
                PushCache();
            }
        }

        private string GetFolderToken(StorageFolder folder)
        {
            var appxFolderToken = StorageApplicationPermissions.FutureAccessList.Add(folder);
            return appxFolderToken;
        }

        private async Task<StorageFolder> ReadyFolderAsync(StorageFolder root, string directory)
        {
            var folder = Directory.Exists(Path.Combine(root.Path, directory))
                ? await root.GetFolderAsync(directory)
                : await root.CreateFolderAsync(directory);
            return folder;
        }

        private async Task<StorageFolder> ReadyFolderAsync(string folderAccessToken)
        {
            var permitFolder = await StorageApplicationPermissions.FutureAccessList.GetFolderAsync(folderAccessToken);
            return permitFolder;
        }

        private void PushCache()
        {
            if (!caches.Any())
                return;
            foreach (var cache in caches)
            {
                blobArtifactSubject.OnNext(cache);
            }
        }

        public IArtifactEntity[] GetArtifactCache()
        {
            if (!caches.Any())
            {
                return Array.Empty<IArtifactEntity>();
            }
            return caches.ToArray();
        }

        public IBranchArtifactEntity[] GetArtifactCache(string projectName)
        {
            if (!caches.Any())
            {
                return Array.Empty<IBranchArtifactEntity>();
            }
            var result = caches.Where(x => x.Project == projectName)
                .SelectMany(x => x.BranchArtifactDetail)
                .ToArray();
            return result;
        }

        public IArtifactDetailEntity[] GetArtifactCache(string projectName, string branchName)
        {
            if (!caches.Any())
            {
                return Array.Empty<IArtifactDetailEntity>();
            }
            var result = caches.Where(x => x.Project == projectName)
                .SelectMany(x => x.BranchArtifactDetail)
                .Where(x => x.Branch == branchName)
                .SelectMany(x => x.Artifact)
                .ToArray();
            return result;
        }
    }
}
