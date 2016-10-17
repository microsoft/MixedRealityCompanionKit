// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

using System;
using System.Security.Cryptography;
using System.Text;

namespace KinectIPD
{
    public class EncDec
    {
        private static Encoding _encoding = Encoding.Default;
        public static byte[] Protect(string stringData)
        {            
            return Protect(_encoding.GetBytes(stringData));
        }
        public static byte[] Protect(byte[] data)
        {
            try
            {
                // Encrypt the data using DataProtectionScope.CurrentUser. The result can be decrypted
                //  only by the same current user.
                return ProtectedData.Protect(data, null, DataProtectionScope.CurrentUser);
            }
            catch (CryptographicException e)
            {
                Console.WriteLine("Data was not encrypted. An error occurred.");
                Console.WriteLine(e.ToString());
                return null;
            }
        }

        public static string UnprotectString(byte[] data)
        {
            return  _encoding.GetString(Unprotect(data));
        }

        public static byte[] Unprotect(byte[] data)
        {
            try
            {
                //Decrypt the data using DataProtectionScope.CurrentUser.
                return ProtectedData.Unprotect(data, null, DataProtectionScope.CurrentUser);
            }
            catch (CryptographicException e)
            {
                Console.WriteLine("Data was not decrypted. An error occurred.");
                Console.WriteLine(e.ToString());
                return null;
            }
        }
    }
}
