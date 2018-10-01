using System;
using Microsoft.WindowsAzure.Storage.Blob;

namespace HoloLensCommander.Models.Entities
{
    public interface IArtifactEntity
    {
        string Project { set; get; }
        IBranchArtifactEntity[] BranchArtifactDetail { set; get; }
    }

    public interface IBranchArtifactEntity
    {
        string Branch { set; get; }
        IArtifactDetailEntity[] Artifact { set; get; }
    }

    public interface IArtifactDetailEntity
    {
        string Name { set; get; }
        string FileName { set; get; }
        Uri Uri { set; get; }
        long Size { set; get; }
        string MD5 { set; get; }
        LeaseState LeaseState { set; get; }
    }

    public struct BlobArtifactEntity : IArtifactEntity
    {
        public BlobArtifactEntity(string project, IBranchArtifactEntity[] branchArtifactDetail) : this()
        {
            Project = project;
            BranchArtifactDetail = branchArtifactDetail;
        }

        public string Project { set; get; }
        public IBranchArtifactEntity[] BranchArtifactDetail { set; get; }
    }

    public struct BlobBranchArtifactEntity : IBranchArtifactEntity
    {
        public BlobBranchArtifactEntity(string branch, IArtifactDetailEntity[] artifact) : this()
        {
            Branch = branch;
            Artifact = artifact;
        }

        public string Branch { set; get; }
        public IArtifactDetailEntity[] Artifact { set; get; }
    }

    public struct BlobArtifactDetailEntity : IArtifactDetailEntity
    {
        public BlobArtifactDetailEntity(string name, string fileName, Uri uri, long size, string md5, LeaseState leaseState) : this()
        {
            Name = name;
            FileName = fileName;
            Uri = uri;
            Size = size;
            MD5 = md5;
            LeaseState = leaseState;
        }

        public string Name { set; get; }
        public string FileName { set; get; }
        public Uri Uri { set; get; }
        public long Size { set; get; }
        public string MD5 { set; get; }
        public LeaseState LeaseState { set; get; }
    }
}
