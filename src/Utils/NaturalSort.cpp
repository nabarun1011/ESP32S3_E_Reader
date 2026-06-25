#include "Utils/NaturalSort.hpp"

static bool IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

int NaturalCompare(
    const String &a,
    const String &b)
{
    size_t ia = 0;
    size_t ib = 0;

    while (ia < a.length() &&
           ib < b.length())
    {
        char ca = a[ia];
        char cb = b[ib];

        if (IsDigit(ca) &&
            IsDigit(cb))
        {
            size_t za = ia;
            size_t zb = ib;

            while (za < a.length() &&
                   a[za] == '0')
            {
                za++;
            }

            while (zb < b.length() &&
                   b[zb] == '0')
            {
                zb++;
            }

            size_t enda = za;
            size_t endb = zb;

            while (enda < a.length() &&
                   IsDigit(a[enda]))
            {
                enda++;
            }

            while (endb < b.length() &&
                   IsDigit(b[endb]))
            {
                endb++;
            }

            size_t lena = enda - za;
            size_t lenb = endb - zb;

            if (lena != lenb)
            {
                return lena < lenb
                           ? -1
                           : 1;
            }

            for (size_t i = 0;
                 i < lena;
                 ++i)
            {
                if (a[za + i] !=
                    b[zb + i])
                {
                    return a[za + i] <
                                   b[zb + i]
                               ? -1
                               : 1;
                }
            }

            size_t totalDigitsA =
                enda - ia;

            size_t totalDigitsB =
                endb - ib;

            if (totalDigitsA !=
                totalDigitsB)
            {
                return totalDigitsA <
                               totalDigitsB
                           ? -1
                           : 1;
            }

            ia = enda;
            ib = endb;

            continue;
        }

        ca = tolower(ca);
        cb = tolower(cb);

        if (ca != cb)
        {
            return ca < cb
                       ? -1
                       : 1;
        }

        ia++;
        ib++;
    }

    if (ia < a.length())
    {
        return 1;
    }

    if (ib < b.length())
    {
        return -1;
    }

    return 0;
}

bool NaturalLess(
    const String &a,
    const String &b)
{
    return NaturalCompare(a, b) < 0;
}