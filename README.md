## ZBIC

This project provides a Python package that supports compression and decompression of the ZBIC algorithm that was first seen on the Nintendo Switch in version 22.0.0.

The `zstd.c`, `zstd.h` and `zstd_errors.h` files in this repository were copied from [Atmosphere-NX](https://github.com/Atmosphere-NX/Atmosphere).

### Installation
The package can be installed with pip: `pip3 install zbic`.

### API reference
```python
def compress(data: bytes) -> bytes: ...
def decompress(data: bytes) -> bytes: ...
```

### Example usage
```python
import zbic

data = b"Hello World!"
compressed = zbic.compress(data)
decompressed = zbic.decompress(compressed)
assert compressed == decompressed
```

### Source code
https://github.com/kinnay/zbic
