#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "./AS_astar.c"
#include "./AS_stack.c"

static PyObject *astar_search(PyObject *self, PyObject *args);

double euclidian_distance(AS_ANode *self, AS_ANode *target) {
    double x1, y1, x2, y2;
    PyObject *seq1 = (PyObject *)self->data;
    PyObject *seq2 = (PyObject *)target->data;
    x1 = PyFloat_AsDouble(PySequence_GetItem(seq1, 0));
    y1 = PyFloat_AsDouble(PySequence_GetItem(seq1, 1));
    x2 = PyFloat_AsDouble(PySequence_GetItem(seq2, 0));
    y2 = PyFloat_AsDouble(PySequence_GetItem(seq2, 1));

    return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
}

int position_compare(AS_ANode *self, AS_ANode *target) {
    double x1, y1, x2, y2;
    PyObject *seq1 = (PyObject *)self->data;
    PyObject *seq2 = (PyObject *)target->data;
    x1 = PyFloat_AsDouble(PySequence_GetItem(seq1, 0));
    y1 = PyFloat_AsDouble(PySequence_GetItem(seq1, 1));
    x2 = PyFloat_AsDouble(PySequence_GetItem(seq2, 0));
    y2 = PyFloat_AsDouble(PySequence_GetItem(seq2, 1));

    return x1 == x2 && y1 == y2;
}

static PyObject *
astar_search(PyObject *self, PyObject *args) {
    PyObject *dict, *py_start, *py_end, *dict_keys, *dict_values, *ret_list;
    AS_ANode *node_arr;
    Py_ssize_t node_arr_length;
    AS_ANode *node_start, *node_end;

    PyObject *exc;

    if (!PyArg_ParseTuple(args, "OOO", &dict, &py_start, &py_end)) {
        return NULL;
    }

    if (!PyDict_Check(dict)) {
        PyErr_SetString(PyExc_TypeError, "nodes must be a dictionary");
        return NULL;
    }

    dict_keys = PyDict_Keys(dict);
    node_arr_length = PyList_Size(dict_keys);
    node_arr = malloc(node_arr_length * sizeof(AS_ANode));

    for (Py_ssize_t i = 0; i < node_arr_length; ++i) {
        AS_ANode node;
        node.data = (void *)PyList_GetItem(dict_keys, i);
        node.distance_to = &euclidian_distance;
        node_arr[i] = node;

        if (PyObject_RichCompareBool((PyObject *)node.data, py_start, Py_EQ)) {
            node_start = &node;
        } else if (PyObject_RichCompareBool((PyObject *)node.data, py_end, Py_EQ)) {
            node_end = &node;
        }
    }

    dict_values = PyDict_Values(dict);

    for (Py_ssize_t i = 0; i < node_arr_length; ++i) {
        AS_ANode *node = &node_arr[i];
        PyObject *neighbours = PyList_GetItem(dict_values, i);

        node->neighbour_count = PySequence_Size(neighbours);
        node->neighbours = malloc(node->neighbour_count * sizeof(AS_ANode *));

        for (Py_ssize_t j = 0; j < (Py_ssize_t)node->neighbour_count; ++j) {
            for (Py_ssize_t k = 0; k < node_arr_length; ++k) {
                AS_ANode *neighbour = &node_arr[k];
                AS_ANode temp;
                temp.data = PySequence_GetItem(neighbours, j);
                if (position_compare(neighbour, &temp)) {
                    node->neighbours[j] = neighbour;
                    break;
                }
            }
        }
    }

    if (AS_AStarSearch(node_arr, node_arr_length, node_start, node_end, &position_compare, &euclidian_distance) == 0) {
        AS_Stack stack;
        AS_AStarReconstructPath(node_end, &stack);
        ret_list = PyList_New(stack.size);
        for (Py_ssize_t i = 0; i < (Py_ssize_t)stack.size; ++i) {
            PyObject *element = stack.pop(&stack);
            Py_INCREF(element);
            PyList_SetItem(ret_list, i, element);
        }
        return ret_list;
    }

    Py_RETURN_NONE;
}

static PyMethodDef astarMethods[] = {
    {"search",  astar_search, METH_VARARGS,
     NULL},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef astarmodule = {
    PyModuleDef_HEAD_INIT,
    "astar",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    astarMethods
};

PyMODINIT_FUNC
PyInit_astar(void)
{
    return PyModule_Create(&astarmodule);
}
