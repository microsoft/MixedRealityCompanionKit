#include "pch.h"
#include "AnchorImporter.h"


AnchorImporter::AnchorImporter()
{
}


AnchorImporter::~AnchorImporter()
{
}

//TODO: use AnchorImportStatus and reconnect in Update loop.
bool AnchorImporter::ConnectToServer(Platform::String^ serverIP, int port)
{
    if (importStatus == AnchorImportStatus::WaitingForConnection)
    {
        OutputDebugString(L"Currently waiting for a connection.");
        return false;
    }

    importStatus = AnchorImportStatus::WaitingForConnection;

    anchorOwnerIP = serverIP;
    anchorPort = port;

    anchorOwnerHost = ref new HostName(anchorOwnerIP);
    anchorConnection = ref new StreamSocket();

    create_task(
        anchorConnection->ConnectAsync(anchorOwnerHost, anchorPort.ToString())
    ).then([this](task<void> previousTask)
    {
        try
        {
            // Try getting all exceptions from the continuation chain above this point.
            previousTask.get();

            DataReader^ networkDataReader = ref new DataReader(anchorConnection->InputStream);
            DataReaderLoadOperation^ drlo = networkDataReader->LoadAsync(4);
            while (drlo->Status == Windows::Foundation::AsyncStatus::Started)
            {
                // just waiting.
            }

            int dataSize = networkDataReader->ReadInt32();

            OutputDebugString(L"Importing anchor data.\n");
            OutputDebugString(L"Client connected, start reading anchor with ");
            OutputDebugString(std::to_wstring(dataSize).c_str());
            OutputDebugString(L" bytes.\n");

            anchorData = new byte[dataSize];
            create_task(
                networkDataReader->LoadAsync((unsigned int)dataSize)
            ).then([=](unsigned int size)
            {
                if (size == 0)
                {
                    OutputDebugString(L"ERROR: No anchor bytes were receibed.\n");
                    return false;
                }

                OutputDebugString(L"Anchor data imported.\n");
                networkDataReader->ReadBytes(Platform::ArrayReference<byte>(anchorData, size));

                ImportAnchorDataAsync(anchorData, size);

                //TODO: start another connection.
                return true;
            });

            return true;
        }
        catch (Platform::Exception^ exception)
        {
            if ((exception->HResult == WSAETIMEDOUT) || //0x8007274c) || // connection timed out
                (exception->HResult == WSAEADDRINUSE))  //0x80072740)) // connection maxed at server end
            {
                // If the connection timed out, try again.
                ConnectToServer(anchorOwnerIP, anchorPort);
            }
            else if (exception->HResult == WSAEADDRNOTAVAIL) //0x80072741)
            {
                // No connection is possible, wait for server to start.
                return false;
            }

            //HandleException(exception);
            return true;
        }
    });
    
    return true;
}

task<bool> AnchorImporter::ImportAnchorDataAsync(byte* anchorData, int anchorSize)
{
    InMemoryRandomAccessStream^ stream = ref new InMemoryRandomAccessStream();
    IOutputStream^ outputStream = stream->GetOutputStreamAt(0);
    DataWriter^ writer = ref new DataWriter(outputStream);

    // Request access to transfer spatial anchors.
    return create_task(SpatialAnchorTransferManager::RequestAccessAsync()).then(
        [anchorData, anchorSize, writer](SpatialPerceptionAccessStatus status)
    {
        if (status == SpatialPerceptionAccessStatus::Allowed)
        {
            // Access is allowed.
            // Write the bytes to the stream.
            OutputDebugString(L"Writing bytes.\n");
            writer->WriteBytes(Platform::ArrayReference<byte>(anchorData, anchorSize));

            // Store the stream.
            return create_task(writer->StoreAsync());
        }
        else
        {
            // Access is denied.
            OutputDebugString(L"ERROR: Spatial Anchor Transfer Manager access denied.\n");
            return task_from_result<size_t>(0);
        }
    }).then([writer, stream](unsigned int bytesWritten)
    {
        if (bytesWritten > 0)
        {
            OutputDebugString(L"Flushing data.\n");
            // Try to import anchors from the byte stream.
            return create_task(writer->FlushAsync())
                .then([stream](bool dataWasFlushed)
            {
                if (dataWasFlushed)
                {
                    // Get the input stream for the anchor data.
                    IInputStream^ inputStream = stream->GetInputStreamAt(0);
                    return create_task(SpatialAnchorTransferManager::TryImportAnchorsAsync(inputStream));
                }
                else
                {
                    OutputDebugString(L"ERROR: Data was not flushed.\n");
                    return task_from_result<Windows::Foundation::Collections::IMapView<Platform::String^, SpatialAnchor^>^>(nullptr);
                }
            });
        }
        else
        {
            OutputDebugString(L"ERROR: Zero bytes written.\n");
            return task_from_result<Windows::Foundation::Collections::IMapView<Platform::String^, SpatialAnchor^>^>(nullptr);
        }
    }).then([this](task<Windows::Foundation::Collections::IMapView<Platform::String^, SpatialAnchor^>^>  previousTask)
    {
        try
        {
            OutputDebugString(L"Importing data.\n");
            Windows::Foundation::Collections::IMapView<Platform::String^, SpatialAnchor^>^ importedAnchorsMap = previousTask.get();
            if (importedAnchorsMap == nullptr || importedAnchorsMap->Size == 0)
            {
                OutputDebugString(L"ERROR: no imported anchors.\n");
                return false;
            }


            std::for_each(Windows::Foundation::Collections::begin(importedAnchorsMap), 
                Windows::Foundation::Collections::end(importedAnchorsMap), [this](Windows::Foundation::Collections::IKeyValuePair<Platform::String^, SpatialAnchor^>^ entry)
            {
                // Get the first imported anchor.
                OutputDebugString(L"Caching imported anchor.\n");
                sharedAnchor = entry->Value;
                return true;
            });
        }
        catch (Platform::Exception^ exception)
        {
            OutputDebugString(L"Error: Unable to import the anchor data buffer bytes into the in-memory anchor collection.\n");
        }

        return false;
    });
}
