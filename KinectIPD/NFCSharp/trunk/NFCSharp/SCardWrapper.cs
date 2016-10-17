using System;
using System.Text;
using System.Runtime.InteropServices;

	internal class SCW
	{
        // PROTOTYPES

        [DllImport("winscard.dll")]
        public static extern int SCardEstablishContext(int dwScope, int pvReserved1, int pvReserved2, out IntPtr hContext);

        [DllImport("winscard.dll")]
        public static extern int SCardReleaseContext(IntPtr hContext);


        [DllImport("winscard.dll")]
        public static extern int SCardListReaders(IntPtr hContext, byte[] Groups, byte[] Readers, ref int pcchReaders);

        [DllImport("winscard.dll")]
        public static extern int SCardListReaderGroups(IntPtr hContext, ref string Groups, ref int pcchGroups);

        [DllImport("winscard.dll", CharSet = CharSet.Unicode)]
        public static extern int SCardGetStatusChange(IntPtr hContext, int dwTimeout, [In, Out] SCARD_READERSTATE[] rgReaderStates, int cReaders);


        [DllImport("winscard.dll")]
        public static extern int SCardConnect(IntPtr hContext, string ReaderName, int dwShareMode, int dwPrefProtocol, out IntPtr hCard, ref int ActiveProtocol);

        [DllImport("winscard.dll")]
        public static extern int SCardDisconnect(IntPtr hCard, int Disposition);


        [DllImport("winscard.dll")]
        public static extern int SCardStatus(IntPtr hCard, string ReaderName, ref int pcchReaderLen, ref int State, ref int Protocol, ref byte ATR, ref int ATRLen);

        [DllImport("winscard.dll")]
        public static extern int SCardState(IntPtr hCard, ref int State, ref int Protocol, ref byte ATR, ref int ATRSize);

        [DllImport("winscard.dll")]
        public static extern int SCardBeginTransaction(IntPtr hCard);

        [DllImport("winscard.dll")]
        public static extern int SCardEndTransaction(IntPtr hCard, int Disposition);


        [DllImport("winscard.dll")]
        public static extern int SCardTransmit(IntPtr hCard, ref SCARD_IO_REQUEST ioSendRequest, byte[] InBuff, int InBuffSize, ref SCARD_IO_REQUEST ioRecvRequest, byte[] OutBuff, out int OutBuffSize);


        [DllImport("winscard.dll")]
        public static extern int SCardControl(IntPtr hCard, int ControlCode, byte[] InBuffer, int InBufferSize, byte[] OutBuffer, int OutBufferSize, out int BytesReturned);

        [DllImport("winscard.dll")]
        public static extern int SCardLocateCards(IntPtr hContext, byte[] mszCards, SCARD_READERSTATE[] rgReaderStates, int cReaders);

        // STRUCTURES

        [StructLayout(LayoutKind.Sequential)]
        public struct SCARD_IO_REQUEST
        {
            public int dwProtocol;
            public int cbPciLength;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SCARD_READERSTATE
        {
            [MarshalAs(UnmanagedType.LPTStr)]
            public string RdrName;
            public IntPtr UserData;
            public int RdrCurrState;
            public int RdrEventState;
            public int ATRLength;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 36)]
            public byte[] ATRBytes;
        }

        public const int SCARD_INFINITE_WAIT   = -1;
        
        // CONTEXT SCOPE

		public const int SCARD_SCOPE_USER      = 0;
		public const int SCARD_SCOPE_TERMINAL  = 1;
		public const int SCARD_SCOPE_SYSTEM    = 2;

        public const int SCARD_SHARE_EXCLUSIVE = 1;
        public const int SCARD_SHARE_SHARED    = 2;
        public const int SCARD_SHARE_DIRECT    = 3;

        // CARD STATES

        public const int SCARD_STATE_UNAWARE     = 0x00;
		public const int SCARD_STATE_IGNORE      = 0x01;
		public const int SCARD_STATE_CHANGED     = 0x02;
		public const int SCARD_STATE_UNKNOWN     = 0x04;
		public const int SCARD_STATE_UNAVAILABLE = 0x08;
		public const int SCARD_STATE_EMPTY       = 0x10;
		public const int SCARD_STATE_PRESENT     = 0x20;
		public const int SCARD_STATE_ATRMATCH    = 0x40;
		public const int SCARD_STATE_EXCLUSIVE   = 0x80;
		public const int SCARD_STATE_INUSE       = 0x100;
		public const int SCARD_STATE_MUTE        = 0x200;
		public const int SCARD_STATE_UNPOWERED   = 0x400;

        // READER STATES

        //public const int SCARD_UNKNOWN    = 0;
        //public const int SCARD_ABSENT     = 1;
        //public const int SCARD_PRESENT    = 2;
        //public const int SCARD_SWALLOWED  = 3;
        //public const int SCARD_POWERED    = 4;
        //public const int SCARD_NEGOTIABLE = 5;
        //public const int SCARD_SPECIFIC   = 6;
        
        // DISPOSITION

        public const int SCARD_LEAVE_CARD =   0;
		public const int SCARD_RESET_CARD =   1;
		public const int SCARD_UNPOWER_CARD = 2;
		public const int SCARD_EJECT_CARD =   3;

		// ERROR CODES

        public const int SCARD_S_SUCCESS = 0;

        public const int SCARD_F_INTERNAL_ERROR          = unchecked((int)0x80100001);
        public const int SCARD_E_CANCELLED               = unchecked((int)0x80100002);
        public const int SCARD_E_INVALID_HANDLE          = unchecked((int)0x80100003);
        public const int SCARD_E_INVALID_PARAMETER       = unchecked((int)0x80100004);
        public const int SCARD_E_INVALID_TARGET          = unchecked((int)0x80100005);
        public const int SCARD_E_NO_MEMORY               = unchecked((int)0x80100006);
        public const int SCARD_F_WAITED_TOO_LONG         = unchecked((int)0x80100007);
        public const int SCARD_E_INSUFFICIENT_BUFFER     = unchecked((int)0x80100008);
        public const int SCARD_E_UNKNOWN_READER          = unchecked((int)0x80100009);
        public const int SCARD_E_TIMEOUT                 = unchecked((int)0x8010000A);
        public const int SCARD_E_SHARING_VIOLATION       = unchecked((int)0x8010000B);
        public const int SCARD_E_NO_SMARTCARD            = unchecked((int)0x8010000C);
        public const int SCARD_E_UNKNOWN_CARD            = unchecked((int)0x8010000D);
        public const int SCARD_E_CANT_DISPOSE            = unchecked((int)0x8010000E);
        public const int SCARD_E_PROTO_MISMATCH          = unchecked((int)0x8010000F);
        public const int SCARD_E_NOT_READY               = unchecked((int)0x80100010);
        public const int SCARD_E_INVALID_VALUE           = unchecked((int)0x80100011);
        public const int SCARD_E_SYSTEM_CANCELLED        = unchecked((int)0x80100012);
        public const int SCARD_F_COMM_ERROR              = unchecked((int)0x80100013);
        public const int SCARD_F_UNKNOWN_ERROR           = unchecked((int)0x80100014);
        public const int SCARD_E_INVALID_ATR             = unchecked((int)0x80100015);
        public const int SCARD_E_NOT_TRANSACTED          = unchecked((int)0x80100016);
        public const int SCARD_E_READER_UNAVAILABLE      = unchecked((int)0x80100017);
        public const int SCARD_P_SHUTDOWN                = unchecked((int)0x80100018);
        public const int SCARD_E_PCI_TOO_SMALL           = unchecked((int)0x80100019);
        public const int SCARD_E_READER_UNSUPPORTED      = unchecked((int)0x8010001A);
        public const int SCARD_E_DUPLICATE_READER        = unchecked((int)0x8010001B);
        public const int SCARD_E_CARD_UNSUPPORTED        = unchecked((int)0x8010001C);
        public const int SCARD_E_NO_SERVICE              = unchecked((int)0x8010001D);
        public const int SCARD_E_SERVICE_STOPPED         = unchecked((int)0x8010001E);
        public const int SCARD_E_UNEXPECTED              = unchecked((int)0x8010001F);
        public const int SCARD_E_ICC_INSTALLATION        = unchecked((int)0x80100020);
        public const int SCARD_E_ICC_CREATEORDER         = unchecked((int)0x80100021);
        public const int SCARD_E_UNSUPPORTED_FEATURE     = unchecked((int)0x80100022);
        public const int SCARD_E_DIR_NOT_FOUND           = unchecked((int)0x80100023);
        public const int SCARD_E_FILE_NOT_FOUND          = unchecked((int)0x80100024);
        public const int SCARD_E_NO_DIR                  = unchecked((int)0x80100025);
        public const int SCARD_E_NO_FILE                 = unchecked((int)0x80100026);
        public const int SCARD_E_NO_ACCESS               = unchecked((int)0x80100027);
        public const int SCARD_E_WRITE_TOO_MANY          = unchecked((int)0x80100028);
        public const int SCARD_E_BAD_SEEK                = unchecked((int)0x80100029);
        public const int SCARD_E_INVALID_CHV             = unchecked((int)0x8010002A);
        public const int SCARD_E_UNKNOWN_RES_MNG         = unchecked((int)0x8010002B);
        public const int SCARD_E_NO_SUCH_CERTIFICATE     = unchecked((int)0x8010002C);
        public const int SCARD_E_CERTIFICATE_UNAVAILABLE = unchecked((int)0x8010002D);
        public const int SCARD_E_NO_READERS_AVAILABLE    = unchecked((int)0x8010002E);
        public const int SCARD_E_COMM_DATA_LOST          = unchecked((int)0x8010002F);
        public const int SCARD_E_NO_KEY_CONTAINER        = unchecked((int)0x80100030);
        public const int SCARD_E_SERVER_TOO_BUSY         = unchecked((int)0x80100031);
        public const int SCARD_W_UNSUPPORTED_CARD        = unchecked((int)0x80100065);
        public const int SCARD_W_UNRESPONSIVE_CARD       = unchecked((int)0x80100066);
        public const int SCARD_W_UNPOWERED_CARD          = unchecked((int)0x80100067);
        public const int SCARD_W_RESET_CARD              = unchecked((int)0x80100068);
        public const int SCARD_W_REMOVED_CARD            = unchecked((int)0x80100069);
        public const int SCARD_W_SECURITY_VIOLATION      = unchecked((int)0x8010006A);
        public const int SCARD_W_WRONG_CHV               = unchecked((int)0x8010006B);
        public const int SCARD_W_CHV_BLOCKED             = unchecked((int)0x8010006C);
        public const int SCARD_W_EOF                     = unchecked((int)0x8010006D);
        public const int SCARD_W_CANCELLED_BY_USER       = unchecked((int)0x8010006E);
        public const int SCARD_W_CARD_NOT_AUTHENTICATED  = unchecked((int)0x8010006F);
        public const int SCARD_W_CACHE_ITEM_NOT_FOUND    = unchecked((int)0x80100070);
        public const int SCARD_W_CACHE_ITEM_STALE        = unchecked((int)0x80100071);
        public const int SCARD_W_CACHE_ITEM_TOO_BIG      = unchecked((int)0x80100072);

		//  PROTOCOLS

        public const int SCARD_PROTOCOL_UNDEFINED = 0x00;       // There is no active protocol.
		public const int SCARD_PROTOCOL_T0        = 0x01;       // T=0 is the active protocol.
		public const int SCARD_PROTOCOL_T1        = 0x02;       // T=1 is the active protocol.
		public const int SCARD_PROTOCOL_RAW       = 0x10000;    // Raw is the active protocol.
		//public const int SCARD_PROTOCOL_DEFAULT   = 0x80000000; // Use implicit PTS.

        // HELPERS
        
		public static string GetScardErrMsg(int ReturnCode)
		{
			switch(ReturnCode)
			{
				case SCARD_E_CANCELLED:
					return("The action was canceled by an SCardCancel request.");
				case SCARD_E_CANT_DISPOSE:
					return("The system could not dispose of the media in the requested manner.");
				case SCARD_E_CARD_UNSUPPORTED:
					return("The smart card does not meet minimal requirements for support.");
				case SCARD_E_DUPLICATE_READER:
					return("The reader driver didn't produce a unique reader name.");
				case SCARD_E_INSUFFICIENT_BUFFER:
					return("The data buffer for returned data is too small for the returned data.");
				case SCARD_E_INVALID_ATR:
					return("An ATR string obtained from the registry is not a valid ATR string.");
				case SCARD_E_INVALID_HANDLE:
					return("The supplied handle was invalid.");
				case SCARD_E_INVALID_PARAMETER:
					return("One or more of the supplied parameters could not be properly interpreted.");
				case SCARD_E_INVALID_TARGET:
					return("Registry startup information is missing or invalid.");
				case SCARD_E_INVALID_VALUE:
					return("One or more of the supplied parameter values could not be properly interpreted.");
				case SCARD_E_NOT_READY:
					return("The reader or card is not ready to accept commands.");
				case SCARD_E_NOT_TRANSACTED:
					return("An attempt was made to end a non-existent transaction.");
				case SCARD_E_NO_MEMORY:
					return("Not enough memory available to complete this command.");
				case SCARD_E_NO_SERVICE:
					return("The smart card resource manager is not running.");
				case SCARD_E_NO_SMARTCARD:
					return("The operation requires a smart card, but no smart card is currently in the device.");
				case SCARD_E_PCI_TOO_SMALL:
					return("The PCI receive buffer was too small.");
				case SCARD_E_PROTO_MISMATCH:
					return("The requested protocols are incompatible with the protocol currently in use with the card.");
				case SCARD_E_READER_UNAVAILABLE:
					return("The specified reader is not currently available for use.");
				case SCARD_E_READER_UNSUPPORTED:
					return("The reader driver does not meet minimal requirements for support.");
				case SCARD_E_SERVICE_STOPPED:
					return("The smart card resource manager has shut down.");
				case SCARD_E_SHARING_VIOLATION:
					return("The smart card cannot be accessed because of other outstanding connections.");
				case SCARD_E_SYSTEM_CANCELLED:
					return("The action was canceled by the system, presumably to log off or shut down.");
				case SCARD_E_TIMEOUT:
					return("The user-specified timeout value has expired.");
				case SCARD_E_UNKNOWN_CARD:
					return("The specified smart card name is not recognized.");
				case SCARD_E_UNKNOWN_READER:
					return("The specified reader name is not recognized.");
				case SCARD_F_COMM_ERROR:
					return("An internal communications error has been detected.");
				case SCARD_F_INTERNAL_ERROR: 
					return("An internal consistency check failed.");
				case SCARD_F_UNKNOWN_ERROR:
					return("An internal error has been detected, but the source is unknown.");
				case SCARD_F_WAITED_TOO_LONG: 
					return("An internal consistency timer has expired.");
				case SCARD_S_SUCCESS:
					return("No error was encountered.");
				case SCARD_W_REMOVED_CARD:
					return("The smart card has been removed, so that further communication is not possible.");
				case SCARD_W_RESET_CARD:
					return("The smart card has been reset, so any shared state information is invalid.");
				case SCARD_W_UNPOWERED_CARD:
					return("Power has been removed from the smart card, so that further communication is not possible.");
				case SCARD_W_UNRESPONSIVE_CARD:
					return("The smart card is not responding to a reset.");
				case SCARD_W_UNSUPPORTED_CARD:
					return("The reader cannot communicate with the card, due to ATR string configuration conflicts.");
                case SCARD_E_COMM_DATA_LOST:
                    return ("A communications error with the smart card has been detected.");
				default:
					return("0x" + ReturnCode.ToString("X") + " " + ReturnCode + " ?");
			}
		}

	}

