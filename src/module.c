
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdint.h>
#include <string.h>

#include "zstd.h"


static PyObject *ZBICError = NULL;

void ZBIC_set_error(size_t error) {
    const char *name = ZSTD_getErrorName(error);
    PyErr_SetString(ZBICError, name);
}

PyObject *ZBIC_compress(PyObject *self, PyObject *args) {
	const uint8_t *in;
	size_t inlen;
    int level = 3;
	if (!PyArg_ParseTuple(args, "y#|i", &in, &inlen, &level)) {
		return NULL;
	}

    size_t size = ZSTD_compressBound(inlen);
    if (ZSTD_isError(size)) {
        ZBIC_set_error(size);
        return NULL;
    }

    void *out = PyMem_Malloc(size);
    if (!out) {
        return PyErr_NoMemory();
    }

    size_t result = ZSTD_compress(out, size, in, inlen, level);
    if (ZSTD_isError(result)) {
        PyMem_Free(out);
        ZBIC_set_error(result);
        return NULL;
    }
	
	PyObject *bytes = PyBytes_FromStringAndSize(out, result);
	PyMem_Free(out);

	return bytes;
}

PyObject *ZBIC_decompress(PyObject *self, PyObject *args) {
	const uint8_t *in;
	size_t inlen;
	if (!PyArg_ParseTuple(args, "y#", &in, &inlen)) {
		return NULL;
	}

    // We currently assume that the ZBIC data contain exactly one frame
    unsigned long long size = ZSTD_getFrameContentSize(in, inlen);
    if (size == ZSTD_CONTENTSIZE_ERROR) {
        PyErr_SetString(ZBICError, "ZBIC data is invalid");
        return NULL;
    }
    else if (size == ZSTD_CONTENTSIZE_UNKNOWN) {
        // We currently assume that the header always contains the decompressed size.
        // 
        // If this turns out not to be true, we need to fix this later, either by
        // using ZSTD_DStream or by adding the decompressed size as a parameter to
        // this function.
        PyErr_SetString(ZBICError, "ZBIC header does not specify decompressed size");
        return NULL;
    }

    PyObject *bytes = PyBytes_FromStringAndSize(NULL, size);
    if (!bytes) {
        return NULL;
    }

    void *out = PyBytes_AsString(bytes);

    size_t result = ZSTD_decompress(out, size, in, inlen);
    if (ZSTD_isError(result)) {
        Py_DECREF(bytes);
        ZBIC_set_error(result);
        return NULL;
    }
	
	return bytes;
}

int ZBIC_module_exec(PyObject *m) {
    if (ZBICError != NULL) {
        PyErr_SetString(
            PyExc_ImportError, "cannot initialize zbic module more than once"
        );
        return -1;
    }

    ZBICError = PyErr_NewException("zbic.error", NULL, NULL);
    if (PyModule_AddObjectRef(m, "error", ZBICError) < 0) {
        return -1;
    }

    return 0;
}

PyMethodDef ZBICMethods[] = {
	{"compress", ZBIC_compress, METH_VARARGS, NULL},
	{"decompress", ZBIC_decompress, METH_VARARGS, NULL},
	{NULL, NULL, 0, NULL}
};

PyModuleDef_Slot ZBICSlots[] = {
    {Py_mod_exec, ZBIC_module_exec},
    {0, NULL}
};

PyModuleDef ZBICModule = {
	PyModuleDef_HEAD_INIT,
	"zbic",
	"ZBIC compression methods",
	0,

	ZBICMethods,
    ZBICSlots
};

PyMODINIT_FUNC PyInit_zbic() {
	return PyModuleDef_Init(&ZBICModule);
}
