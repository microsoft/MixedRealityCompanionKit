using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reactive;
using System.Reactive.Disposables;
using System.Reactive.Linq;
using System.Text;
using System.Threading.Tasks;
using HoloLensCommander.Models.Entities;
using HoloLensCommander.Models.UseCases;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using SelectedTextSpeach.Models.UseCases;
using Windows.ApplicationModel.DataTransfer;
using Windows.Storage;
using Windows.UI.Xaml;

namespace HoloLensCommander
{
    public class GetAppInstallFilesBlobDialogViewModel : IDisposable
    {
        private static readonly string ExpandButtonLabel = "\xE710";     // + sign
        private static readonly string CollapseButtonLabel = "\xE738";   // - sign

        private IBlobArtifactUseCase blobArtifactUsecase = new BlobArtifactUseCase(ApplicationData.Current.LocalFolder, "tmp");
        private IBlobConnectionUseCase blobConnectionUseCase = new BlobConnectionUseCase();
        private CompositeDisposable disposable = new CompositeDisposable();
        private DataPackage dataPackage = new DataPackage();

        public ReactiveProperty<Visibility> ShowBlobSectionVisibility { get; } = new ReactiveProperty<Visibility>(Visibility.Collapsed);
        public ReactiveCommand ShowHideBlobSectionCommand { get; } = new ReactiveCommand();
        public ReactiveProperty<string> ShowHideBlobSectionButtonLabel { get; } = new ReactiveProperty<string>(ExpandButtonLabel);

        public ReactiveProperty<string> StorageConnectionInput { get; }
        public ReactiveProperty<string> StorageContainerInput { get; }

        public ReadOnlyReactiveProperty<bool> IsEnableCheckBlobButton { get; }
        public AsyncReactiveCommand OnClickCheckBlobCommand { get; }
        public ReactiveProperty<bool> IsBlobChecking { get; } = new ReactiveProperty<bool>();
        public ReactiveCommand OnClickCancelBlobCommand { get; } = new ReactiveCommand();
        public ReactiveProperty<string> BlobResult { get; set; } = new ReactiveProperty<string>();
        public ReactiveProperty<bool> ComboBoxEnabled { get; set; }

        public ObservableCollection<IArtifactEntity> Projects { get; } = new ObservableCollection<IArtifactEntity>();
        public ReactiveProperty<IArtifactEntity> SelectedProject { get; } = new ReactiveProperty<IArtifactEntity>();
        public ReactiveCollection<IBranchArtifactEntity> Branches { get; }
        public ReactiveProperty<IBranchArtifactEntity> SelectedBranch { get; } = new ReactiveProperty<IBranchArtifactEntity>();
        public ReactiveCollection<IArtifactDetailEntity> Artifacts { get; }
        public ReactiveProperty<IArtifactDetailEntity> SelectedArtifact { get; } = new ReactiveProperty<IArtifactDetailEntity>();
        public ReactiveProperty<string> ArtifactName { get; } = new ReactiveProperty<string>();
        public ReactiveProperty<string> ArtifactCaption { get; } = new ReactiveProperty<string>();
        public ReactiveProperty<string> ArtifactUrl { get; } = new ReactiveProperty<string>();

        public ReactiveProperty<string> CopyButtonContent { get; }
        public ReactiveProperty<bool> CopyButtonEnabled { get; }
        public ReactiveCommand OnClickCopyCommand { get; }

        public AsyncReactiveCommand OnClickDownloadCommand { get; }
        public ReactiveProperty<string> DownloadStatus { get; } = new ReactiveProperty<string>();
        public AsyncReactiveCommand OnClickOpenDownloadFolderCommand { get; } = new AsyncReactiveCommand();
        public AsyncReactiveCommand OnClickOpenDownloadBlobFolderCommand { get; } = new AsyncReactiveCommand();

        // other view module reference
        public ReactiveProperty<string> DownloadFolderAccessToken => blobArtifactUsecase.DownloadFolderAccessToken;

        public GetAppInstallFilesBlobDialogViewModel()
        {
            // Show/Hide Blob
            ShowHideBlobSectionCommand.Subscribe(_ =>
            {
                ShowHideBlobSectionButtonLabel.Value = ShowBlobSectionVisibility.Value == Visibility.Collapsed
                    ? ExpandButtonLabel
                    : CollapseButtonLabel;
                ShowBlobSectionVisibility.Value = ShowBlobSectionVisibility.Value == Visibility.Collapsed
                    ? Visibility.Visible
                    : Visibility.Collapsed;
            })
            .AddTo(disposable);

            // Storage Credential Input
            StorageConnectionInput = new ReactiveProperty<string>(blobConnectionUseCase.Read<string>("blob_connection_string"));
            StorageConnectionInput.Subscribe(x => blobConnectionUseCase.Save("blob_connection_string", x)).AddTo(disposable);
            StorageContainerInput = new ReactiveProperty<string>(blobConnectionUseCase.Read<string>("container"));
            StorageContainerInput.Subscribe(x => blobConnectionUseCase.Save("container", x)).AddTo(disposable);

            // Copy Button
            CopyButtonContent = new ReactiveProperty<string>("Copy");
            CopyButtonEnabled = ArtifactUrl.Select(x => !string.IsNullOrWhiteSpace(x)).ToReactiveProperty();
            OnClickCopyCommand = CopyButtonEnabled.ToReactiveCommand();
            OnClickCopyCommand
                .Do(_ => ClipboardHelper.CopyToClipboard(ArtifactUrl.Value))
                .SelectMany(x => TemporaryDisableCopyButtonAsObservable(TimeSpan.FromMilliseconds(500)))
                .Subscribe()
                .AddTo(disposable);

            // Download Button
            blobArtifactUsecase.DownloadStatus.Subscribe(x => DownloadStatus.Value = x).AddTo(disposable);
            OnClickDownloadCommand = CopyButtonEnabled.ToAsyncReactiveCommand();
            OnClickDownloadCommand
                .Subscribe(async _ => await blobArtifactUsecase.DownloadHoloLensPackagesAsync(StorageConnectionInput.Value, StorageContainerInput.Value, SelectedArtifact.Value.Name, SelectedArtifact.Value.Size, SelectedArtifact.Value.FileName))
                .AddTo(disposable);

            // OpenFolder Button
            OnClickOpenDownloadFolderCommand.Subscribe(_ => blobArtifactUsecase.OpenFolderAsync()).AddTo(disposable);
            OnClickOpenDownloadBlobFolderCommand.Subscribe(_ => blobArtifactUsecase.OpenDownloadFolderAsync()).AddTo(disposable);

            // Initialize by obtain artifact informations
            blobArtifactUsecase.Artifacts
                .Where(x => x != null)
                .Do(x =>
                {
                    Projects.Add(x);
                    BlobResult.Value = $"Found {Projects.Count} projects.";
                })
                .Subscribe()
                .AddTo(disposable);
            blobArtifactUsecase.RequestFailedMessage
                .Do(x => BlobResult.Value = x)
                .Subscribe()
                .AddTo(disposable);

            // Blob Download
            ComboBoxEnabled = Projects.CollectionChangedAsObservable().Any().ToReactiveProperty();
            IsEnableCheckBlobButton = StorageConnectionInput
                .CombineLatest(StorageContainerInput, (r, l) => !string.IsNullOrWhiteSpace(r) && !string.IsNullOrWhiteSpace(l))
                .ToReadOnlyReactiveProperty();
            OnClickCheckBlobCommand = IsEnableCheckBlobButton.ToAsyncReactiveCommand();
            OnClickCheckBlobCommand.Subscribe(async _ =>
            {
                var task = blobArtifactUsecase.RequestHoloLensPackagesAsync(StorageConnectionInput.Value, StorageContainerInput.Value);
                IsBlobChecking.Value = true;
                Projects.Clear();
                Branches?.Clear();
                Artifacts?.Clear();
                BlobResult.Value = "Trying obtain project infomations.";
                await task;
                IsBlobChecking.Value = false;
            })
            .AddTo(disposable);
            OnClickCancelBlobCommand = IsEnableCheckBlobButton.Select(x => !x).ToReactiveCommand();
            OnClickCancelBlobCommand.Subscribe(_ => blobArtifactUsecase.CancelRequest()).AddTo(disposable);

            // Update Collection with Clear existing collection when selected.
            Branches = SelectedProject.Where(x => x != null)
                .Do(_ => Branches?.Clear())
                .Do(_ => Artifacts?.Clear())
                .SelectMany(x => blobArtifactUsecase.GetArtifactCache(x.Project))
                .ToReactiveCollection();
            Artifacts = SelectedBranch.Where(x => x != null)
                .Do(x => Artifacts?.Clear())
                .SelectMany(x => blobArtifactUsecase.GetArtifactCache(SelectedProject.Value?.Project, x.Branch))
                .ToReactiveCollection();
            SelectedArtifact
                .Where(x => x != null)
                .Do(x =>
                {
                    ArtifactName.Value = x.Name;
                    ArtifactCaption.Value = $"(Size: {x.Size}, MD5: {x.MD5}, LeaseState: {x.LeaseState})";
                    ArtifactUrl.Value = x.Uri.AbsoluteUri;
                })
                .ToReactiveProperty();
        }

        private IObservable<Unit> TemporaryDisableCopyButtonAsObservable(TimeSpan duration)
        {
            // Change ButtonContent a while
            return Observable.Start(() =>
            {
                CopyButtonContent.Value = "Copied!!";
                CopyButtonEnabled.Value = false;
            })
            .Delay(duration)
            .Do(__ =>
            {
                CopyButtonContent.Value = "Copy";
                CopyButtonEnabled.Value = true;
            })
            .ToUnit();
        }

        public void Dispose()
        {
            disposable.Dispose();

        }
    }
}
