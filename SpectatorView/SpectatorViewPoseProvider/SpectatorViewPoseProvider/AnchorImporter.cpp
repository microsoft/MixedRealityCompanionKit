#include "pch.h"
#include "AnchorImporter.h"


AnchorImporter::AnchorImporter()
{
}


AnchorImporter::~AnchorImporter()
{
}


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

            //m_anchorTcpSocketStreamReader = ref new DataReader(m_socketClient->InputStream);

            OutputDebugString(L"Client connected!\n");

            //m_anchorTcpSocketStreamReader->InputStreamOptions = InputStreamOptions::ReadAhead;

            //WaitForAnchorDataStream();

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
