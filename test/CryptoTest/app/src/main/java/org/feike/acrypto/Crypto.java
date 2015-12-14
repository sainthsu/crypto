package org.feike.acrypto;

/**
 * Created by saint on 14/12/2015.
 */
public class Crypto {
    public static final int SHA1 = 1;
    public static final int SHA224 = 2;
    public static final int SHA256 = 3;
    public static final int SHA384 = 4;
    public static final int SHA512 = 5;

    static {
        System.loadLibrary("acrypto");
    }

    public static native byte[] MD5(byte[] data);
    public static native byte[] MD5(String filename);
    public static native byte[] AESEncode(byte[] data,byte[] key);
    public static native byte[] AESDecode(byte[] data,byte[] key);
    //public static native byte[] RSAEncode(byte[] data,byte[] key);
    //public static native byte[] RSADecode(byte[] data,byte[] key);
    public static native byte[] Base64Encode(byte[] data);
    public static native byte[] Base64Decode(byte[] data);

    public static native byte[] SHA(byte[] data,int type);

}
