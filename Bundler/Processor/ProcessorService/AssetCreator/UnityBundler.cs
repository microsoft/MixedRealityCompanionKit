// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace AssetCreator
{
    class UnityBundler
    {
        private string inputPath;
        private string fileName;
        private string outputPath;
        public UnityBundler(string inputPath, string fileName, string outputPath)
        {
            this.inputPath = inputPath;
            this.fileName = fileName;
            this.outputPath = outputPath;
        }

        public async Task<List<AssetCatalog.LODBundle>> CreateBundlesAsync()
        {
            Console.WriteLine(" Copying Exported Max Files To Unity Projects");

            Utilities.DirectoryCopy(inputPath, Path.Combine(UnitySettings.Default.UnityBundleProjectCopy, fileName), true);
            Utilities.DirectoryCopy(inputPath, Path.Combine(UnitySettings.Default.UnityBundleProjectCopy_5_6, fileName), true);

            Console.WriteLine(" Processing Unity");

            // Unity asset bundles are not compatible between versions (yet), so we need to create new unique bundles for each version.
            // The client is responsible for requesting the correct file version.
            var unityTask = RunUnityBundler(UnitySettings.Default.UnityVersionName, UnitySettings.Default.UnityInstallLocation, UnitySettings.Default.UnityBundleProject, UnitySettings.Default.UnityVersionFileSuffix);
            var unity_5_6Task = RunUnityBundler("5.6", UnitySettings.Default.UnityInstallLocation_5_6, UnitySettings.Default.UnityBundleProject_5_6, "_5_6");

            await Task.WhenAll(unityTask, unity_5_6Task);

            CleanTempFiles(UnitySettings.Default.UnityBundleProject, UnitySettings.Default.UnityBundleProjectCopy);
            CleanTempFiles(UnitySettings.Default.UnityBundleProject_5_6, UnitySettings.Default.UnityBundleProjectCopy_5_6);

            var bundles = unityTask.Result;
            bundles.AddRange(unity_5_6Task.Result);
            return bundles;
        }

        private Task<List<AssetCatalog.LODBundle>> RunUnityBundler(string unityVersionName, string unityInstallLocation, string bundleProjectLocation, string bundleSuffix)
        {
            return Task.Run(() =>
            {
                var bundles = new List<AssetCatalog.LODBundle>();

                var unityBundleOutputFolder = Path.Combine(bundleProjectLocation, "AssetBundles");
                if (!Directory.Exists(unityBundleOutputFolder))
                    Directory.CreateDirectory(unityBundleOutputFolder);

                Console.WriteLine("  - Launching Unity {0} to create bundle (async)", unityVersionName);

                Process unityInfo = new Process();
                unityInfo.StartInfo.CreateNoWindow = false;
                unityInfo.StartInfo.UseShellExecute = true;
                unityInfo.StartInfo.WorkingDirectory = unityInstallLocation;
                unityInfo.StartInfo.FileName = "Unity.exe";
                string launchArgs = " -batchmode -quit -projectPath " + bundleProjectLocation + " -executeMethod BuildAssetBundles.BuildBundles ";
                unityInfo.StartInfo.Arguments = launchArgs;
                unityInfo.Start();

                unityInfo.WaitForExit();

                FileInfo[] files = new DirectoryInfo(unityBundleOutputFolder).GetFiles(fileName + "*.unity3d");

                if (files.Count() == 0)
                {
                    Console.WriteLine(" *** NO BUNDLES after Unity {0}", unityVersionName);
                }
                else
                {
                    foreach (FileInfo file in files)
                    {
                        var origName = Path.GetFileNameWithoutExtension(file.FullName);
                        var fileName = Path.GetFileNameWithoutExtension(file.FullName) + bundleSuffix + Path.GetExtension(file.FullName);
                        var fullName = Path.Combine(outputPath, fileName);

                        if (File.Exists(fullName))
                        {
                            File.Delete(fullName);
                        }
                        file.MoveTo(fullName);

                        var number = 0;
                        //HACK: currently we just have 5 LOD's that are differentiated by the filename
                        if (origName.ToUpper().EndsWith("LOD0"))
                            number = 0;
                        if (origName.ToUpper().EndsWith("LOD1"))
                            number = 1;
                        if (origName.ToUpper().EndsWith("LOD2"))
                            number = 2;
                        if (origName.ToUpper().EndsWith("LOD3"))
                            number = 3;
                        if (origName.ToUpper().EndsWith("LOD4"))
                            number = 4;
                        if (origName.ToUpper().EndsWith("LOD5"))
                            number = 5;

                        bundles.Add(new AssetCatalog.LODBundle() { Name = origName, LODNumber = number, Bundle = fileName, UnityVersion = unityVersionName });
                    }
                }
                Console.WriteLine("  - Unity {0} done.", unityVersionName);
                return bundles;
            });
        }

        public void CleanTempFiles(string bundleProjectLocation, string bundleProjectCopyLocation)
        {
            if (!string.IsNullOrEmpty(bundleProjectCopyLocation))
            {
                //Clean up temp Unity
                DirectoryInfo tempUnityDir = new DirectoryInfo(bundleProjectCopyLocation);

                foreach (FileInfo file in tempUnityDir.GetFiles())
                {
                    file.Delete();
                }
                foreach (DirectoryInfo dir in tempUnityDir.GetDirectories())
                {
                    dir.Delete(true);
                }

            }
            if (!string.IsNullOrEmpty(bundleProjectLocation))
            {
                DirectoryInfo assetBundlesDir = new DirectoryInfo(Path.Combine(bundleProjectLocation, "AssetBundles"));
                foreach (FileInfo file in assetBundlesDir.GetFiles())
                {
                    file.Delete();
                }
            }
        }
    }
}
