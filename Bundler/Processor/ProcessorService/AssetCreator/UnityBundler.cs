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

            Utilities.DirectoryCopy(inputPath, UnitySettings.Default.UnityBundleProjectCopy + fileName, true);
            Utilities.DirectoryCopy(inputPath, UnitySettings.Default.UnityBundleProjectCopy_5_6 + fileName, true);

            Console.WriteLine(" Processing Unity");

            // Unity asset bundles are not compatible between versions (yet), so we need to create new unique bundles for each version.
            // The client is responsible for requesting the correct file version.
            var unity_5_5Task = RunUnityBundler(false);
            var unity_5_6Task = RunUnityBundler(true);

            await Task.WhenAll(unity_5_5Task, unity_5_6Task);

            CleanTempFiles();

            var bundles = unity_5_5Task.Result;
            bundles.AddRange(unity_5_6Task.Result);
            return bundles;
        }

        private Task<List<AssetCatalog.LODBundle>> RunUnityBundler(bool is5_6 = false)
        {
            return Task.Run(() =>
            {
                var bundles = new List<AssetCatalog.LODBundle>();

                var unityBundleOutputFolder = (is5_6 ? UnitySettings.Default.UnityBundleProject_5_6 : UnitySettings.Default.UnityBundleProject) + "\\AssetBundles\\";
                if (!Directory.Exists(unityBundleOutputFolder))
                    Directory.CreateDirectory(unityBundleOutputFolder);

                Console.WriteLine("  - Launching Unity " + (is5_6 ? "5.6" : "5.5") + " to create bundle (async)");

                Process unityInfo = new Process();
                unityInfo.StartInfo.CreateNoWindow = false;
                unityInfo.StartInfo.UseShellExecute = true;
                unityInfo.StartInfo.WorkingDirectory = (is5_6 ? UnitySettings.Default.UnityInstallLocation_5_6 : UnitySettings.Default.UnityInstallLocation);
                unityInfo.StartInfo.FileName = @"Unity.exe";
                string launchArgs = " -batchmode -quit -projectPath " + (is5_6 ? UnitySettings.Default.UnityBundleProject_5_6 : UnitySettings.Default.UnityBundleProject) + " -executeMethod BuildAssetBundles.BuildBundles ";
                unityInfo.StartInfo.Arguments = launchArgs;
                unityInfo.Start();

                unityInfo.WaitForExit();

                FileInfo[] files = new DirectoryInfo(unityBundleOutputFolder).GetFiles(fileName + "*.unity3d");

                if (files.Count() == 0)
                {
                    Console.WriteLine(" *** NO BUNDLES after Unity " + (is5_6 ? "5.6" : "5.5"));
                }
                else
                {
                    foreach (FileInfo file in files)
                    {
                        var origName = Path.GetFileNameWithoutExtension(file.FullName);
                        var fileName = Path.GetFileNameWithoutExtension(file.FullName) + (is5_6 ? "_5_6" : "_5_5") + Path.GetExtension(file.FullName);
                        if (File.Exists(outputPath + "\\" + fileName))
                            File.Delete(outputPath + "\\" + fileName);
                        file.MoveTo(outputPath + "\\" + fileName);

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

                        bundles.Add(new AssetCatalog.LODBundle() { Name = origName, LODNumber = number, Bundle = fileName, UnityVersion = is5_6 ? "5.6" : "5.5" });
                    }
                }
                Console.WriteLine("  - Unity " + (is5_6 ? "5.6" : "5.5") + " done.");
                return bundles;
            });
        }

        public void CleanTempFiles()
        {
            if (!string.IsNullOrEmpty(UnitySettings.Default.UnityBundleProjectCopy))
            {
                //Clean up temp Unity
                DirectoryInfo tempUnityDir = new DirectoryInfo(UnitySettings.Default.UnityBundleProjectCopy);

                foreach (FileInfo file in tempUnityDir.GetFiles())
                {
                    file.Delete();
                }
                foreach (DirectoryInfo dir in tempUnityDir.GetDirectories())
                {
                    dir.Delete(true);
                }

            }
            if (!string.IsNullOrEmpty(UnitySettings.Default.UnityBundleProject))
            {
                DirectoryInfo assetBundlesDir = new DirectoryInfo(UnitySettings.Default.UnityBundleProject + "\\AssetBundles\\");
                foreach (FileInfo file in assetBundlesDir.GetFiles())
                {
                    file.Delete();
                }
            }

            if (!string.IsNullOrEmpty(UnitySettings.Default.UnityBundleProjectCopy_5_6))
            {
                DirectoryInfo tempUnityDir_5_6 = new DirectoryInfo(UnitySettings.Default.UnityBundleProjectCopy_5_6);

                foreach (FileInfo file in tempUnityDir_5_6.GetFiles())
                {
                    file.Delete();
                }
                foreach (DirectoryInfo dir in tempUnityDir_5_6.GetDirectories())
                {
                    dir.Delete(true);
                }
            }
            if (!string.IsNullOrEmpty(UnitySettings.Default.UnityBundleProject_5_6))
            {
                DirectoryInfo assetBundlesDir_5_6 = new DirectoryInfo(UnitySettings.Default.UnityBundleProject_5_6 + "\\AssetBundles\\");
                foreach (FileInfo file in assetBundlesDir_5_6.GetFiles())
                {
                    file.Delete();
                }
            }
        }
    }
}
