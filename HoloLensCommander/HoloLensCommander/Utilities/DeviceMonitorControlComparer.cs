using System.Collections.Generic;

namespace HoloLensCommander
{
    /// <summary>
    /// Implementation of IComparer<T> for DeviceMonitorControl objects.
    /// </summary>
    public class DeviceMonitorControlComparer : IComparer<DeviceMonitorControl>
    {
        /// <summary>
        /// Compare two DeviceMonitorControl objects.
        /// </summary>
        /// <param name="x">The DeviceMonitorControl object.</param>
        /// <param name="x">The DeviceMonitorControl object which will be compared to x.</param>
        /// <returns>0 if the objects are equivalent, 1 if x sorts in front of y, -1 otherwise.</returns>
        public int Compare(DeviceMonitorControl x, DeviceMonitorControl y)
        {
            // Get the view model for each device
            DeviceMonitorControlViewModel xViewModel = (DeviceMonitorControlViewModel)x.DataContext;
            DeviceMonitorControlViewModel yViewModel = (DeviceMonitorControlViewModel)y.DataContext;

            // Sort by name
            int comparisonResult = string.Compare(
                xViewModel.Name,
                yViewModel.Name);

            if (string.IsNullOrWhiteSpace(xViewModel.Name) ||
                 string.IsNullOrWhiteSpace(yViewModel.Name))
            {
                // Sort empty names last
                comparisonResult = -(comparisonResult);
            }

            if (0 == comparisonResult)
            {
                // Names were equivalent, sort by address
                comparisonResult = string.Compare(
                    xViewModel.Address, 
                    yViewModel.Address);
            }

            return comparisonResult;
        }
    }
}
