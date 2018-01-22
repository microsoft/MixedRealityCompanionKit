// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Threading.Tasks;

namespace AssetCreator
{
    public class FileProcessor : IEquatable<FileProcessor>
    {
        public bool IsBusy { get { return busy; } }
        public string ZipFilePath { get { return zipFileToProcess; } }

        private string zipFileName;
        private string userFolderName;
        private string baseFolderName;
        private string baseFolderPath;
        private string bundleOutputFolderPath;
        private string processedOutputFolderPath;
        private string failedOutputFolderPath;
        private string tempFolderPath;

        private volatile bool busy = false;

        private string zipFileToProcess;
        private string assetId = "";

        private AssetCatalog.AssetInfo assetInfo = new AssetCatalog.AssetInfo();

        public FileProcessor(string path)
        {
            zipFileToProcess = path;
            zipFileName = Path.GetFileNameWithoutExtension(zipFileToProcess);
            assetId = zipFileName;

            SetupFolders();

            if (Settings.Default.CreateUnityAssetBundles)
            {
                if (AssetCatalog.LoadAssetInfo(Path.Combine(bundleOutputFolderPath, "bundleInfo.json")) != null)
                {
                    assetInfo = AssetCatalog.LoadAssetInfo(Path.Combine(bundleOutputFolderPath, "bundleInfo.json"));
                    // Existing asset, just updating.
                    if (string.IsNullOrEmpty(assetInfo.Id))
                        assetInfo.Id = assetId;
                    assetInfo.Version += 1;
                    assetInfo.OwnerId = userFolderName;
                    assetInfo.Updated = DateTime.Now.ToString();
                    UpdateAssetInfoStatus("waiting for update");
                }
                else
                {
                    // New asset.
                    assetInfo.Id = assetId;
                    assetInfo.OwnerId = userFolderName;
                    assetInfo.Name = zipFileName;
                    assetInfo.Created = DateTime.Now.ToString();
                    assetInfo.Updated = assetInfo.Created;
                    assetInfo.Thumbnail = zipFileName + ".png";
                    assetInfo.Bundles = new List<AssetCatalog.LODBundle>();
                    UpdateAssetInfoStatus("waiting");
                }
            }
        }

        private void SetupFolders()
        {
            userFolderName = new DirectoryInfo(new FileInfo(zipFileToProcess).DirectoryName).Parent.Name;
            baseFolderName = new DirectoryInfo(new FileInfo(zipFileToProcess).DirectoryName).Parent.Parent.Name;

            baseFolderPath = Path.Combine(Settings.Default.BaseDataPath, baseFolderName); ;
            processedOutputFolderPath = Path.Combine(baseFolderPath, userFolderName, "Processed", assetId);
            failedOutputFolderPath = Path.Combine(baseFolderPath, userFolderName, "Failed", assetId);
            tempFolderPath = Path.Combine(Settings.Default.TempPath, assetId);

            if (Settings.Default.CreateUnityAssetBundles)
            {
                bundleOutputFolderPath = Path.Combine(baseFolderPath, userFolderName, "Bundles", assetId);
                Directory.CreateDirectory(bundleOutputFolderPath);
            }
        }

        private void UpdateAssetInfoStatus(string newStatus)
        {
            if (!Settings.Default.CreateUnityAssetBundles)
                return;

            assetInfo.Status = newStatus;
            AssetCatalog.UpdateAssetInfo(assetInfo, Path.Combine(bundleOutputFolderPath, "bundleInfo.json"));
        }

        public async Task<bool> ProcessZip()
        {
            busy = true;
            return await Task.Run(async () =>
            {
                if (!File.Exists(zipFileToProcess))
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine(" *** File missing!");
                    Console.ResetColor();
                    busy = false;
                    return false;
                }

                UpdateAssetInfoStatus("processing_locked");

                var success = false;

                var fileInfo = new FileInfo(zipFileToProcess);
                while (Utilities.IsFileLocked(fileInfo)) { Task.Delay(100).Wait(); Console.WriteLine("Waiting... {0}", zipFileToProcess); }

                var exportPath = Path.Combine(tempFolderPath, "Export");

                try
                {
                    UpdateAssetInfoStatus("processing_extracting");
                    if(Directory.Exists(tempFolderPath))
                        Directory.Delete(tempFolderPath, true);

                    ZipFile.ExtractToDirectory(zipFileToProcess, tempFolderPath);
                    Console.WriteLine(" Extracted File");

                    UpdateAssetInfoStatus("processing_max");
                    //Run Function to launch 3D Max and send it the directory where we extracted zip
                    Execute3DMaxOps(tempFolderPath);

                    if (Directory.Exists(exportPath))
                    {
                        var files = Directory.GetFiles(exportPath, "*.fbx");
                        if (files.Length < 1)
                        {
                            Console.ForegroundColor = ConsoleColor.Red;
                            Console.WriteLine(" *** No FBX after Max");
                            Console.ResetColor();
                            success = false;
                        }
                        else
                        {
                            for (var i = 0; i < files.Length; i++)
                            {
                                // Rename the FBX to the be same as the zip name
                                var fbxName = Path.GetFileNameWithoutExtension(files[i]);
                                var newName = fbxName.Replace("mesh", zipFileName);
                                File.Move(Path.Combine(exportPath, fbxName + ".fbx"), Path.Combine(exportPath, newName + ".fbx"));
                            }

                            File.Move(Path.Combine(exportPath, "mesh.png"), Path.Combine(exportPath, zipFileName + ".png"));
                            File.Move(Path.Combine(exportPath, "mesh.json"), Path.Combine(exportPath, "maxinfo.json"));

                            var bundleOutputPngFileName = Path.Combine(bundleOutputFolderPath, zipFileName + ".png");
                            if (File.Exists(bundleOutputPngFileName))
                                File.Delete(bundleOutputPngFileName);
                            File.Copy(Path.Combine(exportPath, zipFileName + ".png"), bundleOutputPngFileName);

                            var bundleOutputJsonFileName = Path.Combine(bundleOutputFolderPath, "maxinfo.json");
                            if (File.Exists(bundleOutputJsonFileName))
                                File.Delete(bundleOutputJsonFileName);
                            File.Copy(Path.Combine(exportPath,  "maxinfo.json"), bundleOutputJsonFileName);

                            success = true;

                            if (Settings.Default.CreateUnityAssetBundles)
                            {
                                UpdateAssetInfoStatus("processing_unity");
                                var unityBundler = new UnityBundler(exportPath, zipFileName, bundleOutputFolderPath);
                                assetInfo.Bundles = await unityBundler.CreateBundlesAsync();
                                if(assetInfo.Bundles.Count == 0)
                                {
                                    UpdateAssetInfoStatus("failed_unity");
                                    Console.ForegroundColor = ConsoleColor.Red;
                                    Console.WriteLine(" *** No bundles after Unity!");
                                    Console.ResetColor();
                                    success = false;
                                }                                
                            }
                        }
                    }
                    else
                    {
                        UpdateAssetInfoStatus("failed_max");
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine(" *** No export folder from Max!");
                        Console.ResetColor();
                    }
                }
                catch (Exception ex)
                {
                    UpdateAssetInfoStatus("failed_ex");
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine(" *** Exception: " + ex.ToString());
                    Console.ResetColor();
                    success = false;
                }

                if (success)
                {
                    UpdateAssetInfoStatus("success");
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine("Finished Creating New Bundle");
                    Console.ResetColor();

                    if (!Directory.Exists(processedOutputFolderPath))
                        Directory.CreateDirectory(processedOutputFolderPath);

                    // TODO: We probably should have unique folder paths for all files, but just delete and replace for now.
                    var processedOutputZipFileName = Path.Combine(processedOutputFolderPath, Path.GetFileName(zipFileToProcess));
                    if (File.Exists(processedOutputZipFileName))
                        File.Delete(processedOutputZipFileName);
                    File.Move(zipFileToProcess, processedOutputZipFileName);

                    var processedMaxOutputPath = Path.Combine(processedOutputFolderPath, "MaxOutput");
                    if (Directory.Exists(processedMaxOutputPath))
                        Directory.Delete(processedMaxOutputPath, true);
                    Utilities.DirectoryCopy(exportPath, processedMaxOutputPath, true);
                }
                else
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine(" *** Something bad happened along the way.");
                    Console.ResetColor();
                    UpdateAssetInfoStatus("failed_convert");

                    if (!Directory.Exists(failedOutputFolderPath))
                        Directory.CreateDirectory(failedOutputFolderPath);

                    // We probably should have unique folder paths for all files, but just delete for now
                    var failedOutputZipFileName = Path.Combine(failedOutputFolderPath, Path.GetFileName(zipFileToProcess));
                    if (File.Exists(failedOutputZipFileName))
                        File.Delete(failedOutputZipFileName);
                    File.Move(zipFileToProcess, failedOutputZipFileName);

                    var failedMaxOutputPath = Path.Combine(failedOutputFolderPath, "MaxOutput");
                    if (Directory.Exists(failedMaxOutputPath))
                        Directory.Delete(failedMaxOutputPath, true);
                    if (Directory.Exists(exportPath))
                        Utilities.DirectoryCopy(exportPath, failedMaxOutputPath, true);
                }

                //Clean up temp Max Files
                DirectoryInfo tempMaxDir = new DirectoryInfo(tempFolderPath);
                if (tempMaxDir.Exists)
                    tempMaxDir.Delete(true);

                Console.WriteLine("_________________________________________________________");
                Console.WriteLine("");
                busy = false;
                return success;
            });
        }

        private void Execute3DMaxOps(string dir)
        {
            Console.WriteLine(" Launching Max");

            Process maxInfo = new Process();
            maxInfo.StartInfo.CreateNoWindow = false;
            maxInfo.StartInfo.UseShellExecute = true;
            maxInfo.StartInfo.WorkingDirectory = Settings.Default.MaxInstallLocation;
            maxInfo.StartInfo.FileName = @"3dsmax.exe";
            string launchArgs = " -mxs \"global inputDir = @\\\"" + dir + "\\\";" +
                "global inTargetPlats = @\\\"" + Settings.Default.MaxTargetPlatforms + "\\\";" +
                "global inTargetVertCount = @\\\"" + Settings.Default.MaxTargetVertexCount + "\\\";" +
                "global inDontDecimateLessThan = @\\\"" + Settings.Default.MaxDontDecimateLessThan + "\\\";" +

                "global inCombinePartsWithSameMaterial = @\\\"" + Settings.Default.MaxCombinePartsWithSameMaterial + "\\\";" +
                "global inQualitySurfAreaCalc = @\\\"" + Settings.Default.MaxQualitySurfAreaCalc + "\\\";" +
                "global inImportAllFiles = @\\\"" + Settings.Default.MaxImportAllFiles + "\\\";" +

                "filein @\\\"" + Settings.Default.MaxScriptLocation + "\\\"";
            maxInfo.StartInfo.Arguments = launchArgs;
            maxInfo.Start();

            maxInfo.WaitForExit();
            Console.WriteLine(" Max Closed");
        }        

        public bool Equals(FileProcessor other)
        {
            return other == null ? false : zipFileToProcess == other.zipFileToProcess;
        }
    }
}
