using System;
using System.Threading;

namespace DesktopServerApp
{
    public class PerPeriodCounter
    {
        private readonly SynchronizationContext _context;
        private Timer                           _timer;
        private readonly TimeSpan               _interval;
        private readonly Action<int>            _callback;
        private volatile int                    _currentCount;

        public PerPeriodCounter(
            TimeSpan interval,
            Action<int> callback,
            bool captureSyncContext = true)
        {
            this._currentCount = 0;
            this._callback = callback;
            this._interval = interval;

            if (captureSyncContext)
            {
                this._context = SynchronizationContext.Current;
            }
        }
        public void Start()
        {
            this._currentCount = 0;
            this._timer = new Timer(this.OnTimerTick, null, _interval, _interval);
        }
        public void Stop()
        {
            this._timer.Dispose();
            this._timer = null;
        }
        public void Increment()
        {
            Interlocked.Increment(ref this._currentCount);
        }
        void OnTimerTick(object state)
        {
            var value = Interlocked.Exchange(ref this._currentCount, 0);
            this.InvokeCallback(value);
        }
        void InvokeCallback(int value)
        {
            if (this._context != null)
            {
                if (this._callback != null)
                {
                    this._context.Post(_ => this._callback(value), null);
                }
            }
            else
            {
                this?._callback(value);
            }
        }

    }
}
