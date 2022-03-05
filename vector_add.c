#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <numpy/arrayobject.h>

extern double * myVectorAdd(double * h_A, double * h_B, int numElements);

static PyObject* helloworld(PyObject* self, PyObject* args) {
    printf("Hello World\n");
    return Py_None;
}

static PyObject* vector_add(PyObject* self, PyObject* args) {
    PyArrayObject* array1, * array2;
    double * output;

    if (!PyArg_ParseTuple(args, "O!O!", &PyArray_Type, &array1, &PyArray_Type, &array2))
        return NULL;

    if (array1 -> nd != 1 || array2 -> nd != 1 || array1->descr->type_num != PyArray_DOUBLE || array2->descr->type_num != PyArray_DOUBLE) {
        PyErr_SetString(PyExc_ValueError, "arrays must be one-dimensional and of type float");
        return NULL;
    }

    int n1 = array1->dimensions[0];
    int n2 = array2->dimensions[0];

    printf("running vector_add on dim1: %d, stride1: %d, dim2: %d, stride2: %d\n", n1, array1->strides[0], n2, array2->strides[0]);

    if (n1 != n2) {
        PyErr_SetString(PyExc_ValueError, "arrays must have the same length");
        return NULL;
    }

    output = myVectorAdd((double *) array1 -> data, (double *) array2 -> data, n1);

    return PyArray_SimpleNewFromData(1, PyArray_DIMS(array1), PyArray_TYPE(array1), output);
}

static PyObject* vector_add_cpu(PyObject* self, PyObject* args) {
    PyArrayObject* array1, * array2;

    if (!PyArg_ParseTuple(args, "O!O!", &PyArray_Type, &array1, &PyArray_Type, &array2))
        return NULL;

    if (array1 -> nd != 1 || array2 -> nd != 1 || array1->descr->type_num != PyArray_DOUBLE || array2->descr->type_num != PyArray_DOUBLE) {
        PyErr_SetString(PyExc_ValueError, "arrays must be one-dimensional and of type float");
        return NULL;
    }

    int n1 = array1->dimensions[0];
    int n2 = array2->dimensions[0];

    printf("running vector_add on dim1: %d, stride1: %d, dim2: %d, stride2: %d\n", n1, array1->strides[0], n2, array2->strides[0]);

    if (n1 != n2) {
        PyErr_SetString(PyExc_ValueError, "arrays must have the same length");
        return NULL;
    }

    double * output = (double *) malloc(sizeof(double) * n1);

    for (int i = 0; i < n1; i++)
        output[i] = *((double *) array1 -> data + i) + *((double *) array2 -> data + i);

    return PyArray_SimpleNewFromData(1, PyArray_DIMS(array1), PyArray_TYPE(array1), output);
}

static PyMethodDef methods[] = {
    {"helloworld", helloworld, METH_NOARGS, "Prints Hello World"},
    {"vector_add_cpu", vector_add_cpu, METH_VARARGS, "add two vectors on the CPU"},
    {"vector_add", vector_add, METH_VARARGS, "add two vectors with CUDA"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT, "vector_add",
    "vector_add", -1, methods
};

PyMODINIT_FUNC PyInit_vector_add(void) {
    import_array(); 
    return PyModule_Create(&module);
}

