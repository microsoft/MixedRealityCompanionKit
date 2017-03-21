// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Storage;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media;
using static Microsoft.Tools.WindowsDevicePortal.DevicePortal;

namespace HoloLensCommander
{
    /// <summary>
    /// Collection of static methods that perform various utilitarian tasks.
    /// </summary>
    static class Utilities
    {
        /// <summary>
        /// Gets the collection of application names from a list of PackageInfo objects.
        /// </summary>
        /// <param name="packages">List of PackageInfo.</param>
        /// <param name="sort">Indicates whether or not the returned list is to be sorted alphanumerically.</param>
        /// <returns>List of application names.</returns>
        public static List<string> GetAppNamesFromPackageInfo(
            List<PackageInfo> packages,
            bool sort)
        {
            List<string> appNames = new List<string>();

            foreach (PackageInfo packageInfo in packages)
            {
                if (!string.IsNullOrWhiteSpace(packageInfo.Name) &&
                    !packageInfo.Name.StartsWith("@{")           &&
                    !appNames.Contains(packageInfo.Name))
                {
                    appNames.Add(packageInfo.Name);
                }
            }

            if (sort)
            {
                appNames.Sort();
            }

            return appNames;
        }

        /// <summary>
        /// Gets the rectangle that encloses an element.
        /// </summary>
        /// <param name="element">FrameworkElement for which the rectangle is requested.</param>
        /// <returns>The enclosing rectangle.</returns>
        public static Rect GetFrameworkElementRect(FrameworkElement element)
        {
            GeneralTransform buttonTransform = element.TransformToVisual(null);
            Point point = buttonTransform.TransformPoint(new Point());
            return new Rect(point, new Size(element.ActualWidth, element.ActualHeight));
        }

        /// <summary>
        /// Returns the application package name associated with the specified application name.
        /// </summary>
        /// <param name="appName">Name of the application for which the package name is to be returned.</param>
        /// <param name="appPackages">Collection of application packages to be searched.</param>
        /// <returns>The application package name.</returns>
        public static string GetPackageNameFromAppName(
            string appName,
            AppPackages appPackages)
        {
            string packageName = null;

            foreach (PackageInfo packageInfo in appPackages.Packages)
            {
                if (packageInfo.Name == appName)
                {
                    packageName = packageInfo.FullName;
                    break;
                }
            }

            return packageName;
        }
    }
}
