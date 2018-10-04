using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HoloLensCommander;
using HoloLensCommander.Data.Repositories;

namespace HoloLensCommander.Models.UseCases
{
    public interface IBlobSasUrlUseCase
    {
        Task CopySasUrl(string url, string blobConnectionString, string container, string blob);
    }

    public class BlobSasUrlUseCase : IBlobSasUrlUseCase
    {
        public async Task CopySasUrl(string url, string blobConnectionString, string container, string blob)
        {
            var repository = new BlobSasRepository(blobConnectionString, TimeSpan.FromMinutes(5));
            var sasGrantToken = await repository.RequestSasUrlAsync(container, blob);
            ClipboardHelper.CopyToClipboard(url + sasGrantToken);
        }
    }
}
