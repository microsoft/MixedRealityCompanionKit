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
using Windows.ApplicationModel.DataTransfer;
using Windows.Storage;
using Windows.UI.Xaml;

namespace HoloLensCommander
{
    public class MainWindowViewModelReactive : IDisposable
    {
        private static readonly string Lock = "\xE785";
        private static readonly string Unlock = "\xE72E";

        private CompositeDisposable disposable = new CompositeDisposable();

        public ReactiveProperty<bool> IsLockUninstallSideLoadAppButtonEnable { get; } = new ReactiveProperty<bool>();
        public ReactiveCommand ToggleLockUninstallSideLoadApp { get; } = new ReactiveCommand();
        public ReactiveProperty<string> LockUninstallSideLoadAppLabel { get; } = new ReactiveProperty<string>(Lock);
        public ReactiveProperty<string> LockUninstallSideLoadAppTooltip { get; } = new ReactiveProperty<string>(nameof(Lock));

        public MainWindowViewModelReactive()
        {
            ToggleLockUninstallSideLoadApp.Subscribe(_ => IsLockUninstallSideLoadAppButtonEnable.Value = !IsLockUninstallSideLoadAppButtonEnable.Value).AddTo(disposable);
            IsLockUninstallSideLoadAppButtonEnable.Subscribe(x =>
            {
                LockUninstallSideLoadAppLabel.Value = x ? Unlock : Lock;
                LockUninstallSideLoadAppTooltip.Value = x ? nameof(Unlock) : nameof(Lock);
            })
            .AddTo(disposable);
        }

        public void Dispose()
        {
            disposable.Dispose();
        }
    }
}
