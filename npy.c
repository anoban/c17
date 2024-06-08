typedef struct npy_t {
        char           _npymagic[6]; // magic string "\x93NUMPY"
        unsigned char  _vmajor;      // major version of the file format
        unsigned char  _vminor;      // minor version of the file format
        unsigned short _headerlen;   // header length (in LE byte order)
        char*          _header;      // a character buffer of _headerlen in length
        void*          _array;       // the array data
} npy_t;
