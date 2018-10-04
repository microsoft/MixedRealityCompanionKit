using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HoloLensCommander.Data.Repositories;

namespace HoloLensCommander.Models.UseCases
{
    public interface IBlobConnectionUseCase
    {
        T Read<T>(string key);
        void Save<T>(string key, T value);
    }

    public class BlobConnectionUseCase : IBlobConnectionUseCase
    {
        private readonly BlobApplicationDataContainerRepository repository;
        public BlobConnectionUseCase()
        {
            repository = new BlobApplicationDataContainerRepository("azureBlobSettings");
        }

        public T Read<T>(string key)
        {
            return repository.Get<T>(key);
        }

        public void Save<T>(string key, T value)
        {
            repository.Set<T>(key, value);
        }
    }
}
