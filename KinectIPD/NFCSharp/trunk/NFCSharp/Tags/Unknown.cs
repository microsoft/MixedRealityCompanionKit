using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NFCSharp.Tags
{
    class Unknown : NFCTag
    {
        public Unknown(IntPtr Handle, int Proto, NFCReader Reader, byte[] ATRbytes) : base(Handle, Proto, Reader, ATRbytes) { }

        public override void NDEFFormat()
        {
            throw new NotSupportedException();
        }
        
        public override byte[] ReadAll()
        {
            throw new NotSupportedException();
        }

        public override void WriteAll(byte[] Data)
        {
            throw new NotSupportedException();
        }

        public override void Lock()
        {
            throw new NotSupportedException();
        }
    }
}
