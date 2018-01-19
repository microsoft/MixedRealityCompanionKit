// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.IO;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace AssetCreator
{
    class Program
    {
        static FileProcessor CurrentProcessor;
        static Queue<FileProcessor> FileQueue = new Queue<FileProcessor>();
        static void Main(string[] args)
        {
            Console.WriteLine("Base data path: {0}", Settings.Default.BaseDataPath);
            Console.WriteLine("3DS Max path: {0}", Settings.Default.MaxInstallLocation);
            Console.WriteLine("3DS Max processing script path: {0}", Settings.Default.MaxScriptLocation);

            if (Settings.Default.CreateUnityAssetBundles)
            {
                Console.WriteLine("Unity bundle project: {0}", UnitySettings.Default.UnityBundleProject);
                Console.WriteLine("Unity bundle project folder to process: {0}", UnitySettings.Default.UnityBundleProjectCopy);
            }
            Console.WriteLine("_________________________________________________________");
            Console.WriteLine("");

            
            // Kick off a couple background tasks to handle the processing.
            Task.Run(() =>
            {
                while (true)
                {
                    CheckQueue();
                    Task.Delay(100).Wait();
                }
            });

            Task.Run(() =>
            {
                while (true)
                {
                    UpdateFolders();
                    Task.Delay(10000).Wait();
                }
            });

            while (true)
            {
                Task.Delay(100).Wait();
            };
        }

        public static void UpdateFolders()
        {
            if (!Directory.Exists(Settings.Default.BaseDataPath))
                Directory.CreateDirectory(Settings.Default.BaseDataPath);

            if (Directory.GetDirectories(Settings.Default.BaseDataPath).Length == 0)
                Directory.CreateDirectory(Settings.Default.BaseDataPath + "\\Public");

            /// The data folder has subfolders to isolate different groups.  Each group has their own set of user folders, upload folder and output folders.
            /// Each level also has it's own set of "public" folders.
            foreach (var orgFolderPath in Directory.GetDirectories(Settings.Default.BaseDataPath))
            {
                ScanUploadFolder(orgFolderPath);
                if (Settings.Default.CreateUnityAssetBundles)
                    AssetCatalog.UpdateCatalog(orgFolderPath);
            }
        }

        private static bool scanningFolder;
        private static void ScanUploadFolder(string orgDataPath)
        {
            if (scanningFolder)
                return;
            scanningFolder = true;
            
            try
            {
                if (Directory.GetDirectories(orgDataPath).Length == 0)
                    Directory.CreateDirectory(orgDataPath + "\\Public");
                
                foreach (var userFolderPath in Directory.GetDirectories(orgDataPath))
                {
                    var uploadFolder = userFolderPath + "\\Upload";

                    if (!Directory.Exists(uploadFolder))
                        Directory.CreateDirectory(uploadFolder);

                    Console.ForegroundColor = ConsoleColor.Blue;
                    foreach (var filePath in Directory.GetFiles(uploadFolder, "*.zip"))
                    {
                        if (!IsItemInQueue(filePath) && (CurrentProcessor == null || CurrentProcessor.ZipFilePath != filePath))
                        {
                            FileQueue.Enqueue(new FileProcessor(filePath));
                            Console.WriteLine("Added File {0}", filePath);
                        }
                    }
                }
                Console.ResetColor();
            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("*** Exception scanning for uploads {0}", ex.Message);
                Console.ResetColor();
            }
            finally { scanningFolder = false; }
        }

        private static bool IsItemInQueue(string filePath)
        {
            foreach (var file in FileQueue)
            {
                if (file.ZipFilePath == filePath)
                {
                    return true;
                }
            }
            return false;
        }

        private static async void CheckQueue()
        {
            if (CurrentProcessor == null || !CurrentProcessor.IsBusy)
            {
                CurrentProcessor = null;
                if (FileQueue.Count > 0)
                {
                    try
                    {
                        CurrentProcessor = FileQueue.Dequeue();
                        Console.WriteLine("FileQueue.Dequeue {0}", CurrentProcessor.ZipFilePath);
                        await CurrentProcessor.ProcessZip();
                        UpdateFolders();
                    }
                    catch (Exception ex)
                    {
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine("*** Exception in processZip {0}", ex.Message);
                        Console.ResetColor();
                    }
                }
            }
        }
    }
}
