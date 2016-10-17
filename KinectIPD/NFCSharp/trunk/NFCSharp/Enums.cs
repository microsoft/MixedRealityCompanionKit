using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NFCSharp
{
    public enum ReaderBrand
    {
        Unknown,
        ACS, DUALi, Identive
    }

    public enum TagType
    {
        Unknown, Felica212K, Felica424K, Topaz,
        MifareUltralightFamily, MifareDESFire, MifareMini, 
        MifareStandard1K, MifareStandard4K, MifarePlus2K, MifarePlus4K,
        ISO14443A, ISO14443A_part1, ISO14443A_part2, ISO14443A_part3,
        ISO14443B, ISO14443B_part1, ISO14443B_part2, ISO14443B_part3,
        ISO15693, ISO15693_part1, ISO15693_part2, ISO15693_part3, ISO15693_part4,
        ContactCard7816_10_IIC, ContactCard7816_10_ExtendedIIC, ContactCard7816_10_2WBP, ContactCard7816_10_3WBP
    }

    public enum KeyTypes { TypeA, TypeB };
}
